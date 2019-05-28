#include "AudioEncoder.hpp"

libav::AudioEncoder::AudioEncoder()
{


}

libav::AudioEncoder::~AudioEncoder()
{
	avcodec_free_context(this->context);
	av_frame_free(this->frame);
}
libav::AudioEncoder::encodeFrame()
{

}

