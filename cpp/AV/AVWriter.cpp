#include "AVWriter.hpp"

libav::AVWriter::AVWriter(const char *filename, int resHeight, int resWidth,
std::string videoCodec, std::string audioCodec)
{
	AVCodec *videoCodec;
	int ret;
	int have_video = 0;
	int encode_video = 0;
	AVDictionary *opt = NULL;
	int i;


	avformat_alloc_output_context2(&this->context, NULL, NULL, filename);
	if (!this->context) {
		printf("Could not deduce output format from file extension: using MPEG.\n");
		avformat_alloc_output_context2(&this->context, NULL, "mpeg", filename);
	}

	if (!this->context)
	return 1;

	this->format = this->context->oformat;

	if (this->format->videoCodec != AV_CODEC_ID_NONE) {
		videoStream();
		open_stream(&video_st, this->context, &videoCodec, this->format->videoCodec, cap);
		have_video = 1;
		encode_video = 1;
	}

	if (hasVideo)
		//videoStream();

	if (hasAudio)
		//audioStream();

	av_dump_format(this->context, 0, filename, 1);

	if (!(this->format->flags & AVFMT_NOFILE)) {
		ret = avio_open(&this->context->pb, filename, AVIO_FLAG_WRITE);
		if (ret < 0) {
			fprintf(stderr, "Could not open %s\n", filename);
			return 1;
		}
	}

	ret = avformat_write_header(this->context, &opt);
	if (ret < 0) {
		fprintf(stderr, "Error occurred when opening output file\n");
		return 1;
	}

}

libav::AVWriter::~AVWriter()
{

	//AVWriter Close Stream
	av_write_trailer(this->context);
	if (!(this->format->flags & AVFMT_NOFILE))
		avio_closep(&this->context->pb);

	//AVWriter ~Destructor
	avformat_free_context(this->context);
}

