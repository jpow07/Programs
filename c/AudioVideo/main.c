#include <stdio.h>
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>


int main(int argc, const char* argv[]) {
	

	AVFormatContext *s = NULL;
	AVDeviceInfoList **device_list;
	AVDictionary *options;
	int number_of_devices;

	//initalize AVFormatContext
	s = avformat_alloc_context();
	
	//initalize AVDeviceInfoList
	

	avdevice_register_all();

	number_of_devices = avdevice_list_devices(s, device_list);
	
	printf("Number of available devices: %d\n", number_of_devices);

	return 0;
}
