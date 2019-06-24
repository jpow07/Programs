#ifndef VIDEOCAPTURE_H
#define VIDEOCAPTURE_H
#include "Stream.h"

/* VideoCapture */
AVFrame *get_video_frame(struct OutputStream *ost);
void fill_yuv_image(AVFrame *pict, int frame_index, int width, int height);

#endif
