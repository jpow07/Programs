#ifndef VIDEOWRITER_H
#define VIDEOWRITER_H
#include "Stream.h"

AVFrame *alloc_picture(enum AVPixelFormat pix_fmt, int width, int height);
void open_video(AVFormatContext *oc, AVCodec *codec, struct OutputStream *ost, AVDictionary *opt_arg);
int write_video_frame(AVFormatContext *oc, struct OutputStream *ost);

#endif
