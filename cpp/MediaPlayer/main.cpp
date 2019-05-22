#include <iostream>
#include <stdio.h>
#include <vector>
#include <cassert> 
#include <cstdio> 

extern "C"{

//FFMPEG Libs
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavfilter/avfilter.h>
#include <libavformat/avformat.h>
#include <libavresample/avresample.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
#include <libswresample/swresample.h>
#include <libswscale/swscale.h>

}

//OpenCV Libs
#include <opencv2/opencv.hpp> 
#include <opencv2/core.hpp> 
#include <opencv2/highgui.hpp> 
#include <opencv2/imgcodecs.hpp> 
#include <opencv2/imgproc.hpp> 

#define RGB 0
#define BGR 1

typedef unsigned char uchar;

void print_pixel(cv::Mat image, int row, int col,int type);
static void encode(AVCodecContext *enc_ctx, AVFrame *frame, AVPacket *pkt, FILE *outfile);
void terminal_image(cv::Mat mat);

static void convertMat2Frame(cv::Mat &mat, AVFrame *frame);

int main(int argc, char* argv[]){

	//************************ MAT ***********************************
	cv::Mat mat = cv::imread(argv[1], cv::IMREAD_UNCHANGED);


	const char *filename;
	const AVCodec *codec;
	AVCodecContext *c = NULL;

	FILE *file;

	AVPacket *packet;
	uint8_t endcode[] = {0, 0, 1, 0xb7};

	filename = argv[2];
	std::string codec_name = "libx264rgb";
	codec = avcodec_find_encoder_by_name(codec_name.c_str());
	if(!codec){
		printf("Codec not found\n");
		return 1;
	}

	c = avcodec_alloc_context3(codec);
	if(!c){
		printf("Error: Could not allocate video codec context\n");
		return 1;
	}

	packet = av_packet_alloc();
	if(!packet){
		return 1;
	}

	//Init Codec Context
	c->bit_rate = 4000;
	c->width = mat.size().width;
	c->height = mat.size().height;
	c->time_base = (AVRational){1, 25};
	c->framerate = (AVRational){25, 1};
	c->gop_size = 10;
	c->max_b_frames = 1;
	c->pix_fmt = AV_PIX_FMT_RGB24;
	
	//Init Frame
	AVFrame *frame;
	frame = av_frame_alloc();

	int number_of_pixels = mat.size().width * mat.size().height;
	int number_of_bytes = 3 * number_of_pixels;
	
	//init frame data on heap
	frame->data[0] = (uint8_t*) malloc(sizeof(uint8_t) * number_of_bytes);

	frame->width = c->width;
	frame->height = c->height;
	frame->format = c->pix_fmt; 
	frame->pts = 0;
	frame->linesize[0] = frame->width * 3;

	std::cout << "AVFrame Resolution: " << frame->width << "x" << frame->height << "\n" 
		<< "LineSize: " << frame->linesize[0] << "\n" 
		<< "Format: " << frame->format << std::endl;


	convertMat2Frame(mat, frame); 

	int ret = avcodec_open2(c, codec, NULL);
	if(ret < 0) {
		printf("Could not allocate the video frame data\n");
		return 1;
	}	

	file = fopen(filename, "wb");
	if (!file) {
		printf("Failed to open file\n");
		return 1;	
	}


	ret = av_frame_get_buffer(frame, 32);
	encode(c, frame, packet, file);	
	encode(c, NULL, packet, file);


	fwrite(endcode, 1, sizeof(endcode), file);
	fclose(file);
	avcodec_free_context(&c);
	av_frame_free(&frame);
	av_packet_free(&packet);	


	return 0;	
}

static void encode(AVCodecContext *enc_ctx, AVFrame *frame, AVPacket *pkt, FILE *outfile) {
	int ret;
   
       	/* send the frame to the encoder */
       	if (frame)
        	printf("Send frame %3" PRId64"\n", frame->pts);
   
 	ret = avcodec_send_frame(enc_ctx, frame);
	if (ret < 0) {
        	fprintf(stderr, "Error sending a frame for encoding\n");
		exit(1);
       	}
   
       	while (ret >= 0) {
        	ret = avcodec_receive_packet(enc_ctx, pkt);
        	if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
        		return;
        	} else if (ret < 0) {
        		fprintf(stderr, "Error during encoding\n");
              		exit(1);
        	}
   
        	printf("Write packet %3" PRId64" (size=%5d)\n", pkt->pts, pkt->size);
        	fwrite(pkt->data, 1, pkt->size, outfile);
        	av_packet_unref(pkt);
	}
}
/*Dummy image will be opencv to AVFrame conversion*/
/* Prepare a dummy image. */
static void convertMat2Frame(cv::Mat &mat, AVFrame *frame) {

	//int number_of_colors = 3;		
	for(int i = 0; i < mat.size().height; i++ ) {
		for(int j = 0; j < mat.size().width; j++) {
			for(int k = 0; k < 3; k++) {
				cv::Vec3b pixel = mat.at<cv::Vec3b>(i, j); 
				int pixel_position = (i * mat.size().width) + j;
				frame->data[0][pixel_position * 3 + k] = (uint8_t) pixel.val[k];
			}
		}	
	}	
}




















// Saves to 3 channels (could be converted to YUV if needed)


	//*****************************AVFrame**************************	

/*
	AVFrame *frame;

	frame = av_frame_alloc();
	frame->width = resized.size().width;
	frame->height = resized.size().height;
	frame->format = AV_PIX_FMT_RGB24;
	int number_of_pixels = frame->width * frame->height;

	frame->data[0] = (uint8_t*)malloc(sizeof(uint8_t) * number_of_pixels);
	frame->data[1] = (uint8_t*)malloc(sizeof(uint8_t) * number_of_pixels);
	frame->data[2] = (uint8_t*)malloc(sizeof(uint8_t) * number_of_pixels);


	for (int i = 0; i < resized.size().height; i++) {

		for (int j = 0; j < resized.size().width; j++) {
			cv::Vec3b pixel = resized.at<cv::Vec3b>(i, j); 

			unsigned int byte_location = (i * frame->width) + j;
			frame->data[0][byte_location] = pixel.val[0];
			frame->data[1][byte_location] = pixel.val[1];
			frame->data[2][byte_location] = pixel.val[2];

			int red =   frame->data[0][byte_location]; 	// Red
			int green = frame->data[1][byte_location]; 	// Green
			int blue =  frame->data[2][byte_location]; 	// Blue
	
			std::cout << "\033[38;2;" << (int)red << ";" << (int)green << ";" << (int)blue << "m/";
		}

		std::cout << std::endl;
	}

	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	cv::waitKey(0);
	
	for (int i = 0; i < frame->height; i++) {
		for (int j = 0; j < frame->width; j++) {
			unsigned int byte_location = (i * frame->width) + j;
			uint8_t red =   frame->data[0][byte_location]; 	// Red
			uint8_t green = frame->data[1][byte_location]; 	// Green
			uint8_t blue =  frame->data[2][byte_location]; 	// Blue
	
			std::cout << "\033[38;2;" << (int)red << ";" << (int)green << ";" << (int)blue << "m$";
		}

		std::cout << std::endl;
	}
	std::cout << frame->width << "x" << frame->height << std::endl; 

*/
