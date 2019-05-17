#include "AV_Writer.hpp"

AV_Writer::AV_Writer(const char *filename, const char* codecName, int width, int height, double framerate) {

	int ret;
	this->context = NULL;
	this->codec_options = NULL;
	
	AVCodec *codec;
	std::cout << "Finding encoder by name" << std::endl;
	codec = avcodec_find_encoder_by_name(codecName);
	if (!codec) {
		fprintf(stderr, "Couldn't find codec: %s\n", codecName);	
		exit(1);
	}

	std::cout << "Allocating video codec context" << std::endl;
	this->context = avcodec_alloc_context3(codec);
	if (!(this->context)) {
		fprintf(stderr, "Couldn't allocate video codec context\n");	
		exit(1);
	}	

	fprintf(stdout, "Initializing Context Variables\n");

	this->context->width = width;
	this->context->height = height;
	this->context->time_base = (AVRational){1, 30};
	this->context->framerate = (AVRational){30 , 1};
	this->context->gop_size = 3;
	this->context->max_b_frames = 1;
	this->context->pix_fmt = AV_PIX_FMT_YUV420P;
	this->context->codec = codec;


	if (this->context->codec->id == AV_CODEC_ID_H264) {
		av_opt_set(this->codec_options, "preset", "veryfast", 0);	
	}
	fprintf(stdout, "Setting H264 Options\n");

	fprintf(stdout, "Allocating FileContext, Opening new file to write\n");
	avformat_alloc_output_context2(&this->formatContext, nullptr, nullptr, filename);
	if (!(this->formatContext)) {
		fprintf(stderr, "Could not open file: %s", filename);
		exit(1);
	}

	ret = avcodec_open2(this->context, this->context->codec, &this->codec_options);
	if (ret < 0) {
//		fprintf(stderr, "Could not open codec %s: %s\n", codecName, av_err2str(ret));
		exit(1);
	}
	if (!(this->outputFormat->flags & AVFMT_NOFILE)) {
		ret = avio_open(&this->formatContext->pb, filename, AVIO_FLAG_WRITE);
		if (ret < 0) {
			fprintf(stderr, "Couldn't open '%s\n", filename);
            		exit(1);
		}
	}
	fprintf(stdout, "Writing Header File\n");
	ret = avformat_write_header(this->formatContext, &this->codec_options);
	if(ret < 0) {
		fprintf(stderr, "Couldn't write file header\n");
		exit(1);

	}

	
	fprintf(stdout, "Initialized AV_Writer\n");
}

AV_Writer::~AV_Writer() {

	av_write_trailer(this->formatContext);
	avcodec_free_context(&this->context);
	//av_frame_free(this->image);

	fprintf(stdout, "Destroyed AV_Writer\n");
}

// Convert an uncompressed image into compressed packet
void AV_Writer::encodeFrame() {
	int ret;
	av_init_packet(this->packet);
	ret = avcodec_send_frame(this->context, this->image);
	if (ret < 0) {
		fprintf(stderr, "Could not compress image into a packet\n"); 
		exit(1);
	}

	int got_packet = !avcodec_receive_packet(this->context, this->packet);
	if(got_packet) 
		writeToStream();		
	
}

// turn a compressed packet into an interleaved stream
void AV_Writer::writeToStream() {
	
	// rescale output packet timestamp values from codec to stream timebase
	av_packet_rescale_ts(this->packet, this->context->time_base, this->stream->time_base);
	this->packet->stream_index = this->stream->index;

	// Write the compressed frame to the media file.
	int ret = av_interleaved_write_frame(this->formatContext, this->packet);
	if (ret < 0)
		fprintf(stderr, "Could not write compressed image into a stream\n"); 
}

void AV_Writer::createFrame() {

	this->image = av_frame_alloc();
	if (!this->image) {
		fprintf(stderr, "Could not allocate video frame\n");
		exit(1);
  	}

 	this->image->format = this->context->pix_fmt;
 	this->image->width  = this->context->width;
 	this->image->height = this->context->height;

    	// allocate the buffers for the frame data
    	int ret = av_frame_get_buffer(this->image, 32);
    	if (ret < 0) {
        	fprintf(stderr, "Could not allocate frame data.\n");
        	exit(1);
    	}

}

