#include "AVWriter.hpp"

libav::AVWriter::AVWriter(const char *filename, int resHeight, int resWidth, std::string video_codec, std::string audio_codec, bool hasVideo, bool hasAudio)
{
	AVCodec *videoCodec;
	int ret;
	int have_video = 0;
	int encode_video = 0;
	AVDictionary *opt = NULL;
	int i;

	avformat_alloc_context();
	if (!this->context) {
		printf("Could not deduce output format from file extension: using MPEG.\n");
		avformat_alloc_context();
	}

	if (!this->context)
		exit(1);

	this->format = this->context->oformat;

	if (hasVideo && this->format->video_codec != AV_CODEC_ID_NONE) {
		videoStream = new VideoEncoder(this->context, video_codec.c_str(), resHeight, resWidth);
//		this->format->video_codec = getCodec();
	}
/*
	if (hasAudio && this->format->audioCodec != AV_CODEC_ID_NONE){
		audioStream();
		this->format->audioCodec = getCodec();
	}
*/

	av_dump_format(this->context, 0, filename, 1);

	if (!(this->format->flags & AVFMT_NOFILE)) {
		ret = avio_open(&this->context->pb, filename, AVIO_FLAG_WRITE);
		if (ret < 0) {
			fprintf(stderr, "Could not open %s\n", filename);
			exit(1);
		}
	}

	ret = avformat_write_header(this->context, &opt);
	if (ret < 0) {
		fprintf(stderr, "Error occurred when opening output file\n");
		exit(1);
	}

	std::cout << "\033[38;2;0;255;0mAVWriter Initialized\033[39m" << std::endl;
}

libav::AVWriter::~AVWriter()
{

	//AVWriter Close Stream
	av_write_trailer(this->context);
	if (!(this->format->flags & AVFMT_NOFILE))
		avio_closep(&this->context->pb);

	//AVWriter ~Destructor
	avformat_free_context(this->context);
	free(this->videoStream);
}

bool libav::AVWriter::writeFrame(cv::Mat &mat)
{
	
	bool success = this->videoStream->encodeFrame(this->context, mat);

	return (success)? true : false;

}


