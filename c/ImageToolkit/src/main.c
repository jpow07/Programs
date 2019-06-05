#include <stdio.h>
#include <stdlib.h>

#include "PixelBuffer.h" 

int main() {
	
	PixelBuffer pixBuffer;
	pixBuffer.pixel8Buffer = (Pixel8*) malloc(1920*1080*3);	

	Pixel8 red = RGBA2Pixel8(255, 0, 0, 0);
	printf("%x %u\n", red, getRedComponent(red));		

	Pixel8 green = RGBA2Pixel8(0, 255, 0, 0);
	printf("%x %u\n", green, getGreenComponent(green));		

	Pixel8 blue = RGBA2Pixel8(0, 0, 255, 0);
	printf("%x %u\n", blue, getBlueComponent(blue));		

	Pixel8 alpha = RGBA2Pixel8(0, 0, 0, 255);
	printf("%x %u\n", alpha, getAlphaComponent(alpha));		


	int x = 10;
	int *p = &x;

	printf("%p %p %d\n", &p, p, *p);

	return 0;
}
