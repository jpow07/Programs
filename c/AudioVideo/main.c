/*
 * 
 *
 *
 */


#include <stdio.h>
#include <inttypes.h>

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/opt.h>


typedef struct Stream{
	AVStream *stream;
	AVCodecContext *context;
	AVFormatContext *format;
	AVFrame *frame;
	AVPacket *packet;
	FILE *file;

} Stream;

//Mocking Functions (used to generate audio and video frames)
void makeVideoFrame(AVFrame *videoFrame, int i);
AVFrame *makeAudioFrame();

static void encode(AVCodecContext *enc_ctx, AVFrame *frame, AVPacket *pkt, FILE *outfile)
{
    int ret;
    // send the frame to the encoder
    if (frame)
        printf("Send frame %3"PRId64" ", frame->pts);
	
    ret = avcodec_send_frame(enc_ctx, frame);
    if (ret < 0) {
        fprintf(stderr, "Error sending a frame for encoding\n");
        exit(1);
    }

    ret = avcodec_receive_packet(enc_ctx, pkt);
    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
    	return;
    else if (ret < 0) {
        fprintf(stderr, "Error during encoding\n");
        exit(1);
    }
    printf("Write packet %3"PRId64" (size=%5d)\n", pkt->pts, pkt->size);
    fwrite(pkt->data, 1, pkt->size, outfile);
    av_packet_unref(pkt);
}

int main(int argc, const char* argv[]){

    const char *filename, *codec_name;
    Stream videoStream = {0};

    AVCodecContext *c= NULL;
    AVFormatContext *oc;
    int i, ret, x, y;
    FILE *f;
    AVFrame *frame;
    AVPacket *pkt;
    _Bool isRecordingVideo = 0;
    _Bool isRecordingAudio = 0;
    
    uint8_t endcode[] = { 0, 0, 1, 0xb7 };
    if (argc <= 2) {
        fprintf(stderr, "Usage: %s <output file> <codec name>\n", argv[0]);
        exit(0);
    }
    filename = argv[1];
    codec_name = argv[2];

	AVCodec *tmpCodec;

    	tmpCodec = avcodec_find_encoder_by_name(codec_name);
    	if (!tmpCodec) {
        	fprintf(stderr, "Codec '%s' not found\n", codec_name);
        	exit(1);
    	}

    c = avcodec_alloc_context3(tmpCodec);
    c->codec = tmpCodec; 
    if (!c) {
        fprintf(stderr, "Could not allocate video codec context\n");
        exit(1);
    }
    pkt = av_packet_alloc();
    if (!pkt)
        exit(1);
    /* put sample parameters */
    c->bit_rate = 40000;
    /* resolution must be a multiple of two */
    c->width = 352;// 1280;
    c->height = 288;//720;
    /* frames per second */
    c->time_base = (AVRational){1, 25};
    c->framerate = (AVRational){25, 1};

    /* gop_size is the number of frames grouped together, 3(,15, 100, etc.), and 
     * every 3(15, 100, etc) frames will be an intra-frame (I-Frame). 
     * max_b_frames Frames states how many B-frames will be in the gop group.
     *  The remaining images will be P-frames
     *  I-Frames are key frames
     *  P-Frames are compressed based on last frame
     *  B-Frames are compressed based of last frame and next frame
     */
    c->gop_size = 10;
    c->max_b_frames = 3;

    c->pix_fmt = AV_PIX_FMT_YUV420P;
    if (c->codec->id == AV_CODEC_ID_H264)
        av_opt_set(c->priv_data, "preset", "slow", 0);
    /* open it */
    ret = avcodec_open2(c, c->codec, NULL);
    if (ret < 0) {
        fprintf(stderr, "Could not open codec: %s\n", av_err2str(ret));
        exit(1);
    }
    f = fopen(filename, "wb");
    if (!f) {
        fprintf(stderr, "Could not open %s\n", filename);
        exit(1);
    }
    frame = av_frame_alloc();
    if (!frame) {
        fprintf(stderr, "Could not allocate video frame\n");
        exit(1);
    }
    frame->format = c->pix_fmt;
    frame->width  = c->width;
    frame->height = c->height;
    ret = av_frame_get_buffer(frame, 32);
    if (ret < 0) {
        fprintf(stderr, "Could not allocate the video frame data\n");
        exit(1);
    }

    /* allocate the output media context */
    avformat_alloc_output_context2(&oc, NULL, NULL, filename);
    if (!oc) {
        printf("Could not deduce output format from file extension: using MPEG.\n");
        avformat_alloc_output_context2(&oc, NULL, "mpeg", filename);
    }
    if (!oc)
        return 1;

    /* encode 1 second of video */
	printf("break");
    for (i = 0; i < (25 * 60); i++) {
        fflush(stdout);
        /* make sure the frame data is writable */
        int ret = av_frame_make_writable(frame);
        if (ret < 0)
            exit(1);
	makeVideoFrame(frame, i);
        /* encode the image */
        encode(c, frame, pkt, f);
    }
    printf("NULL Frame\n");
    /* flush the encoder */
    encode(c, NULL, pkt, f);
    /* add sequence end code to have a real MPEG file */
    fwrite(endcode, 1, sizeof(endcode), f);
    fclose(f);
    avcodec_free_context(&c);
    av_frame_free(&frame);
    av_packet_free(&pkt);

	return 0;
}

void makeVideoFrame(AVFrame *videoFrame, int i) {

        /* prepare a dummy image */
        /* Y */
        for (int y = 0; y < videoFrame->height; y++) {
            for (int x = 0; x < videoFrame->width; x++) {
                videoFrame->data[0][y * videoFrame->linesize[0] + x] = x + y + i * 3;
            }
        }

        /* Cb and Cr */
        for (int y = 0; y < videoFrame->height/2; y++) {
            for (int x = 0; x < videoFrame->width/2; x++) {
                videoFrame->data[1][y * videoFrame->linesize[1] + x] = 128 + y + i * 2;
                videoFrame->data[2][y * videoFrame->linesize[2] + x] = 64 + x + i * 5;
            }
        }
        videoFrame->pts = i;
}

AVFrame *makeAudioFrame() {


}

