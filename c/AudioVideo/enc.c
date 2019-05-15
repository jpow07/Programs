#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>

#define STREAM_DURATION   240.0 // Seconds
#define STREAM_FRAME_RATE 25 // 25 images
#define STREAM_PIX_FMT    AV_PIX_FMT_YUV420P /* default pix_fmt */
#define SCALE_FLAGS SWS_BICUBIC
#define NUM_OF_FRAMES 100


typedef struct OutputStream {
    AVStream *st;
    AVCodecContext *encoder; //encoder

    // pts of the next frame that will be generated
    int64_t next_pts;		// Linked List of presentation timestamp
    AVFrame *frame;		//Frame to be captured
    AVFrame *tmp_frame;		// Temporary frame (used for backup)

} OutputStream;

void encodeFrame(AVCodecContext *encoder, AVFrame *frame, AVPacket *pkt, FILE *outfile);
void getFrame(AVFrame *frame);
AVCodecContext* initCodecContext(AVCodecContext *codec_ctx);
AVFrame *initFrame(enum AVPixelFormat pix_fmt, int width, int height);
AVOutputFormat *initOutputFormat(AVOutputFormat format, FILE *file);

int main(int argc, char *argv[]) {
	int return_code;
	
	//User Input
    	const char *filename = "out.264";
	
	//Initialize Packet that stores the data
    	AVPacket *pkt;
    	pkt = av_packet_alloc();
    	if (!pkt)
        	exit(1);

	//Initialize Encoder Context
	AVCodecContext *encoder = initCodecContext(encoder);
	

	// Initalize OutputFormat
	AVOutputFormat format;	

	//Initialize AVFrame
	AVFrame *frame = initFrame(encoder->pix_fmt, encoder->width, encoder->height);
	

	//Open File
    	uint8_t EOF_parameters[] = { 0, 0, 1, 0xb7 }; //This is appended to end of file
	FILE *file_index;
	file_index = fopen(filename, "wb");	
	if(!file_index) {
        	fprintf(stderr, "Could not open %s\n", filename);
        	exit(1);
	} 

	//Initialize Format Context
	AVFormatContext *output_context;	

	// allocate the output media context
	avformat_alloc_output_context2(&output_context, NULL, NULL, filename);
	if (!output_context) {
        	printf("Could not deduce output format from file extension: using MPEG.\n");
        	avformat_alloc_output_context2(&output_context, NULL, "mpeg", filename);
        	return 1;
	}

	// Initialize Stream
	AVStream *stream = avformat_new_stream(output_context, NULL);
	if (!stream) {
        	fprintf(stderr, "Could not allocate stream\n");
        	exit(1);
    	}

	// Capture Frames to be written 
    	for (int i = 0; i < NUM_OF_FRAMES; i++) {
        	fflush(stdout);

		getFrame(frame);
		frame->pts = i;

		encodeFrame(encoder, frame, pkt, file_index);
	}

	//Finalize Binary File with Footer (EOF_parameters)
    	encodeFrame(encoder, NULL, pkt, file_index);//Flushes the buffer
    	fwrite(EOF_parameters, 1, sizeof(EOF_parameters), file_index);
	fclose(file_index);

	//Clean up Structures
	avcodec_free_context(&encoder);
	av_frame_free(&frame);
	av_packet_free(&pkt);


    return 0;
}

void encodeFrame(AVCodecContext *encoder, AVFrame *frame, AVPacket *pkt, FILE *outfile) {

    int ret;

    /* send the frame to the encoder */
    if (frame)
        printf("Send frame %3"PRId64"\n", frame->pts);

    ret = avcodec_send_frame(encoder, frame);
    if (ret < 0) {
        fprintf(stderr, "Error sending a frame for encoding\n");
        exit(1);
    }

    while (ret >= 0) {
        ret = avcodec_receive_packet(encoder, pkt);
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
}

void getFrame(AVFrame *frame) {
	
	
	int ret;
        // make sure the frame data is writable
        ret = av_frame_make_writable(frame);
        if (ret < 0)
            exit(1);
        // prepare a dummy image
        // Y
        for (int y = 0; y < frame->height; y++) {
            for (int x = 0; x < frame->width; x++) {
                frame->data[0][y * frame->linesize[0] + x] = x + y * 3;
            }
        }
        // Cb and Cr
        for (int y = 0; y < frame->height/2; y++) {
            for (int x = 0; x < frame->width/2; x++) {
                frame->data[1][y * frame->linesize[1] + x] = 128 + y * 2;
                frame->data[2][y * frame->linesize[2] + x] = 64 + x * 5;
            }
        }
}

AVCodecContext* initCodecContext(AVCodecContext *codec_ctx) {
	printf("BREAKPOINT initCodecContext\n");
    	const char *codec_name = "libx264";
	int return_code; //checks for errors, not returned by function

	//Initialize Codec
	const AVCodec *codec;

    	codec = avcodec_find_encoder_by_name(codec_name);
	printf("BREAKPOINT initCodecContext\n");
    	if (!codec) {
        	fprintf(stderr, "Codec '%s' not found\n", codec_name);
        	exit(1);
    	}
		
	printf("Codec: %s\n", codec_name);

    	codec_ctx = avcodec_alloc_context3(codec);
    	if (!codec_ctx) {
        	fprintf(stderr, "Could not allocate video codec context\n");
        	exit(1);
    	}

    	codec_ctx->bit_rate = 40000;
    	codec_ctx->width = 352 ;
    	codec_ctx->height = 288;
    	codec_ctx->time_base = (AVRational){1, 25};
    	codec_ctx->framerate = (AVRational){25, 1};
    	codec_ctx->gop_size = 10;
    	codec_ctx->max_b_frames = 1;
    	codec_ctx->pix_fmt = AV_PIX_FMT_YUV420P;

    	if (codec_ctx->codec->id == AV_CODEC_ID_H264)
		av_opt_set(codec_ctx->priv_data, "preset", "slow", 0);

    	return_code = avcodec_open2(codec_ctx, codec_ctx->codec, NULL);
    	if (return_code < 0) {
       	 	fprintf(stderr, "Could not open codec: %s\n", av_err2str(return_code));
        	exit(1);
    	}

	return codec_ctx;
}

AVFrame *initFrame(enum AVPixelFormat pix_fmt, int width, int height) {
    AVFrame *image;
    int ret;

    image = av_frame_alloc();
    if (!image)
        return NULL;

    image->format = pix_fmt;
    image->width  = width;
    image->height = height;

    // allocate the buffers for the frame data
    ret = av_frame_get_buffer(image, 32);
    if (ret < 0) {
        fprintf(stderr, "Could not allocate frame data.\n");
        exit(1);
    }

    return image;
}

AVOutputFormat *initOutputFormat(AVOutputFormat format, FILE *file) {

	

}
