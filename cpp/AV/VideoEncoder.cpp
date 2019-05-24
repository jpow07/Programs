#include "VideoEncoder.hpp" 

libav::VideoEncoder::VideoEncoder(AVFormatContext *formatContext, const char *codec_name,
	       	int height, int width)
{

	AVCodec *codec;
	int i, ret;

	if (USING_H264)
		codec = avcodec_find_encoder_by_name(codec_name);

	if (!codec) {
		fprintf(stderr, "Couldn't find encoder: '%s'\n", codec_name);
		exit(1);
	}
	std::cout << "Found Encoder" << std::endl;
	
	//Output Context
	this->stream = avformat_new_stream(formatContext, NULL);
	if (!this->stream) {
		fprintf(stderr, "Could not allocate stream\n");
		exit(1);
	}

	std::cout << "Allocated Stream" << std::endl;

	this->context = avcodec_alloc_context3(codec);
	if (!this->context) {
		fprintf(stderr, "Could not alloc an encoding context\n");
		exit(1);
	}
	std::cout << "Allocated Context" << std::endl;

	this->context->codec = codec;

	if (USING_H264) {
		this->context->codec_id = AV_CODEC_ID_H264;
		av_opt_set(this->context->priv_data, "preset", "veryfast", 0);	
	}

	this->context->bit_rate = 400000;
	this->context->width = width;
	this->context->height = height;
	this->stream->time_base = (AVRational){ 1, STREAM_FRAME_RATE };
	this->context->time_base = this->stream->time_base;
	this->context->gop_size = 12; 
	this->context->pix_fmt = STREAM_PIX_FMT;

	if (formatContext->oformat->flags & AVFMT_GLOBALHEADER)
		this->context->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;


	ret = avcodec_open2(this->context, this->context->codec, NULL);
	if (ret < 0) {
		fprintf(stderr, "Could not open video codec\n");
		exit(1);
	}
	std::cout << "Opened Video Codec" << std::endl;


	// Allocate Frame
	this->frame = av_frame_alloc();
	if(!this->frame) {
		fprintf(stderr, "Could not allocate frame.\n");
		exit(1);
	}

	this->frame->format = STREAM_PIX_FMT;
	this->frame->width  = width;
	this->frame->height = height;

	std::cout << "Frame HWF are set" << std::endl;

	ret = av_frame_get_buffer(this->frame, 32);
	if (ret < 0) {
		fprintf(stderr, "Could not allocate frame data.\n");
		exit(1);
	}
	std::cout << "Allocated Frame Data" << std::endl;

	if (!this->frame) {
		fprintf(stderr, "Could not allocate video frame\n");
		exit(1);
	}

	// Allocate something	
	ret = avcodec_parameters_from_context(this->stream->codecpar, this->context);
	if (ret < 0) {
		fprintf(stderr, "Could not copy the stream parameters\n");
		exit(1);
	}

}

libav::VideoEncoder::~VideoEncoder()
{
	avcodec_free_context(&this->context);
	av_frame_free(&this->frame);
}

void libav::VideoEncoder::encodeFrame()
{

	int got_packet = 0;
	av_init_packet(this->packet);

	int ret = avcodec_send_frame(context, frame);
	if (ret < 0 && ret != AVERROR_EOF) {
		fprintf(stderr, "Error encoding video frame\n");
		exit(1);
	}

	got_packet = !avcodec_receive_packet(this->context, this->packet);

}

void libav::VideoEncoder::encodeFrame(cv::Mat &mat)
{

	int got_packet = 0;

	if (av_frame_make_writable(this->frame) < 0)
		exit(1);

	int subpixel = 3; //number of pixels RGB = 3 RGBA = 4
	for(int i = 0; i < this->context->height ; i++ ) {
		for(int j = 0; j < this->context->width; j++) {
			for(int k = 0; k < subpixel; k++) {
				cv::Vec3b pixel = mat.at<cv::Vec3b>(i, j); 
				int pixelPos = (i * this->context->width) + j;
				this->frame->data[0][pixelPos * subpixel + k] = (unsigned char) pixel.val[k];
			}
		}	
	}	
	this->frame->pts = this->next_pts++;

	av_init_packet(this->packet);

	int ret = avcodec_send_frame(context, this->frame);
	if (ret < 0 && ret != AVERROR_EOF) {
		fprintf(stderr, "Error encoding video frame\n");
		exit(1);
	}

	got_packet = !avcodec_receive_packet(context, this->packet);
}

