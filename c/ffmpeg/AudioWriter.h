#ifndef AUDIOWRITER_H
#define AUDIOWRITER_H

#include "Stream.h"
#include "AudioCapture.h"

AVFrame *alloc_audio_frame(enum AVSampleFormat sample_fmt, uint64_t channel_layout,
                                  int sample_rate, int nb_samples);
void open_audio(AVFormatContext *oc, AVCodec *codec, struct OutputStream *ost, AVDictionary *opt_arg);
int write_audio_frame(AVFormatContext *oc, struct OutputStream *ost);

#endif
