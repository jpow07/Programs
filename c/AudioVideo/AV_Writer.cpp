#include "AV_Writer.hpp"


AV_Writer::AV_Writer(char *filename, char* codecName, int width, int height, double framerate) {

	this->context = NULL;
	AVCodec *codec;
	codec = avcodec_find_encoder_by_name(codecName);
	if (!codec) {
		fprintf(stderr, "Couldn't find codec: %s\n", codecName);	
		exit(1);
	}

	this->context = avcodec_alloc_context(codec);
	if (!(this->context)) {
		fprintf(stderr, "Couldn't allocate video codec context\n");	
		exit(1);
	}	

	this->context->codec = codec;
	this->context->width = width;
	this->context->height = height;
	this->context->time_base = (AVRational){1, framerate};
	this->context->framerate = (AVRational){framerate, 1};
	this->context->gop_size = 3;
	this->context->max_b_frames = 1;
	this->context->pix_fmt = AV_PIX_FMT_YUV420P;

	if (this->context->codec->id == AV_CODEC_ID_H264) {
		av_opt_set(this->context->codec->priv_data, "preset", "slow", 0);	
	}
	
	ret = avcodec_open2(this->context, this->context->codec, nullptr);
	if (ret < 0) {
		fprintf("Cound not open codec %s: %s\n", codecName, av_err2str(ret));
		exit(1);
	}
	
	avformat_alloc_output_context(this->fileContext, nullptr, nullptr, filename);
	if (!(this->fileContext)) {
		fprintf(stderr, "Could not open file: %s", filename);
		exit(1);
	}

	fprintf(stdout, "Initialized AV_Writer");
}

~AV_Writer() {

	avformat_write_trailer();
	avcodec_free_context(this->context);
	av_frame_free(this->image);

	fprintf(stdout, "Destroyed AV_Writer");

}

// Convert an uncompressed image into compressed packet
void AV_Writer::encodeFrame() {
	int ret;
	av_init_packet(this->packet);
	ret = avcodec_send_frame(this->image);
	if (ret < 0) {
		fprintf("Could not compress image into a packet\n"); 
		exit(1);
	}

	got_packet = !avcodec_receive_packet(this->context, this->packet);
	if(got_packet) 
		ret = writeToStream();		
	
}

// turn a compressed packet into an interleaved stream
void AV_Writer::writeToStream() {
	
	// rescale output packet timestamp values from codec to stream timebase
	av_packet_rescale_ts(this->packet, this->context->time_base, this->stream->time_base);
	this->packet->stream_index = this->stream->index;

	// Write the compressed frame to the media file.
	int ret = av_interleaved_write_frame(this->fileContext, this->packet);
	if (ret < 0)
		fprintf("Could not write compressed image into a stream\n"); 
}

void AV_Writer::createFrame() {

	this->image = av_frame_alloc();
	if (!this->image) {
		fprintf(stderr, "Could not allocate video frame\n");
		exit(1);
  	}

 	this->image->format = pix_fmt;
 	this->image->width  = width;
 	this->image->height = height;

    	// allocate the buffers for the frame data
    	int ret = av_frame_get_buffer(this->image, 32);
    	if (ret < 0) {
        	fprintf(stderr, "Could not allocate frame data.\n");
        	exit(1);
    	}

}

