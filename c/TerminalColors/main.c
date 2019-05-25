#include <stdio.h>
#include "colors.h"

int main(int argc, const char *argv[]) {
	COLORTEXT(255, 105, 0);
	printf("Hello, Color Terminal!\n");

	DEFAULTCOLOR;
	printf("Back to Normal\n");

	COLORTEXT(255, 0, 0);
	printf("I'm the color red!\n");

	COLORTEXT(0, 255, 0);
	printf("I'm the color green!\n");

	COLORTEXT(0, 0, 255);
	printf("I'm the color blue!\n");

	COLORTEXT(255, 105, 0);
	printf("I'm the color orange!\n");

	return 0;
}
