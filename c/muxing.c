/*
 * Copyright (c) 2003 Fabrice Bellard
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/*
 * @file
 * libavformat API example.
 *
 * Output a media file in any supported libavformat format. The default
 * codecs are used.
 * @example muxing.c
 */

#include <libavutil/avutil.h>
#include <libavutil/opt.h>
#include <libavutil/timestamp.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>

#define STREAM_DURATION   10.0
#define STREAM_FRAME_RATE 25 /* 25 images/s */
#define STREAM_PIX_FMT    AV_PIX_FMT_YUV420P /* default pix_fmt */

#define SCALE_FLAGS SWS_BICUBIC

// a wrapper around a single output AVStream
typedef struct Container {
    AVStream *stream;
    AVCodecContext *encoder;

    /* pts of the next frame that will be generated */
    int64_t next_pts;
    int samples_count;

    AVFrame *frame;
    AVFrame *tmp_frame;

    struct SwsContext *sws_ctx;
} Container;

void log_packet(const AVFormatContext *fmt_ctx, const AVPacket *pkt);
int write_frame(AVFormatContext *fmt_ctx, const AVRational *time_base, 
					AVStream *stream, AVPacket *pkt);
void constructor(Container *container, AVFormatContext *formatContext, AVCodec **codec);
AVFrame *alloc_image(enum AVPixelFormat pix_fmt, int width, int height);
void open_video(AVFormatContext *oc, AVCodec *codec, Container *container,
	       	AVDictionary *opt_arg);
void fill_yuv_image(AVFrame *pict, int frame_index, int width, int height);
AVFrame *get_video_frame(Container *container);
int write_video_frame(AVFormatContext *oc, Container *container);
void destructor(AVFormatContext *oc, Container *container);

int main(int argc, char **argv)
{
    struct Container video_st = { 0 };

    const char *filename;
    AVOutputFormat *fmt;
    AVFormatContext *oc;
    AVCodec *video_codec;
    int ret;
    int have_video = 0;
    int encode_video = 0;
    AVDictionary *opt = NULL;
    int i;

    if (argc < 2) {
        printf("usage: %s output_file\n"
               "API example program to output a media file with libavformat.\n"
               "This program generates a synthetic audio and video stream, encodes and\n"
               "muxes them into a file named output_file.\n"
               "The output format is automatically guessed according to the file extension.\n"
               "Raw images can also be output by using '%%d' in the filename.\n"
               "\n", argv[0]);
        exit(1);
    }

    filename = argv[1];

    /* allocate the output media context */
    avformat_alloc_output_context2(&oc, NULL, NULL, filename);
    if (!oc) {
        printf("Could not deduce output format from file extension: using MPEG.\n");
	exit(1);
        avformat_alloc_output_context2(&oc, NULL, "mpeg", filename);
    }
    if (!oc)
        return 1;

    printf("FORMAT: %s\n\n\n\n", oc->oformat->long_name);
    fmt = oc->oformat;

    /* Add the audio and video streams using the default format codecs
     * and initialize the codecs. */
    if (fmt->video_codec != AV_CODEC_ID_NONE) {
        constructor(&video_st, oc, &video_codec);
        have_video = 1;
        encode_video = 1;
    }

    /* Now that all the parameters are set, we can open the audio and
     * video codecs and allocate the necessary encode buffers. */
    if (have_video)
        open_video(oc, video_codec, &video_st, opt);

    av_dump_format(oc, 0, filename, 1);

    /* open the output file, if needed */
    if (!(fmt->flags & AVFMT_NOFILE)) {
        ret = avio_open(&oc->pb, filename, AVIO_FLAG_WRITE);
        if (ret < 0) {
            fprintf(stderr, "Could not open '%s': %s\n", filename,
                    av_err2str(ret));
            return 1;
        }
    }

    /* Write the stream header, if any. */
    ret = avformat_write_header(oc, &opt);
    if (ret < 0) {
        fprintf(stderr, "Error occurred when opening output file: %s\n",
                av_err2str(ret));
        return 1;
    }

    while (encode_video) {
            encode_video = !write_video_frame(oc, &video_st);
    }

    /* Write the trailer, if any. The trailer must be written before you
     * close the CodecContexts open when you wrote the header; otherwise
     * av_write_trailer() may try to use memory that was freed on
     * av_codec_close(). */
    av_write_trailer(oc);

    /* Close each codec. */
    if (have_video)
        destructor(oc, &video_st);

    if (!(fmt->flags & AVFMT_NOFILE))
        /* Close the output file. */
        avio_closep(&oc->pb);

    /* free the stream */
    avformat_free_context(oc);

    return 0;
}

void log_packet(const AVFormatContext *fmt_ctx, const AVPacket *pkt)
{
    AVRational *time_base = &fmt_ctx->streams[pkt->stream_index]->time_base;

    printf("pts:%s pts_time:%s dts:%s dts_time:%s duration:%s duration_time:%s stream_index:%d\n",
           av_ts2str(pkt->pts), av_ts2timestr(pkt->pts, time_base),
           av_ts2str(pkt->dts), av_ts2timestr(pkt->dts, time_base),
           av_ts2str(pkt->duration), av_ts2timestr(pkt->duration, time_base),
           pkt->stream_index);
}

int write_frame(AVFormatContext *fmt_ctx, const AVRational *time_base, AVStream *stream, AVPacket *pkt)
{
    /* rescale output packet timestamp values from codec to stream timebase */
    av_packet_rescale_ts(pkt, *time_base, stream->time_base);
    pkt->stream_index = stream->index;

    /* Write the compressed frame to the media file. */
    //log_packet(fmt_ctx, pkt);
    return av_interleaved_write_frame(fmt_ctx, pkt);
}

/* Add an output stream. */
void constructor(Container *container, AVFormatContext *formatContext,
                       AVCodec **codec) {
   AVCodecContext *context = NULL;
   char *codec_id = "libx264";
   int i;

   /* find the encoder */
   *codec = avcodec_find_encoder_by_name(codec_id);
   if (!(*codec)) {
       fprintf(stderr, "Could not find encoder for '%s'\n", codec_id);
       exit(1);
   }

   container->stream = avformat_new_stream(formatContext, NULL);
   if (!container->stream) {
       fprintf(stderr, "Could not allocate stream\n");
       exit(1);
   }
   container->stream->id = formatContext->nb_streams - 1;
   context = avcodec_alloc_context3(*codec);
   if (!context) {
     fprintf(stderr, "Could not alloc an encoding context\n");
      exit(1);
   }

   context->codec = *codec;
   context->bit_rate = 400000;
   context->width    = 352;
   context->height   = 288;
   context->time_base = (AVRational){ 1, STREAM_FRAME_RATE };
   container->stream->time_base = context->time_base; 
 
//   container->stream->time_base = (AVRational){ 1, STREAM_FRAME_RATE };
//   context->time_base = container->stream->time_base;

   context->gop_size = 10; //emit one intra frame every twelve frames at mcontainer
   context->pix_fmt = STREAM_PIX_FMT;
   context->max_b_frames = 5;



    /* Some container formats (like MP4) require global headers to be present.
     * Mark the encoder so that it behaves accordingly. */
    if (formatContext->oformat->flags & AVFMT_GLOBALHEADER) {
        context->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

	printf("Format Context Flags: %d 0x%x\n\n", formatContext->oformat->flags & AVFMT_GLOBALHEADER, formatContext->oformat->flags & AVFMT_GLOBALHEADER);
	printf("Codec Context Flags: 0x%x\n"
			"GLOBAL_FLAG: 0x%x\n"
			"Codec Context set: 0x%x\n\n", 
			context->flags, 
			AV_CODEC_FLAG_GLOBAL_HEADER,
		       	context->flags | AV_CODEC_FLAG_GLOBAL_HEADER);
	
    }

    container->encoder = context;
    if (container->encoder->codec_id == AV_CODEC_ID_H264) {
	    printf("Setting Options\n");
       av_opt_set(container->encoder->priv_data, "preset", "fast", 0);
    }

}

/**************************************************************/
/* video output */

AVFrame *alloc_image(enum AVPixelFormat pix_fmt, int width, int height)
{
    AVFrame *picture;
    int ret;

    picture = av_frame_alloc();
    if (!picture)
        return NULL;

    picture->format = pix_fmt;
    picture->width  = width;
    picture->height = height;

    /* allocate the buffers for the frame data */
    ret = av_frame_get_buffer(picture, 32);
    if (ret < 0) {
        fprintf(stderr, "Could not allocate frame data.\n");
        exit(1);
    }

    return picture;
}

void open_video(AVFormatContext *oc, AVCodec *codec, Container *container, AVDictionary *opt_arg)
{
    int ret;
    AVCodecContext *context = container->encoder;
    AVDictionary *opt = NULL;

    av_dict_copy(&opt, opt_arg, 0);

    /* open the codec */
    ret = avcodec_open2(container->encoder, container->encoder->codec, NULL);
    av_dict_free(&opt);
    if (ret < 0) {
        fprintf(stderr, "Could not open video codec: %s\n", av_err2str(ret));
        exit(1);
    }

    /* allocate and init a re-usable frame */
    container->frame = alloc_image(context->pix_fmt, context->width, context->height);
    if (!container->frame) {
        fprintf(stderr, "Could not allocate video frame\n");
        exit(1);
    }

    /* If the output format is not YUV420P, then a temporary YUV420P
     * picture is needed too. It is then converted to the required
     * output format. */
    container->tmp_frame = NULL;
    if (context->pix_fmt != AV_PIX_FMT_YUV420P) {
        container->tmp_frame = alloc_image(AV_PIX_FMT_YUV420P, context->width, context->height);
        if (!container->tmp_frame) {
            fprintf(stderr, "Could not allocate temporary picture\n");
            exit(1);
        }
    }

    /* copy the stream parameters to the muxer */
    ret = avcodec_parameters_from_context(container->stream->codecpar, context);
    if (ret < 0) {
        fprintf(stderr, "Could not copy the stream parameters\n");
        exit(1);
    }
}

/* Prepare a dummy image. */
void fill_yuv_image(AVFrame *pict, int frame_index,
                           int width, int height)
{
    int x, y, i;

    i = frame_index;

    /* Y */
    for (y = 0; y < height; y++)
        for (x = 0; x < width; x++)
            pict->data[0][y * pict->linesize[0] + x] = x + y + i * 3;

    /* Cb and Cr */
    for (y = 0; y < height / 2; y++) {
        for (x = 0; x < width / 2; x++) {
            pict->data[1][y * pict->linesize[1] + x] = 128 + y + i * 2;
            pict->data[2][y * pict->linesize[2] + x] = 64 + x + i * 5;
        }
    }
}

AVFrame *get_video_frame(Container *container)
{
    AVCodecContext *context = container->encoder;

    /* check if we want to generate more frames */
    if (av_compare_ts(container->next_pts, context->time_base,
                      STREAM_DURATION, (AVRational){ 1, 1 }) >= 0)
        return NULL;

    /* when we pass a frame to the encoder, it may keep a reference to it
     * internally; make sure we do not overwrite it here */
    if (av_frame_make_writable(container->frame) < 0)
        exit(1);

    if (context->pix_fmt != AV_PIX_FMT_YUV420P) {
        /* as we only generate a YUV420P picture, we must convert it
         * to the codec pixel format if needed */
        if (!container->sws_ctx) {
            container->sws_ctx = sws_getContext(context->width, context->height,
                                          AV_PIX_FMT_YUV420P,
                                          context->width, context->height,
                                          context->pix_fmt,
                                          SCALE_FLAGS, NULL, NULL, NULL);
            if (!container->sws_ctx) {
                fprintf(stderr,
                        "Could not initialize the conversion context\n");
                exit(1);
            }
        }
        fill_yuv_image(container->tmp_frame, container->next_pts, context->width, context->height);
        sws_scale(container->sws_ctx, (const uint8_t * const *) container->tmp_frame->data,
                  container->tmp_frame->linesize, 0, context->height, container->frame->data,
                  container->frame->linesize);
    } else {
        fill_yuv_image(container->frame, container->next_pts, context->width, context->height);
    }

    container->frame->pts = container->next_pts++;

    return container->frame;
}

/*
 * encode one video frame and send it to the muxer
 * return 1 when encoding is finished, 0 otherwise
 */
int write_video_frame(AVFormatContext *oc, Container *container)
{
    int ret;
    AVCodecContext *context;
    AVFrame *frame;
    int got_packet = 0;
    AVPacket pkt = { 0 };

    context = container->encoder;

    frame = get_video_frame(container);

    av_init_packet(&pkt);

    /* encode the image */
    ret = avcodec_send_frame(context, frame);
    if (ret < 0 && ret != AVERROR_EOF) {
        fprintf(stderr, "Error encoding video frame: %s\n", av_err2str(ret));
        exit(1);
    } 

    got_packet = !avcodec_receive_packet(context, &pkt);
    if (got_packet) {
        ret = write_frame(oc, &context->time_base, container->stream, &pkt);
    } else {
        ret = 0;
    }

    if (ret < 0) {
        fprintf(stderr, "Error while writing video frame: %s\n", av_err2str(ret));
        exit(1);
    }

    return (frame || got_packet) ? 0 : 1;
}

void destructor(AVFormatContext *oc, Container *container)
{
    avcodec_free_context(&container->encoder);
    av_frame_free(&container->frame);
    av_frame_free(&container->tmp_frame);
    sws_freeContext(container->sws_ctx);
}