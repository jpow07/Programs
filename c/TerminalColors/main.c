#include <stdio.h>
#include "colors.h"

int main(int argc, const char *argv[]) {
	COLORTEXT_BG(255, 105, 0);
	printf("Hello, Color Terminal!\n");

	DEFAULTCOLOR_BG;
	printf("Back to Normal\n");

	COLORTEXT_BG(255, 0, 0);
	printf("I'm the color red!\n");

	COLORTEXT_BG(0, 255, 0);
	printf("I'm the color green!\n");

	COLORTEXT_BG(0, 0, 255);
	printf("I'm the color blue!\n");

	COLORTEXT_BG(255, 105, 0);
	printf("I'm the color orange!\n");

	DEFAULTCOLOR_BG;
	printf("Back to Normal Again\n");

	COLORTEXT_FG(255, 105, 0);
	printf("                 ");
	DEFAULTCOLOR_FG;
			
		

	return 0;
}
