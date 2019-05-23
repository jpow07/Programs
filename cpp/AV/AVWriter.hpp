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

#ifndef AVWRITER_H_
#define AVWRITER_H_


#define __STDC_CONSTANT_MACROS
extern "C" {
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libavutil/opt.h>
#include <libavutil/mathematics.h>
#include <libavutil/timestamp.h>
#include <libavformat/avformat.h>
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
#define USING_H264 1

namespace libav {

class AVWriter {
public:
	AVWriter();
	~AVWriter();

	bool writeFrame(AVFrame *frame);
	bool writeFrame(cv::Mat &mat);

private:
	AVOutputFormat *format;
	AVFormatContext *context;

	AVPacket *videoPacket;
	AVPacket *audioPacket;

	//AVCodec *videoCodec;
	//AVCodec *audioCodec;

	VideoEncoder videoStream;
	AudioEncoder audioStream;

	bool hasVideo;
	bool hasAudio;

	void writePacket();

};


} //End Namespace

#endif 
