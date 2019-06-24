#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


int main(int argc, char *argv[]){


	unsigned int primeSize = 1000;

	puts("0 and 1 cannot be prime.");
	puts("2, 3, 5 are prime.");
	puts("The remaining prime numbers are: ");
	for (unsigned int i = 5; i < primeSize; i += 2) {
		if (i % 3 == 0 || i % 5 == 0)
			continue;
		else
			printf("%d\n", i);
	}

	return 0;
}
