#include <stdio.h>
#include <stdint.h> 

#define DEBUG_ON 1
#define DEBUG_OFF 0

#define BITMAPFILEHEADER uint16_t;
#define BITMAPINFOHEADER uint16_t;
#define DEFAULT 0
enum {
	UNCOMPRESSED,	//Uncompressed
	RLE8,		//8-bit image
	RLE4,		//4-bit image
	BITFIELDS	//16-bit and 32-bit images

};

#define PIXEL uint32_t
#define RGBA_Pixel(r,g,b,a) (((((r) << 8) | (g)) << 8 | (b)) << 8 | (a))
#define RGB_Pixel(r,g,b) ( ( ( (r) << 8) | (g)) << 8 | (b))
#define HEADER_KEY(a, b) (((a) << 8) | (b))

int main(int argc, char *argv[]) {
	//BMP Information
	uint32_t height_resolution = 10;
	uint32_t width_resolution = 20;
	uint32_t number_of_pixels = height_resolution * width_resolution;  

	// File Header
	uint16_t header = HEADER_KEY('B', 'M');
	uint16_t reserved1 = DEFAULT;	
	uint16_t reserved2 = DEFAULT;	
	const uint32_t offset  = 26; // Number of bytes until the image data	
	uint32_t filesize = number_of_pixels + offset;
	
	// Image Header
	uint32_t image_size = 40;
	uint32_t image_width = width_resolution;
	uint32_t image_height = height_resolution;
	const uint16_t num_color_planes = 1;
	uint16_t bit_count = 24;
	uint32_t compression = UNCOMPRESSED;	
	uint32_t vertical_width = DEFAULT;
	uint32_t horizontal_height = DEFAULT;
	uint32_t color_map = DEFAULT;
	uint32_t significant_colors = DEFAULT;

	const char *filename = argv[1];	
	FILE *file_index;
	file_index = fopen(filename,"wb");	
	
	//Write file header
	fwrite(&header, sizeof(header), 1, file_index);
	fwrite(&filesize, sizeof(filesize), 1, file_index);
	fwrite(&reserved1, sizeof(reserved1), 1, file_index);
	fwrite(&reserved2, sizeof(reserved2), 1, file_index);
	fwrite(&offset, sizeof(offset), 1, file_index);

	//Write image header
	fwrite(&image_size, sizeof(image_size), 1, file_index);
	fwrite(&image_width, sizeof(image_width), 1, file_index);
	fwrite(&image_height, sizeof(image_height), 1, file_index);
	fwrite(&num_color_planes, sizeof(num_color_planes), 1, file_index);
	fwrite(&bit_count, sizeof(bit_count), 1, file_index);
	fwrite(&compression, sizeof(compression), 1, file_index);
	fwrite(&vertical_width, sizeof(vertical_width), 1, file_index);
	fwrite(&horizontal_height, sizeof(horizontal_height), 1, file_index);
	fwrite(&color_map, sizeof(color_map), 1, file_index);
	fwrite(&significant_colors, sizeof(significant_colors), 1, file_index);

	int image[number_of_pixels];

	for (int i = 0; i < image_width; i++) {
		for (int j = 0; j < image_height; j++) {
			image[(i * image_height) + j] = RGB_Pixel(255,0,0);	
//			printf("%d ", (i * image_height) + j);
		}	
	}

	int i = 0;
	while(i < number_of_pixels) {
		fwrite(image, sizeof(int), number_of_pixels, file_index);
		i++;
	}
	printf("i: %d pixels: %d\n", i, number_of_pixels);	
	fclose(file_index);


	return 0;
}


