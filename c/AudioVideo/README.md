# USing FFMPEG Libraries
Last Update 14-May-2019

### General Step to construct an audio video file
An AVFrame is a raw frame that can be encoded into an AVPacket.
The AVPacket is written to a digital container by interleaving the audio and video into
one stream.

0. Define Variable/Structs

```
	AVOutputFormat format;
	AVFormatContext
```

1. Initialize the Codec

```
AVCodec *codec;
codec = avcodec_find_encoder_by_name("libx264");
if (!codec) {
	fprintf(stderr, "Codec '%s' not found\n", codec_name);
	exit(1);
}
```

2. Initialize the Codec Context

```
AVCodecContext *context;
context = avcodec_alloc_context3(codec); 
if (!codec) {
	fprintf(stderr, "Could not allocate video codec context\n");
	exit(1);
}
```

3. Initalize the Packet

```
AVPacket *packet;
packet = av_packet_alloc();
if(!packet) {
	fprintf(stderr, "Failure to initialize packet\n");
	exit(1);
}
```

4. Initialize the Codec Context

```
context->codec = codec; // Store codec in CodecContext struct
context->bit_rate = 400000;
context->width = 1920;
context->height = 1080;
context->time_base = (AVRational){1, FRAMERATE}; // FRAMERAte should be defined prior
context->framerate = (AVRational){FRAMERATE, 1};
context->gop_size = 3; // Determine number of I-Frames every X frames
context->max_b_frames = 1; // Max number of B-frames per gop_size
context->pix_fmt = AV_PIX_FMT_YUV420P; // Pixel format (RGB, BGR, YUV)
```

a. Set Options for H264 encoder

```
if (context->codec->id == AV_CODEC_ID_H264) {
	av_opt_set(context->codec->priv_data, "preset", "slow", 0);
}
```

5. Open Codec

```
ret = avcodec_open2(context, context->codec, NULL);
if (ret < 0) {
	fprintf(stderr, "Could not open codec %s\n", av_err2str(ret));
	exit(1);
}
```


7. Open the file

```
AVFormatContext formatContext;
avformat_alloc_output_context(formatContext, NULL, NULL, filename);
if (!formatContext) {
	return 1;
}

```

8. Initialize AVFormatContext

```



```

9. Allocate Frame data

```
frame = av_frame_alloc();
frame->format = contex->pix_fmt;
frame->width = context->width;
frame->height = context->height;

ret = av_frame_get_buffer(frame, 32);
if (ret < 0) {
	fprintf(stderr, "Could not allocate the video frame data\n");
	exit(1);
}
```

9. 

10. Encode the frames into packets

```
//Send frame to be encoded
successful = avcodec_send_frame();
if(!succesful) {
	printf("Error Message\n");
	exit(1);
}

//receive the previous frame as an encoded packet.
successful = return_code = avcodec_receive_packet();
if(!error) {
	printf("Error Message\n");
	exit(1);
}
```

7. Interleave Packets into a single AVStream (Muxing)

```
```





