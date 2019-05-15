# USing FFMPEG Libraries
Last Update 14-May-2019

### General Step to construct an audio video file
An AVFrame is a raw frame that can be encoded into an AVPacket.
The AVPacket is written to a digital container by interleaving the audio and video into
one stream.


1. Initialize the Codec

```
AVCodec *codec;
```

2. Initialize the Codec Context

```
AVCodecContext *context;
```

3. Initalize the Packet

```
AVPacket *packet;
```

4. Initialize the Muxer

```
```

5. Collect Frame

```
```

6. Encode the frames into packets

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





