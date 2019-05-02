#include <stdio.h>
#include <stdlib.h>


#define SIZE1 3
#define SIZE2 27

int main(int argc, char *argv[]) {

	int *data[SIZE1];
	
	for(int i = 0; i < SIZE1; i++)
		data[i] = (int*) malloc(sizeof(int) *  SIZE2);

	for(int i = 0; i < SIZE1; i++)
		for(int j = 0; j < SIZE2; j++)
			data[i][j] = j;

	for(int j = 0; j < SIZE2; j++){

		if(j % 3 == 0)
			printf("[%3d,  %3d, %3d]\n", data[0][j], data[0][j+1], data[0][j+2]);
		else		
			printf("\n");
	}

	printf("Three-Dimension Array\n");
	int y_axis = 5;
	int x_axis  = 10;
	int z_axis = 3;
	
	int finite_space[y_axis * x_axis * z_axis];	
	
	for(int z = 0; z < z_axis; z++){
		printf("\tz-Plane %d\ny, x ------------>\n", z);
		for(int y = 0; y < y_axis; y++){
			printf("|");
			for(int x = 0; x < x_axis; x++){
				int zy_position = (z * y_axis) + y;
				int xyz_position = (zy_position * x_axis) + x;
				int point3d = xyz_position;
				printf("%3d ", point3d);
				finite_space[point3d] = point3d;
	
			}
			printf("\n");
		}	
		printf("V\n");
	}

	int x = 2;
	int y = 3;
	int z = 1;
	
	int zy_position = (z * y_axis) + y;
	int xyz_position = (zy_position * x_axis) + x;
	int point3d = xyz_position;

	printf("point(%d, %d, %d) : %d\n", x, y, z, finite_space[point3d]);


	for(int z = 0; z < z_axis; z++){
		if(z == 0)
			printf("\tRed\ny, x ------------>\n");
		else if( z == 1)
			printf("\tGreen\ny, x ------------>\n");
		else if( z == 2)
			printf("\tBlue\ny, x ------------>\n");

		for(int y = 0; y < y_axis; y++){
			printf("|");
			for(int x = 0; x < x_axis; x++){
				int zy_position = (z * y_axis) + y;
				int xyz_position = (zy_position * x_axis) + x;
				int point3d = xyz_position;
				printf("%3d ", point3d);
				finite_space[point3d] = point3d;
	
			}
			printf("\n");
		}	
		printf("V\n");
	}
	
	
	
	for(int y = 0; y < y_axis; y++){
		if(z == 0)
			printf("\tRed\ny, x ------------>\n");
		else if( z == 1)
			printf("\tGreen\ny, x ------------>\n");
		else if( z == 2)
			printf("\tBlue\ny, x ------------>\n");

		for(int x = 0; x < x_axis; x++){
			printf("|");
			for(int z = 0; z < z_axis; x++){
				int xy_position = (y * x_axis) + x;
				int xyz_position = (xy_position * z_axis) + z;
				int point3d = xyz_position;
				printf("%3d ", point3d);
				finite_space[point3d] = point3d;
	
			}
			printf("\n");
		}	
		printf("V\n");
	}
	
	
	
	
	return 0;


}
