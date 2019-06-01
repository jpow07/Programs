#include <stdio.h>

#include "ImageBuffer.h" 

int main() {
	
	ImageBuffer pixBuffer;
	pixBuffer.buffer[ImageBufferSize - 1] = 255255255;	

	printf("%d\n", 	pixBuffer.buffer[ImageBufferSize - 1]);		

	return 0;
}
