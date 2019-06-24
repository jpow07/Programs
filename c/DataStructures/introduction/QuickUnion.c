/* Use redirection with test2.txt to test */
#include <stdio.h>

#define N 10

int main(int argc, char* argv[]) {
	// Pairs connected from p to q (p - q)
	int p, q, t;
	int id[N];
	int junkPairs[N];	
	puts("p q | 0 1 2 3 4 5 6 7 8 9");
	puts("----+--------------------");
	// Fill array from 0 to N
	for(int i = 0; i < N; i++)
		id[i] = i;
	
	int i, j;
	int k = 0;
	// Grab two points as input
	while (scanf("%d %d\n", &p, &q) == 2) {
		for(i = p; i != id[i]; i = id[i]);
		for(j = q; j != id[j];j = id[j]);
		if(i == j){
			junkPairs[k] = p;
			k++;
			junkPairs[k] = q;
			k++;		
		}

		id[i] = j;

		//print out the values for p and q and the updated connectivity graph
		printf("%d %d | ", p, q);
		//print out the results
		for (int i = 0; i < N; i++)
			printf("%d ", id[i]);
		puts("");
		
	}	

	//Once all points are connected print out the junk pairs
	puts("");
	printf("Junk Connections (Previously declared when input)\n");
	for (int i = 0; i < k; i++) {
		printf("%d ", junkPairs[i]);
		if(i % 2 == 1)
			printf("\n");
	}


	return 0;
}
