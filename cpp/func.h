#ifndef _FUNC_H
#define _FUNC_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

	#include <libavutil/avutil.h>
	#include <libavcodec/avcodec.h>
	#include <libavutil/channel_layout.h>
	#include <libavutil/opt.h>
	#include <libavutil/mathematics.h>
	#include <libavutil/timestamp.h>
	#include <libavformat/avformat.h>
	#include <libswscale/swscale.h>
	#include <libswresample/swresample.h>

/* 5 seconds stream duration */
#define STREAM_DURATION   10.0
#define STREAM_FRAME_RATE 25 /* 25 images/s */
#define STREAM_PIX_FMT    AV_PIX_FMT_YUV420P /* default pix_fmt */
#define SCALE_FLAGS SWS_BICUBIC


// a wrapper around a single output AVStream
typedef struct {
    AVStream *st;
    AVCodecContext *enc;

    /* pts of the next frame that will be generated */
    int64_t next_pts;
    int samples_count;

    AVFrame *frame;
    AVFrame *tmp_frame;

    float t, tincr, tincr2;

    struct SwsContext *sws_ctx;
    struct SwrContext *swr_ctx;
} OutputStream;

void log_packet(const AVFormatContext *fmt_ctx, const AVPacket *pkt);

int write_frame(AVFormatContext *fmt_ctx, const AVRational *time_base,
	      	 	AVStream *st, AVPacket *pkt);

/* Add an output stream. */
void add_stream(OutputStream *ost, AVFormatContext *oc, AVCodec **codec,
                       enum AVCodecID codec_id);

/* audio output */
AVFrame *alloc_audio_frame(enum AVSampleFormat sample_fmt, uint64_t channel_layout,
                                  int sample_rate, int nb_samples);

void open_audio(AVFormatContext *oc, AVCodec *codec, 
			OutputStream *ost, AVDictionary *opt_arg);

/* Prepare a 16 bit dummy audio frame of 'frame_size' samples and
 * 'nb_channels' channels. */
AVFrame *get_audio_frame(OutputStream *ost);

/*
 * encode one audio frame and send it to the muxer
 * return 1 when encoding is finished, 0 otherwise
 */
int write_audio_frame(AVFormatContext *oc, OutputStream *ost);

/* video output */
AVFrame *alloc_picture(enum AVPixelFormat pix_fmt, int width, int height);

 void open_video(AVFormatContext *oc, AVCodec *codec, OutputStream *ost,
	        AVDictionary *opt_arg);

/* Prepare a dummy image. */
void create_image(AVFrame *pict, int frame_index, int width, int height);

 AVFrame *get_video_frame(OutputStream *ost);

/*
 * encode one video frame and send it to the muxer
 * return 1 when encoding is finished, 0 otherwise
 */
int write_video_frame(AVFormatContext *oc, OutputStream *ost);
void close_stream(AVFormatContext *oc, OutputStream *ost);


#endif  
