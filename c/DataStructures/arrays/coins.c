#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main(int argc, char *argv[]) {
	if (argc < 3) {
		printf("Usage: %s [Number of Flips] [Number of Tests]", argv[0]);
		exit(1);
	}
	
	int numTests = atoi(argv[1]);
	int numCoinFlips = atoi(argv[2]);
	int numFlips = numCoinFlips * numTests;
	int *coinFlips = (int*)calloc(sizeof(int), numTests);
	srand((unsigned int) time(0));

	for(int i = 0; i < numTests; i++){
		for (int j = 0; j < numCoinFlips; j++){
			if (rand() % 2) // <-- Flip a coin
				coinFlips[i]++;// += 1;
		}
	}

	int sum = 0;
	for (int i = 0; i < numTests; i++) {
		sum += coinFlips[i];
		printf("Heads occured %d Times\n", coinFlips[i]); 
	}
	printf("Number of Flips: %d\n"
		"Average: %2.0f%%\n", numFlips, ((float) sum / numFlips) * pow(10, 2));

	free(coinFlips);
	return 0;
}
