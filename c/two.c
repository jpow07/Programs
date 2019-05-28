#include <stdio.h>

int main(int argc, char *argv[]) {
	//65536	
	//
	unsigned int value; 
	for(int i = 0; i < 100000000; i++) {
		value = 65536 * ((255 * i) % 255) + 256 * ((255 + i) % 255) + 255;
	}

	printf("0x%x\n", value );

	return 0;
}
