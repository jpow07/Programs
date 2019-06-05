#ifndef PIXELBUFFER_H_
#define PIXELBUFFER_H_

/*!
 * \file ImageBuffer.h
 * \class ImageBuffer
 * \brief Image Buffer used for holding individual Pixel information
 *
 * Buffer that stores pixel data to be used for holding pixel data
 * that is used to write, read, and manipulate Pixel data from images.
*/
#include <limits.h>
#include "Pixel.h"

typedef struct PixelBuffer {
	
	Pixel1 *pixel1Buffer; //!< Buffer to store Pixel1 data
	Pixel8 *pixel8Buffer; //!< Buffer to store Pixel8 data
	Pixel16 *pixel16Buffer; //!< Buffer to store Pixel17 data
	Pixel24 *pixel24Buffer; //!< Buffer to store Pixel24 data
	Pixel48 *pixel48Buffer; //!< Buffer to store Pixel48 data

} PixelBuffer;

/*!
  \fn void initHeapBuffer(ImageBuffer *object, int width, int height);
  \brief Initialize the *heapBuffer
  \param object The ImageBuffer Struct
  \param width Image Width
  \param height Image Height
 */
void initHeapBuffer(PixelBuffer* object, int width , int height);



#endif
