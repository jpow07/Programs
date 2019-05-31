#include <stdio.h>
//#include <stdlib.h>

const int bytesPerPixel = 4; /// red, green, blue
const int fileHeaderSize = 14;
const int infoHeaderSize = 40;

void generateBitmapImage(unsigned char *image, int height, int width, char* imageFileName);
unsigned char* createBitmapFileHeader(int height, int width, int paddingSize);
unsigned char* createBitmapInfoHeader(int height, int width);

#define RGBA_Pixel(r,g,b,a) (((( (r) << 8 ) | (g) ) << 8 | (b) >> 8) | (a))
#define RGB_Pixel(r,g,b) (0 | ( ( ( ( (r) << 8 ) | (g) ) << 8 | (b) ) ) )	

int main(){

    int height = 1080;
    int width = 1920;
    unsigned int fileSize = height * width * bytesPerPixel;
    unsigned int image[height][width];//[bytesPerPixel];
    char* imageFileName = "bitmap.bmp";

	printf("File Size: %'u bytes; %'u bits\n", fileSize, fileSize * 8);

    int i, j;
    for(i = 0; i < height; i++){
        for(j = 0; j < width; j++){
		image[i][j] = RGB_Pixel(255, 105, 0);
        }
    }

    generateBitmapImage((unsigned char *)image, height, width, imageFileName);
    printf("Image generated!\n");
}


void generateBitmapImage(unsigned char *image, int height, int width, char* imageFileName){

    unsigned char padding[3] = {0, 0, 0};
    int paddingSize = (4 - (width*bytesPerPixel) % 4) % 4;

    unsigned char* fileHeader = createBitmapFileHeader(height, width, paddingSize);
    unsigned char* infoHeader = createBitmapInfoHeader(height, width);

    FILE* imageFile = fopen(imageFileName, "wb");

    fwrite(fileHeader, 1, fileHeaderSize, imageFile);
    fwrite(infoHeader, 1, infoHeaderSize, imageFile);

    int i;
    for(i=0; i<height; i++){
        fwrite(image+(i*width*bytesPerPixel), bytesPerPixel, width, imageFile);
        fwrite(padding, 1, paddingSize, imageFile);
    }

    fclose(imageFile);
 //   free(fileHeader);
 //   free(infoHeader);
}

unsigned char* createBitmapFileHeader(int height, int width, int paddingSize){
    int fileSize = fileHeaderSize + infoHeaderSize + (bytesPerPixel*width+paddingSize) * height;

    static unsigned char fileHeader[] = {
        0,0, /// signature
        0,0,0,0, /// image file size in bytes
        0,0,0,0, /// reserved
        0,0,0,0, /// start of pixel array
    };

    fileHeader[ 0] = (unsigned char)('B');
    fileHeader[ 1] = (unsigned char)('M');
    fileHeader[ 2] = (unsigned char)(fileSize    );
    fileHeader[ 3] = (unsigned char)(fileSize>> 8);
    fileHeader[ 4] = (unsigned char)(fileSize>>16);
    fileHeader[ 5] = (unsigned char)(fileSize>>24);
    fileHeader[10] = (unsigned char)(fileHeaderSize + infoHeaderSize);

    return fileHeader;
}

unsigned char* createBitmapInfoHeader(int height, int width){
    static unsigned char infoHeader[] = {
        0,0,0,0, /// 0-3 header size
        0,0,0,0, /// 4-7 image width
        0,0,0,0, /// 8-11 image height
        0,0, ///     12-13 number of color planes
        0,0, ///     13-14 bits per pixel
        0,0,0,0, /// 15-18 compression
        0,0,0,0, /// 19-22 image size
        0,0,0,0, /// 23-26 horizontal resolution
        0,0,0,0, /// 27-30 vertical resolution
        0,0,0,0, /// 31-34 colors in color table
        0,0,0,0, /// 35-38 important color count
    };

    infoHeader[ 0] = (unsigned char)(infoHeaderSize);
    infoHeader[ 4] = (unsigned char)(width    );
    infoHeader[ 5] = (unsigned char)(width>> 8);
    infoHeader[ 6] = (unsigned char)(width>>16);
    infoHeader[ 7] = (unsigned char)(width>>24);
    infoHeader[ 8] = (unsigned char)(height    );
    infoHeader[ 9] = (unsigned char)(height>> 8);
    infoHeader[10] = (unsigned char)(height>>16);
    infoHeader[11] = (unsigned char)(height>>24);
    infoHeader[12] = (unsigned char)(1);
    infoHeader[14] = (unsigned char)(bytesPerPixel*8);
    


    return infoHeader;
}
