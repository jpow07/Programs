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
void terminal_image(cv::Mat mat);

int main(int argc, char* argv[]){

	//************************ MAT ***********************************
	cv::Mat image = cv::imread(argv[1], cv::IMREAD_UNCHANGED);
	cv::Mat rgb_image;
        cv::cvtColor(image, rgb_image, cv::COLOR_BGR2RGB);

	cv::Mat resized;
	cv::resize(rgb_image, resized, cv::Size(175, 75), 0, 0, cv::INTER_CUBIC);

	std::cout << "RGB Mat Type" << std::endl; 
	for (int i = 0; i < resized.size().height; i++) {

		for (int j = 0; j < resized.size().width; j++) {
				print_pixel(resized, i, j, RGB);
		}

		std::cout << std::endl;
	}

	cv::imshow("Regular Mat", image);
	cv::imshow("Converted Mat", rgb_image);
	cv::waitKey(0);

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


	//********************************************************************************************************//
	//******************************************* AVFRAME ****************************************************//
	//********************************************************************************************************//



	AVFrame *frame;

	frame = av_frame_alloc();
	frame->width = resized.size().width;
	frame->height = resized.size().height;
	frame->format = AV_PIX_FMT_RGB24;

	int number_of_pixels = frame->width * frame->height;
	int number_of_bytes = 3 * number_of_pixels;
	
	frame->data[0] = (uint8_t*) malloc(sizeof(uint8_t) * number_of_bytes);
	int number_of_colors = 3;		
	for(int i = 0; i < resized.size().height; i++ ) {
		for(int j = 0; j < resized.size().width; j++) {
			for(int k = 0; k < 3; k++) {
				cv::Vec3b pixel = resized.at<cv::Vec3b>(i, j); 
				int pixel_position = (i * resized.size().width) + j;
				frame->data[0][pixel_position * 3 + k] = pixel.val[k];
			}
		}	
	}	

	//PRINT PICTURE
	for(int i = 0; i < number_of_bytes; i += 3) {
	
		if (i % frame->width * 3 == 0)
			std::cout << std::endl;

		uint8_t red = frame->data[0][i];
		uint8_t green = frame->data[0][i + 1];
		uint8_t blue = frame->data[0][i + 2];
		std::cout << "\033[38;2;" << (int)red << ";" << (int)green << ";" << (int)blue << "m$";
	
	
	}
	std::cout << std::endl;
	return 0;	
}




void print_pixel(cv::Mat image, int row, int col, int type) { 
	cv::Vec3b pixel = image.at<cv::Vec3b>(row, col); 
	uchar red, green, blue;	

	if (type) { //BGR
		blue = pixel.val[0];
		green = pixel.val[1];
		red = pixel.val[2];
	} else { //RGB
		red = pixel.val[0];
		green = pixel.val[1];
		blue = pixel.val[2];
	}

	std::cout << "\033[38;2;" << int(red) << ";" << (int)green << ";" << (int)blue << "m|";
}



