
#include <cinttypes>
#include <cstdio>

// ffmpeg C libraries
extern "C" {

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/opt.h>

}


class AV_Writer {

public:
	AV_Writer();
	~AV_Writer();

	void encodeFrame();
	void writeToStream();
	void convertFrame(cv::Mat mat);
	void allocateFrame();

private:
	AVCodecContext 	*context; // Stores codec information
	AVFrame		*frame;	  // Stores uncompressed image
	AVPacket 	*packet;  // Stores compressed image
	AVStream	*stream;  // Stores interleaved stream
	AVFormatContext *fileContext; // Stores Digital Container Information
	bool isRecordingVideo;		// Track state of AV_Writer Video
	bool isRecordingAudio;		// Track state of AV_Writer Audio

	

	

};
