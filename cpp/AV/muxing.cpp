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


// a wrapper around a single output AVStream
typedef struct OutputStream {
	AVStream *st;
	AVCodecContext *enc;

	int64_t next_pts;

	AVFrame *frame;

} OutputStream;


int writeFrame(AVFormatContext *fmt_ctx, const AVRational *time_base, 
		AVStream *st, AVPacket *pkt) {

	av_packet_rescale_ts(pkt, *time_base, st->time_base);
	pkt->stream_index = st->index;
	return av_interleaved_write_frame(fmt_ctx, pkt);
}

void open_stream(OutputStream *ost, AVFormatContext *oc, AVCodec **codec,
		enum AVCodecID codec_id, cv::VideoCapture cap) {

	AVCodecContext *c;
	int i;

	if (USING_H264) {
		*codec = avcodec_find_encoder_by_name("libx264rgb");
	} else {
		*codec = avcodec_find_encoder(codec_id);
	}
	if (!(*codec)) {
		fprintf(stderr, "Could not find encoder for '%s'\n", avcodec_get_name(codec_id));
		exit(1);
	}


	ost->st = avformat_new_stream(oc, NULL);
	if (!ost->st) {
		fprintf(stderr, "Could not allocate stream\n");
		exit(1);
	}

	//ost->st->id = oc->nb_streams - 1;
	c = avcodec_alloc_context3(*codec);

	if (!c) {
		fprintf(stderr, "Could not alloc an encoding context\n");
		exit(1);
	}


	if (USING_H264) {
		c->codec_id = AV_CODEC_ID_H264;
		av_opt_set(c->priv_data, "preset", "veryfast", 0);	
	}

	c->bit_rate = 400000;
	c->width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
	c->height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
	ost->st->time_base = (AVRational){ 1, STREAM_FRAME_RATE };
	c->time_base = ost->st->time_base;
	c->gop_size = 12; 
	c->pix_fmt = STREAM_PIX_FMT;

	if (oc->oformat->flags & AVFMT_GLOBALHEADER)
		c->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

	ost->enc = c;
}

AVFrame *allocFrame(enum AVPixelFormat pix_fmt, int width, int height) {

	AVFrame *frame;
	int ret;

	frame = av_frame_alloc();
	if (!frame)
		return NULL;

	frame->format = pix_fmt;
	frame->width  = width;
	frame->height = height;

	ret = av_frame_get_buffer(frame, 32);
	if (ret < 0) {
		fprintf(stderr, "Could not allocate frame data.\n");
		exit(1);
	}

	return frame;
}

void open_video(AVFormatContext *oc, AVCodec *codec, OutputStream *ost, AVDictionary *opt_arg) {
	int ret;
	AVCodecContext *c = ost->enc;
	AVDictionary *opt = NULL;
	av_dict_copy(&opt, opt_arg, 0);
	ret = avcodec_open2(c, codec, &opt);
	av_dict_free(&opt);

	if (ret < 0) {
		fprintf(stderr, "Could not open video codec\n");
		exit(1);
	}

	ost->frame = allocFrame(c->pix_fmt, c->width, c->height);
	if (!ost->frame) {
		fprintf(stderr, "Could not allocate video frame\n");
		exit(1);
	}

	ret = avcodec_parameters_from_context(ost->st->codecpar, c);
	if (ret < 0) {
		fprintf(stderr, "Could not copy the stream parameters\n");
		exit(1);
	}
}

AVFrame *convertMatToFrame(OutputStream *ost, cv::Mat mat) {

	if (av_frame_make_writable(ost->frame) < 0)
		exit(1);

	int subpixel = 3; //number of pixels RGB = 3 RGBA = 4
	for(int i = 0; i < ost->enc->height ; i++ ) {
		for(int j = 0; j < ost->enc->width; j++) {
			for(int k = 0; k < subpixel; k++) {
				cv::Vec3b pixel = mat.at<cv::Vec3b>(i, j); 
				int pixelPos = (i * ost->enc->width) + j;
				ost->frame->data[0][pixelPos * subpixel + k] = (unsigned char) pixel.val[k];
			}
		}	
	}	

	ost->frame->pts = ost->next_pts++;
	return ost->frame ;
}

int encodeVideoFrame(AVFormatContext *oc, OutputStream *ost, cv::Mat mat) {
	int ret;
	AVCodecContext *context;
	AVFrame *frame;
	int got_packet = 0;
	AVPacket pkt = { 0 };
	context = ost->enc;

	frame = convertMatToFrame(ost, mat);
	av_init_packet(&pkt);

	ret = avcodec_send_frame(context, frame);
	if (ret < 0 && ret != AVERROR_EOF) {
		fprintf(stderr, "Error encoding video frame\n");
		exit(1);
	} 

	got_packet = !avcodec_receive_packet(context, &pkt);

	if (got_packet) {
		ret = writeFrame(oc, &context->time_base, ost->st, &pkt);
	} else {
		ret = 0;
	}

	if (ret < 0) {
		fprintf(stderr, "Error while writing video frame\n");
		exit(1);
	}

	return (frame || got_packet) ? 0 : 1;
}

void destructor(AVFormatContext *oc, OutputStream *ost)
{
	avcodec_free_context(&ost->enc);
	av_frame_free(&ost->frame);
}

int main(int argc, char **argv) {

	if (argc < 2) {
		printf("usage: %s [FILE]\n", argv[0]);
		return 1;
	}

	cv::VideoCapture cap;
	cv::Mat mat;

	int deviceID = 1;
	int apiID = cv::CAP_ANY;
	cap.open(deviceID + apiID); 
	if (!cap.isOpened()) { 
		std::cerr << "ERROR! Unable to open camera" << std::endl;
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

	avformat_alloc_output_context2(&oc, NULL, NULL, filename);
	if (!oc) {
		printf("Could not deduce output format from file extension: using MPEG.\n");
		avformat_alloc_output_context2(&oc, NULL, "mpeg", filename);
	}

	if (!oc)
		return 1;

	fmt = oc->oformat;

	if (fmt->video_codec != AV_CODEC_ID_NONE) {
		open_stream(&video_st, oc, &video_codec, fmt->video_codec, cap);
		have_video = 1;
		encode_video = 1;
	}

	if (have_video)
		open_video(oc, video_codec, &video_st, opt);
	//VideoEncoder Class
	//AudioEncoder Class

	av_dump_format(oc, 0, filename, 1);

	//AVWriterClass
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

	//AVWriter Capture Audio and Video
	while (encode_video) {
		cap >> mat;
		imshow("Live", mat);

		if(cv::waitKey(5) >= 0)
			break;

		//AVWriter::writeFrame()
		//VideoEncoder::encodeFrame()
		encode_video = !encodeVideoFrame(oc, &video_st, mat);
	}

	//AudioEncoder.SendFrames()



	//VideoEncoder Destructor
	if (have_video)
		destructor(oc, &video_st);

	//AVWriter Close Stream
	av_write_trailer(oc);
	if (!(fmt->flags & AVFMT_NOFILE))
		avio_closep(&oc->pb);

	//AVWriter ~Destructor
	avformat_free_context(oc);

	return 0;
}

