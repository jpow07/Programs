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


class AV_Writer {

public:
	AV_Writer(const char *filename, const char* codecName, int width, int height, double framerate);
	~AV_Writer();

	void encodeFrame();
	void writeToStream();
	void allocateFrame();
	void createFrame();

private:
	int resolutionHeight;
	int resolutionWidth;
	int pixel_format;
	double videoFrameRate;

	AVCodecContext 	*context; // Stores codec information
	AVFrame		*image;	  // Stores uncompressed image
	AVPacket 	*packet;  // Stores compressed image
	AVStream	*stream;  // Stores interleaved stream
	AVFormatContext *formatContext; // Stores Digital Container Information
	AVOutputFormat	*outputFormat;  // Store file format information
	AVDictionary *codec_options;	//Store Codec specific Options

	bool isRecordingVideo;		// Track state of AV_Writer Video

	

	

};
