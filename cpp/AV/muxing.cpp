/*
 * 
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
#define __STDC_CONSTANT_MACROS

extern "C" {
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <libavutil/avassert.h>
#include <libavutil/channel_layout.h>
#include <libavutil/opt.h>
#include <libavutil/mathematics.h>
#include <libavutil/timestamp.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

//OpenCV Libraries
#include <opencv2/opencv.hpp> 
#include <opencv2/core.hpp> 
#include <opencv2/highgui.hpp> 
#include <opencv2/imgcodecs.hpp> 
#include <opencv2/imgproc.hpp> 

#define STREAM_FRAME_RATE 10 /* 25 images/s */
#define STREAM_PIX_FMT    AV_PIX_FMT_BGR24 /* default pix_fmt */

#define SCALE_FLAGS SWS_BICUBIC
#define VIDEO_CODEC "libx264rgb"

// a wrapper around a single output AVStream
typedef struct OutputStream {
    AVStream *st;
    AVCodecContext *enc;
    /* pts of the next frame that will be generated */
    int64_t next_pts;
    int samples_count;
    AVFrame *frame;
    AVFrame *tmp_frame;
    float t, tincr, tincr2;
    struct SwsContext *sws_ctx;
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
 void add_stream(OutputStream *ost, AVFormatContext *oc,
                       AVCodec **codec,
                       enum AVCodecID codec_id, cv::VideoCapture cap)
{
    AVCodecContext *c;
    int i;
    /* find the encoder */
    *codec = avcodec_find_encoder_by_name("libx264rgb");
    if (!(*codec)) {
        fprintf(stderr, "Could not find encoder for '%s'\n", avcodec_get_name(codec_id));
        exit(1);
    }
    ost->st = avformat_new_stream(oc, NULL);
    if (!ost->st) {
        fprintf(stderr, "Could not allocate stream\n");
        exit(1);
    }
    ost->st->id = oc->nb_streams-1;
    c = avcodec_alloc_context3(*codec);
    if (!c) {
        fprintf(stderr, "Could not alloc an encoding context\n");
        exit(1);
    }
    
    c->codec_id = AV_CODEC_ID_H264;
    c->bit_rate = 400000;
    /* Resolution must be a multiple of two. */
    c->width    = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    c->height   = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    /* timebase: This is the fundamental unit of time (in seconds) in terms
    * of which frame timestamps are represented. For fixed-fps content,
     * timebase should be 1/framerate and timestamp increments should be
     * identical to 1. */
    ost->st->time_base = (AVRational){ 1, STREAM_FRAME_RATE };
    c->time_base       = ost->st->time_base;
    c->gop_size      = 12; /* emit one intra frame every twelve frames at most */
    c->pix_fmt       = STREAM_PIX_FMT;
    if (c->codec_id == AV_CODEC_ID_MPEG2VIDEO) {
        /* just for testing, we also add B-frames */
        c->max_b_frames = 2;
    }
    if (c->codec_id == AV_CODEC_ID_MPEG1VIDEO) {
        /* Needed to avoid using macroblocks in which some coeffs overflow.
         * This does not happen with normal video, it just happens here as
         * the motion of the chroma plane does not match the luma plane. */
        c->mb_decision = 2;
    }

    /* Some formats want stream headers to be separate. */
    if (oc->oformat->flags & AVFMT_GLOBALHEADER)
        c->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    ost->enc = c;
}
/**************************************************************/
/* video output */
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
    /* allocate the buffers for the frame data */
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
    /* open the codec */
    ret = avcodec_open2(c, codec, &opt);
    av_dict_free(&opt);
    if (ret < 0) {
        fprintf(stderr, "Could not open video codec\n");
        exit(1);
    }
    /* allocate and init a re-usable frame */
    ost->frame = alloc_picture(c->pix_fmt, c->width, c->height);
    if (!ost->frame) {
        fprintf(stderr, "Could not allocate video frame\n");
        exit(1);
    }
    ost->tmp_frame = NULL;
    if (c->pix_fmt != STREAM_PIX_FMT) {
        ost->tmp_frame = alloc_picture(STREAM_PIX_FMT, c->width, c->height);
        if (!ost->tmp_frame) {
            fprintf(stderr, "Could not allocate temporary picture\n");
            exit(1);
        }
    }
    /* copy the stream parameters to the muxer */
    ret = avcodec_parameters_from_context(ost->st->codecpar, c);
    if (ret < 0) {
        fprintf(stderr, "Could not copy the stream parameters\n");
        exit(1);
    }
}

void cvMat2AVFrame(cv::Mat &mat, AVFrame *frame, int width, int height) {
	int subpixel = 3;
	for(int i = 0; i < height ; i++ ) {
		for(int j = 0; j < width; j++) {
			for(int k = 0; k < subpixel; k++) {
				cv::Vec3b pixel = mat.at<cv::Vec3b>(i, j); 
				int pixel_position = (i * mat.size().width) + j;
				frame->data[0][pixel_position * subpixel + k] = (unsigned char) pixel.val[k];

			}
		}	
	}	
}
/* Prepare a dummy image. */
 void create_RGB_frame(AVFrame *frame, int frame_index, int width, int height)
{
	unsigned char val = 255;
	//Convert a 3D array into a 1D array
	int subpixel = 3;
	for(int i = 0; i < height; i++ ) {
		for(int j = 0; j < width; j++) {
			for(int k = 0; k < subpixel; k++) {
				int pixel_position = (i * width) + j;
				if(k % subpixel == 1) { 
				frame->data[0][pixel_position * subpixel + k] = val;
				}

			}
		}	
	}	
/*
	int y, x;
	uint8_t *p = pict->data[0];
	for(y = 0; y < height; y++) {
    		for(x = 0; x < width; x++) {
        		*p++ = x % 255; // R
        		*p++ = 0; // G
        		*p++ = 0; // B
    		}
	}
*/
}
 AVFrame *get_video_frame(OutputStream *ost, cv::Mat mat)
{
    AVCodecContext *c = ost->enc;

    if (av_frame_make_writable(ost->frame) < 0)
        exit(1);
    if (c->pix_fmt != STREAM_PIX_FMT) {
        if (!ost->sws_ctx) {
            ost->sws_ctx = sws_getContext(c->width, c->height,
                                          STREAM_PIX_FMT,
                                          c->width, c->height,
                                          c->pix_fmt,
                                          SCALE_FLAGS, NULL, NULL, NULL);
            if (!ost->sws_ctx) {
                fprintf(stderr, "Could not initialize the conversion context\n");
                exit(1);
            }
        }
	//cvMat2AVFrame(mat, ost->tmp_frame);
        create_RGB_frame(ost->tmp_frame, ost->next_pts, c->width, c->height);
        sws_scale(ost->sws_ctx, (const uint8_t * const *) ost->tmp_frame->data,
                  ost->tmp_frame->linesize, 0, c->height, ost->frame->data,
                  ost->frame->linesize);
    } else {
	cvMat2AVFrame(mat, ost->frame, c->width, c->height);
        //create_RGB_frame(ost->frame, ost->next_pts, c->width, c->height);	
    }
    ost->frame->pts = ost->next_pts++;
	

    return ost->frame;
}
/*
 * encode one video frame and send it to the muxer
 * return 1 when encoding is finished, 0 otherwise
 */
 int write_video_frame(AVFormatContext *oc, OutputStream *ost, cv::Mat mat)
{
    int ret;
    AVCodecContext *context;
    AVFrame *frame;
    int got_packet = 0;
    AVPacket pkt = { 0 };
    context = ost->enc;
    

    frame = get_video_frame(ost, mat);

    av_init_packet(&pkt);
    /* encode the image */
    ret = avcodec_send_frame(context, frame);
    if (ret < 0 && ret != AVERROR_EOF) {
        fprintf(stderr, "Error encoding video frame\n");
        exit(1);
    } 

    got_packet = !avcodec_receive_packet(context, &pkt);

    if (got_packet) {
        ret = write_frame(oc, &context->time_base, ost->st, &pkt);
    } else {
        ret = 0;
    }
    if (ret < 0) {
        fprintf(stderr, "Error while writing video frame\n");
        exit(1);
    }
    return (frame || got_packet) ? 0 : 1;
}
 void close_stream(AVFormatContext *oc, OutputStream *ost)
{
    avcodec_free_context(&ost->enc);
    av_frame_free(&ost->frame);
    av_frame_free(&ost->tmp_frame);
    sws_freeContext(ost->sws_ctx);
}
/**************************************************************/
/* media file output */
int main(int argc, char **argv)
{

    if (argc < 2) {
        printf("usage: %s [FILE]\n", argv[0]);
        return 1;
    }

    cv::VideoCapture cap;
    cv::Mat mat;

    int deviceID = 0;
    int apiID = cv::CAP_ANY;
    cap.open(deviceID + apiID); 
    if (!cap.isOpened()) { 
	    std::cerr << "ERROR! Unable to open camera\n"; 
        return -1; 
    } 

    OutputStream video_st = { 0 };
    const char *filename;
    AVOutputFormat *fmt;
    AVFormatContext *oc;
    AVCodec *video_codec;
    int ret;
    int have_video = 0;
    int encode_video = 0;
    AVDictionary *opt = NULL;
    int i;


    filename = argv[1];
    for (i = 2; i+1 < argc; i+=2) {
        if (!strcmp(argv[i], "-flags") || !strcmp(argv[i], "-fflags"))
            av_dict_set(&opt, argv[i]+1, argv[i+1], 0);
    }

    avformat_alloc_output_context2(&oc, NULL, NULL, filename);
    if (!oc) {
        printf("Could not deduce output format from file extension: using MPEG.\n");
        avformat_alloc_output_context2(&oc, NULL, "mpeg", filename);
    }
    if (!oc)
        return 1;
    fmt = oc->oformat;

    if (fmt->video_codec != AV_CODEC_ID_NONE) {
        add_stream(&video_st, oc, &video_codec, fmt->video_codec, cap);
	std::cout << "Context Settings: \n"
		<< "Width: " << video_st.enc->width << "\n"
		<< "Height: " << video_st.enc->height << std::endl;
        have_video = 1;
        encode_video = 1;
    }

    if (have_video)
        open_video(oc, video_codec, &video_st, opt);

    av_dump_format(oc, 0, filename, 1);

    if (!(fmt->flags & AVFMT_NOFILE)) {
        ret = avio_open(&oc->pb, filename, AVIO_FLAG_WRITE);
        if (ret < 0) {
            fprintf(stderr, "Could not open %s\n", filename);
            return 1;
        }
    }

    ret = avformat_write_header(oc, &opt);
    if (ret < 0) {
        fprintf(stderr, "Error occurred when opening output file\n");
        return 1;
    }

    while (encode_video) {
	    cap >> mat;
	    imshow("Live", mat);
	    if(cv::waitKey(5) >= 0)
		    break;
            encode_video = !write_video_frame(oc, &video_st, mat);
    }

    av_write_trailer(oc);

    if (have_video)
        close_stream(oc, &video_st);

    if (!(fmt->flags & AVFMT_NOFILE))
        avio_closep(&oc->pb);

    avformat_free_context(oc);

    return 0;
}

