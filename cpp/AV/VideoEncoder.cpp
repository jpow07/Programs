#include "VideoEncoder.hpp" 

libav::VideoEncoder::VideoEncoder()
{

	AVCodecContext *c;
	int i, ret;

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

	ost->enc = c;
}

libav::VideoEncoder::~VideoEncoder()
{
	avcodec_free_context(this->context);
	av_frame_free(this->frame);
}

void libav::VideoEncoder::allocateFrame()
{
	int ret;

	this->frame = av_frame_alloc();
	if (!this->frame)
		return NULL;

	this->frame->format = pix_fmt;
	this->frame->width  = width;
	this->frame->height = height;

	ret = av_frame_get_buffer(this->frame, 32);
	if (ret < 0) {
		fprintf(stderr, "Could not allocate frame data.\n");
		exit(1);
	}

}

AVPacket* libav::VideoEncoder::encodeFrame()
{

	int ret;
	AVCodecContext *context;
	AVFrame *frame;
	int got_packet = 0;
	AVPacket pkt = { 0 };
	context = ost->enc;

	//frame = convertMatToFrame(ost, mat);
	av_init_packet(&pkt);

	ret = avcodec_send_frame(context, frame);
	if (ret < 0 && ret != AVERROR_EOF) {
		fprintf(stderr, "Error encoding video frame\n");
		exit(1);


	got_packet = !avcodec_receive_packet(context, &pkt);

	//RETURN HERE TO SEND PACKET BACK TO AVWRITER (AVWRITER)
	if (got_packet) {
		return this->packet
	} else {
		return NULL; 
	}

}

AVPacket* libav::VideoEncoder::encodeFrame(cv::Mat &mat)
{

	int ret;
	AVCodecContext *context;
	AVFrame *frame;
	int got_packet = 0;
	AVPacket pkt = { 0 };
	context = ost->enc;

	if (av_frame_make_writable(this->frame) < 0)
		exit(1);

	int subpixel = 3; //number of pixels RGB = 3 RGBA = 4
	for(int i = 0; i < this->context->height ; i++ ) {
		for(int j = 0; j < this->encoder->width; j++) {
			for(int k = 0; k < subpixel; k++) {
				cv::Vec3b pixel = mat.at<cv::Vec3b>(i, j); 
				int pixelPos = (i * this->context->width) + j;
				this->frame->data[0][pixelPos * subpixel + k] = (unsigned char) pixel.val[k];
			}
		}	
	}	

	this->frame->pts = this->next_pts++;

	av_init_packet(&pkt);

	ret = avcodec_send_frame(context, frame);
	if (ret < 0 && ret != AVERROR_EOF) {
		fprintf(stderr, "Error encoding video frame\n");
		exit(1);


	got_packet = !avcodec_receive_packet(context, &pkt);

	//RETURN HERE TO SEND PACKET BACK TO AVWRITER (AVWRITER)
	if (got_packet) {
		return this->packet
	} else {
		return NULL; 
	}

}


