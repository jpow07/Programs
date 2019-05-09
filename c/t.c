#include <stdio.h>
#include <inttypes.h>


int main() { 

	uint64_t x = 100001;
	printf("%s\n", PRId64);
	printf("%"PRId64"\n", x);


	return 0;
};
