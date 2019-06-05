#include <stdio.h>
#include <limits.h>
#include <assert.h>

#include "Pixel.h"

int main () {

	assert(sizeof(Pixel8) == 4);
	printf("\033[38;2;0;255;0mTest Passed\n");

	assert(RGBA2Pixel8(255, 255, 255, 255) == UINT_MAX);
	printf("\033[38;2;0;255;0mTest Passed\n");

	assert(RGB2Pixel8(255, 255, 255) == (UINT_MAX & 0xFFFFFF00));
	printf("\033[38;2;0;255;0mTest Passed\n");

	return 0;

}
