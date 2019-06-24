#include <stdio.h>
#include <assert.h>

int main() {
	
	int x = 5;
	assert(x == 4); // Disable with -DNDEBUG in gcc
	printf("%d\n", x);
		
	for (int i = 33; i < 121; i++) {
		printf("\033[39mPassword attempt: ");
		for (int j = 0; j < 6; j++ ) {
			printf("\033[38;2;200;100;100m%c", i + j);
		}
		printf("\n\033[38;2;255;0;0mattempt failed.\n");

	}		
	printf("\n");

	return 0;
}
