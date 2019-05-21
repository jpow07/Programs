#include <iostream>
#include <cinttypes>
#include <cstdio>

// ffmpeg C libraries
extern "C" {

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/error.h>
#include <libavutil/opt.h>

}

namespace libav {

class VideoWriter {

public:
	VideoWriter(const char *filename, const char* codecName, int width, int height, double framerate);
	~VideoWriter();

	void initializeCodecContext();
	void encodeFrame();
	void writeToStream();
	void allocateFrame();
	void createFrame();

private:

	int resolutionHeight; 	// Resolution Height
	int resolutionWidth;	// Resolution Width
	int pixel_format;	// Format of Pixels (RGB24, BGR24, Grayscale, YUV420P)
	int gop_size;		// Number of frames captured in a group with I-Frame
	int max_b_frames;	// Number of B-Frames in a GOP Size

	AVRational time_base; 	// Time base
	AVRational framerate;	// Frame Rate

	AVCodecContext 	*context; 	// Stores codec information
	AVFrame		*image;	  	// Stores uncompressed image
	AVPacket 	*packet;  	// Stores compressed image
	AVStream	*stream;  	// Stores interleaved stream
	AVFormatContext *formatContext; // Stores Digital Container Information
	AVOutputFormat	*outputFormat;  // Store file format information
	AVDictionary 	*codec_options;	//Store Codec specific Options

	bool isRecordingVideo;		// Track state of AV_Writer Video

};

}
