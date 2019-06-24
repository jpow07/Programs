#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Point {
	float x;
	float y;
} Point;

float distance(Point first, Point second) {
	double xDistance = second.x - first.x;
	double yDistance = second.y - first.y;
	double xPow = pow(xDistance, 2);	
	double yPow = pow(yDistance, 2);	
	float distance = (float) sqrt(xPow + yPow);
	return distance;
}

float randFloat() {
	return 1.0 * rand() / RAND_MAX;
}

int main(int argc, char *argv[]) {

	float d = atof(argv[2]);
	int count = 0;
	int N = atoi(argv[1]);
	Point *pt = malloc(N * sizeof(*pt));

	for(int i = 0; i < N; i++) {
		pt[i].x = randFloat();
		pt[i].y	= randFloat();
//		printf("Point %d: %.2f, %.2f\n", i, pt[i].x, pt[i].y);
	}

	for (int i = 0; i < N; i++)
		for (int j = i+1; j < N; j++)
			if(distance(pt[i], pt[j]) < d) {
				printf("Point %d: %.2f, %.2f\n", i, pt[i].x, pt[i].y);
				printf("Point %d: %.2f, %.2f\n", j, pt[j].x, pt[j].y);
				printf("Distance: %.2f\n", distance(pt[i], pt[j]));
				count++;
			}
	printf("Number of Comparisons: %d\n", N * N);
	printf("%d edges shorter than %f\n", count, d);
	
	free(pt);
	return 0;
}
