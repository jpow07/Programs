#ifndef IMAGEBUFFER_H_
#define IMAGEBUFFER_H_

#include "ImageBufferMacros.h"

//#define ImageBufferSize 1920 * 1080

typedef struct ImageBuffer {

	unsigned int buffer[ImageBufferSize];		

} ImageBuffer;

void pushBackImageBuffer(ImageBuffer *object);



#endif
