#ifndef STREAM_H_
#define STREAM_H_

#include <libavutil/avassert.h>
#include <libavutil/channel_layout.h>
#include <libavutil/opt.h>
#include <libavutil/mathematics.h>
#include <libavutil/timestamp.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>

//#include "AudioCapture.h"
#include "AudioWriter.h"

#include "VideoWriter.h"
#include "VideoCapture.h"

#define STREAM_DURATION   10.0
#define STREAM_FRAME_RATE 25 /* 25 images/s */
#define STREAM_PIX_FMT    AV_PIX_FMT_YUV420P /* default pix_fmt */

#define SCALE_FLAGS SWS_BICUBIC

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
    struct SwrContext *swr_ctx;
} OutputStream;


void log_packet(const AVFormatContext *fmt_ctx, const AVPacket *pkt);
int write_frame(AVFormatContext *fmt_ctx, const AVRational *time_base, 
						AVStream *st, AVPacket *pkt);
void add_stream(OutputStream *ost, AVFormatContext *oc,
                       AVCodec **codec, enum AVCodecID codec_id);
void close_stream(AVFormatContext *oc, OutputStream *ost);

#endif
