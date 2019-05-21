#include "VideoWriter.hpp"

#define VIDEO_CODEC "libx264"
#define AUDIO_CODEC "wav"


libav::VideoWriter::VideoWriter(const char *filename, const char* codecName, int width, int height, double framerate) {

}

libav::VideoWriter::~VideoWriter() {

	av_write_trailer(this->formatContext);
	avcodec_free_context(&this->context);
	//av_frame_free(this->image);

	fprintf(stdout, "Destroyed VideoWriter\n");
}

// Convert an uncompressed image into compressed packet
void libav::VideoWriter::encodeFrame() {
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
void libav::VideoWriter::writeToStream() {
	
	// rescale output packet timestamp values from codec to stream timebase
	av_packet_rescale_ts(this->packet, this->context->time_base, this->stream->time_base);
	this->packet->stream_index = this->stream->index;

	// Write the compressed image to the media file.
	int ret = av_interleaved_write_frame(this->formatContext, this->packet);
	if (ret < 0)
		fprintf(stderr, "Could not write compressed image into a stream\n"); 
}

void libav::VideoWriter::createFrame() {

	this->image = av_frame_alloc();
	if (!this->image) {
		fprintf(stderr, "Could not allocate video image\n");
		exit(1);
  	}

 	this->image->format = this->context->pix_fmt;
 	this->image->width  = this->context->width;
 	this->image->height = this->context->height;

    	// allocate the buffers for the image data
    	int ret = av_frame_get_buffer(this->image, 32);
    	if (ret < 0) {
        	fprintf(stderr, "Could not allocate image data.\n");
        	exit(1);
    	}

}

