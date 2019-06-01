#ifndef IMAGEBUFFER_H_
#define IMAGEBUFFER_H_

/*!
 * \file ImageBuffer.h
 * \class ImageBuffer
 * \brief Image Buffer used for holding individual Pixel information
 *
 * Buffer that stores pixel data to be used for holding pixel data
 * that is used to write, read, and manipulate Pixel data from images.
*/

#include "Pixel.h"
#include "ImageBufferMacros.h"

//#define ImageBufferSize 1920 * 1080

typedef struct ImageBuffer {

	Pixel stackBuffer[ImageBufferSize]; //!< Useful for storing image on stack 
	Pixel *heapBuffer; //!< Useful for storing images dynamically (i.e. HD, QHD, 4K)

} ImageBuffer;

/*!
  \fn void initHeapBuffer(ImageBuffer *object, int width, int height);
  \brief Initialize the *heapBuffer
  \param object The ImageBuffer Struct
  \param width Image Width
  \param height Image Height
 */
void initHeapBuffer(ImageBuffer *object, int width, int height);



#endif
