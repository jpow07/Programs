#include <stdlib.h>
#include <string.h>

#include <libavformat/avformat.h> 	// Framework for muxing and demuxing (Containers)
#include <libavutil/avutil.h>		// Utility library
#include <libavcodec/avcodec.h>		// Framework for encoding a decoding (Compression)

#define STREAM_DURATION   40.0 // Seconds
#define STREAM_FRAME_RATE 25 // 25 images
#define STREAM_PIX_FMT    AV_PIX_FMT_YUV420P /* default pix_fmt */
#define SCALE_FLAGS SWS_BICUBIC

// a wrapper around a single output AVStream
typedef struct OutputStream {
    AVStream *st;
    AVCodecContext *enc; //encoder

    // pts of the next frame that will be generated
    int64_t next_pts;		// Linked List of presentation timestamp
    AVFrame *frame;		//Frame to be captured
    AVFrame *tmp_frame;		// Temporary frame (used for backup)

} OutputStream;

int write_frame(AVFormatContext *fmt_ctx, const AVRational *time_base, AVStream *st, AVPacket *pkt)
{
    /* rescale output packet timestamp values from codec to stream timebase */
    av_packet_rescale_ts(pkt, *time_base, st->time_base);
    pkt->stream_index = st->index;

    /* Write the compressed frame to the media file. */
    return av_interleaved_write_frame(fmt_ctx, pkt);
}

/* Add an output stream. */
void create_stream(OutputStream *ost, AVFormatContext *oc,
                       AVCodec **codec,
                       enum AVCodecID codec_id)
{
    AVCodecContext *c;
    int i;

    /* find the encoder */
    *codec = avcodec_find_encoder(codec_id);
    if (!(*codec)) {
        fprintf(stderr, "Could not find encoder for %s\n", avcodec_get_name(codec_id));
        exit(1);
    }

    ost->st = avformat_new_stream(oc, NULL);
    if (!ost->st) {
        fprintf(stderr, "Could not allocate stream\n");
        exit(1);
    }

    //ost->st->id = oc->nb_streams-1;
    c = avcodec_alloc_context3(*codec);
    if (!c) {
        fprintf(stderr, "Could not alloc an encoding context\n");
        exit(1);
    }

    ost->enc = c;
    c->codec_id = codec_id;

    c->bit_rate = 400000;
    // Resolution must be a multiple of two. 
    c->width    = 352;
    c->height   = 288;

    /*
     * Timebase: This is the fundamental unit of time (in seconds) in terms
     * of which frame timestamps are represented. For fixed-fps content,
     * timebase should be 1/framerate and timestamp increments should be
     * identical to 1.
     */
    ost->st->time_base = (AVRational){ 1, STREAM_FRAME_RATE };
    c->time_base       = ost->st->time_base;
    c->gop_size      = 12; /* emit one intra frame every twelve frames at most */
    c->pix_fmt       = STREAM_PIX_FMT;

    if (c->codec_id == AV_CODEC_ID_MPEG2VIDEO) {
        // just for testing, we also add B-frames
        c->max_b_frames = 2;
    }

    if (c->codec_id == AV_CODEC_ID_MPEG1VIDEO) {
        /* 
	 * Needed to avoid using macroblocks in which some coeffs overflow.
         * This does not happen with normal video, it just happens here as
         * the motion of the chroma plane does not match the luma plane.
	 */
        c->mb_decision = 2;
    }

    // Some formats want stream headers to be separate.
    if (oc->oformat->flags & AVFMT_GLOBALHEADER)
        c->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
}

// video output

AVFrame *alloc_picture(enum AVPixelFormat pix_fmt, int width, int height)
{
    AVFrame *picture;
    int ret;

    picture = av_frame_alloc();
    if (!picture)
        return NULL;

    picture->format = pix_fmt;
    picture->width  = width;
    picture->height = height;

    // allocate the buffers for the frame data
    ret = av_frame_get_buffer(picture, 32);
    if (ret < 0) {
        fprintf(stderr, "Could not allocate frame data.\n");
        exit(1);
    }

    return picture;
}

void open_video(AVFormatContext *oc, AVCodec *codec, OutputStream *ost, AVDictionary *opt_arg)
{
    int ret;
    AVCodecContext *c = ost->enc;
    AVDictionary *opt = NULL;

    av_dict_copy(&opt, opt_arg, 0);

    // open the codec
    ret = avcodec_open2(c, codec, &opt);
    av_dict_free(&opt);
    if (ret < 0) {
        fprintf(stderr, "Could not open video codec: %s\n", av_err2str(ret));
        exit(1);
    }

    // allocate and init a re-usable frame
    ost->frame = alloc_picture(c->pix_fmt, c->width, c->height);
    if (!ost->frame) {
        fprintf(stderr, "Could not allocate video frame\n");
        exit(1);
    }

    /* 
     * If the output format is not YUV420P, then a temporary YUV420P
     * picture is needed too. It is then converted to the required
     * output format. 
     */
    ost->tmp_frame = NULL;

    // copy the stream parameters to the muxer
    ret = avcodec_parameters_from_context(ost->st->codecpar, c);
    if (ret < 0) {
        fprintf(stderr, "Could not copy the stream parameters\n");
        exit(1);
    }
}

// Prepare a dummy image.
void fill_yuv_image(AVFrame *pict, int frame_index,
                           int width, int height)
{
    int x, y, i;

    i = frame_index;

    //Y
    for (y = 0; y < height; y++)
        for (x = 0; x < width; x++)
            pict->data[0][y * pict->linesize[0] + x] = x + y * 3;

    // Cb and Cr
    for (y = 0; y < height / 2; y++) {
        for (x = 0; x < width / 2; x++) {
            pict->data[1][y * pict->linesize[1] + x] = 128 + y + i * 2;
            pict->data[2][y * pict->linesize[2] + x] = 64 + x + i * 5;
        }
    }
}

AVFrame *get_video_frame(OutputStream *ost)
{
    AVCodecContext *c = ost->enc;

// REPLACE with is stream active()? continue : return NULL;
    // check if we want to generate more frames
    if (av_compare_ts(ost->next_pts, c->time_base,
                     STREAM_DURATION, (AVRational){ 1, 1}) >= 0)
        return NULL;
//
//


    /* 
     * When we pass a frame to the encoder, it may keep a reference to it
     * internally; make sure we do not overwrite it here 
     */
    int ret  = av_frame_make_writable(ost->frame); 
    if (ret < 0){
        fprintf(stderr, "Error making frame writable: %s\n", av_err2str(ret));
        exit(1);
    }


	
    fill_yuv_image(ost->frame, ost->next_pts, c->width, c->height);

    ost->frame->pts = ost->next_pts++;

    return ost->frame;
}

/*
 * encode one video frame and send it to the muxer
 * return 1 when encoding is finished, 0 otherwise
 */
int write_video_frame(AVFormatContext *oc, OutputStream *ost)
{
    int ret;
    AVCodecContext *c;
    AVFrame *frame;
    int got_packet = 0;
    AVPacket pkt = { 0 };

    c = ost->enc;

// Generate Frame HERE!!
// Maybe Add OpenCV logic here!
//	capture << cvMat;
//	convert cvMat2AvFrame();
//

    frame = get_video_frame(ost);

    av_init_packet(&pkt);

    // encode the image; Handle Frame then Handle Packet

    
    ret = avcodec_send_frame(c, frame);
    if (ret < 0) {
        fprintf(stderr, "Error encoding video frame: %s\n", av_err2str(ret));
        exit(1);
    }
    got_packet = (avcodec_receive_packet(c, &pkt) == 0)? 1: 0;
    if (got_packet) {
        ret = write_frame(oc, &c->time_base, ost->st, &pkt);
    } else {
        ret = 0;
    }
    if (ret < 0) {
        fprintf(stderr, "Error while writing video frame: %s\n", av_err2str(ret));
        exit(1);
    }

    return (frame || got_packet ) ? 0 : 1;
}

void close_stream(AVFormatContext *oc, OutputStream *ost)
{
    avcodec_free_context(&ost->enc);
    av_frame_free(&ost->frame);
    av_frame_free(&ost->tmp_frame);
}

// media file output

int main(int argc, char **argv)
{
    OutputStream video_stream = { 0 };
    const char *filename;
    AVOutputFormat *fmt;
    AVFormatContext *oc;
    AVCodec *video_codec;
    int ret;
    int have_video = 0;
    int encode_video = 0; 
    int i;
    AVDictionary *opt = NULL;


    filename = "live.avi";

    // allocate the output media context
    avformat_alloc_output_context2(&oc, NULL, NULL, filename);
    if (!oc) {
        printf("Could not deduce output format from file extension: using MPEG.\n");
        avformat_alloc_output_context2(&oc, NULL, "mpeg", filename);
    }
    if (!oc)
        return 1;

    fmt = oc->oformat;

    /* 
     * Add the audio and video streams using the default format codecs
     * and initialize the codecs. 
     */
    if (fmt->video_codec != AV_CODEC_ID_NONE) {
        create_stream(&video_stream, oc, &video_codec, fmt->video_codec);
        have_video = 1;
        encode_video = 1;
    }

    /* 
     * Now that all the parameters are set, we can open the audio and
     * video codecs and allocate the necessary encode buffers.
     */
    if (have_video)
        open_video(oc, video_codec, &video_stream, opt);


    av_dump_format(oc, 0, filename, 1);

    // open the output file, if needed.
    if (!(fmt->flags & AVFMT_NOFILE)) {
        ret = avio_open(&oc->pb, filename, AVIO_FLAG_WRITE);
        if (ret < 0) {
            fprintf(stderr, "Could not open '%s': %s\n", filename,
                    av_err2str(ret));
            return 1;
        }
    }

    // Write the stream header, if any.
    ret = avformat_write_header(oc, &opt);
    if (ret < 0) {
        fprintf(stderr, "Error occurred when opening output file: %s\n",
                av_err2str(ret));
        return 1;
    }



    // LOOP TO CAPTURE FRAME
    while (encode_video) {
	 // RECORDING
        // select the stream to encode
        if (encode_video)
            encode_video = (write_video_frame(oc, &video_stream) == 0)? 1: 0;

	//could add audio here
	//if(encode_audio)
    }

    /* Write the trailer, if any. The trailer must be written before you
     * close the CodecContexts open when you wrote the header; otherwise
     * av_write_trailer() may try to use memory that was freed on
     * av_codec_close(). */
    av_write_trailer(oc);

    // Close each codec.
    if (have_video)
        close_stream(oc, &video_stream);

    if (!(fmt->flags & AVFMT_NOFILE))
        // Close the output file.
        avio_closep(&oc->pb);

    // free the stream 
    avformat_free_context(oc);

    return 0;
}
