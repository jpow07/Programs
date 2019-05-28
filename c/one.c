#include <stdio.h>

int main(int argc, char *argv[]) {

	unsigned int value;
	for(int i = 0; i < 100000000; i++) {
		value = ((255 * i % 255 ) << 16) | ((255 + i) % 255 << 8) | (255 << 0);
	}
	printf("0x%x\n", value );

	return 0;
}
