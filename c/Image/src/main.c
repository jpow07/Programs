#include <stdio.h>
#include <limits.h>
#include <assert.h>

#include "ImageBuffer.h" 
#include "Pixel.h" 

int main() {
	
	ImageBuffer pixBuffer;
	pixBuffer.staticBuffer[ImageBufferSize - 1] = 255255255;	
	pixBuffer.dynamicBuffer = (Pixel*) malloc(1920*1080*3);	


	Pixel red = RGBA2Pixel(255, 0, 0, 0);
	printf("%x %u\n", red, getRedComponent(red));		

	Pixel green = RGBA2Pixel(0, 255, 0, 0);
	printf("%x %u\n", green, getGreenComponent(green));		

	Pixel blue = RGBA2Pixel(0, 0, 255, 0);
	printf("%x %u\n", blue, getBlueComponent(blue));		

	Pixel alpha = RGBA2Pixel(0, 0, 0, 255);
	printf("%x %u\n", alpha, getAlphaComponent(alpha));		

	return 0;
}
