#include <iostream>


//OpenCV Libraries
#include <opencv2/opencv.hpp> 
#include <opencv2/core.hpp> 
#include <opencv2/highgui.hpp> 
#include <opencv2/imgcodecs.hpp> 
#include <opencv2/imgproc.hpp> 


#include "AVWriter.hpp"

#define STREAM_FRAME_RATE 10 /* 25 images/s */
#define STREAM_PIX_FMT    AV_PIX_FMT_BGR24 /* default pix_fmt */

#define SCALE_FLAGS SWS_BICUBIC
#define VIDEO_CODEC "libx264rgb"
#define USING_H264 1


int main(int argc, const char *argv[]) {

	if (argc < 2) {
		printf("usage: %s [FILE]\n", argv[0]);
		return 1;
	} 

	const char *filename = argv[1];
	
	cv::VideoCapture cap;
	cv::Mat mat;

	int deviceID = 1;
	int apiID = cv::CAP_ANY;
	cap.open(deviceID + apiID); 
	if (!cap.isOpened()) { 
		std::cerr << "ERROR! Unable to open camera" << std::endl;
		return -1; 
	}


	libav::AVWriter mWriter(filename, cap.get(cv::CAP_PROP_FRAME_HEIGHT), 
				cap.get(cv::CAP_PROP_FRAME_WIDTH), VIDEO_CODEC, "wav", true, false);

	bool encode_video = true;
	while (encode_video) {
		cap >> mat;
		imshow("Live", mat);

		if(cv::waitKey(5) >= 0)
			encode_video = false;
			
		//mWriter << mat;
		bool success = false;
		success = mWriter.writeFrame(mat);
		if(success)
			std::cout << "\033[38;2;0;255;0mSuccesful Frame Write\033[39m" << std::endl;
		else
			std::cout << "\033[38;2;255;0;0mUnsuccessful Frame Write\033[39m" << std::endl;
		
	}


	return 0;
}
