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
		VideoWriter(const char *filename);
		~VideoWriter();


	private:




};

}
