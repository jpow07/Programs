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
#include <libavformat/avformat.h>
}

#include "VideoEncoder.hpp"
//#include "AudioEncoder.hpp"


namespace libav {

class AVWriter {
public:
	AVWriter(const char *filename, int resHeight, int resWidth, std::string video_codec, 
			std::string audio_codec, bool hasVideo, bool hasAudio);
	~AVWriter();

	//	void writeFrame(AVFrame *frame);
	bool writeFrame(cv::Mat &mat);

private:
	AVOutputFormat *format;
	AVFormatContext *context;

	AVPacket *videoPacket;
	AVPacket *audioPacket;

	VideoEncoder *videoStream;
//	AudioEncoder *audioStream;

	bool hasVideo;
	bool hasAudio;

	void writePacket();

};


} //End Namespace

#endif 
