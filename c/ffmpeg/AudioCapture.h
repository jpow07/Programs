#ifndef AUDIOCAPTURE_H
#define AUDIOCAPTURE_H

#include "Stream.h"

AVFrame *get_audio_frame(struct OutputStream *ost);

#endif
