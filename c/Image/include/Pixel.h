#ifndef PIXEL_H_
#define PIXEL_H_
/*! \file Pixel.h
 *   \brief Define Pixel Object 
 *   
 *   Pixel is an unsigned int that stores individual values as a single value
 *   This also contains macros for manipulating elementary data types and treating
 *   them as Pixel data types. 
 */

#include <stdint.h>

/*! \def Pixel
 * \brief Pixel is a macro definition for unsigned int.
*/
#define Pixel uint32_t

/*! \def RGBA2Pixel(red, green, blue, alpha)
 * \brief A macro that returns and integer from RGBA values.
 *
 * \param red is the red color value
 * \param green is the green color value
 * \param blue is the blue color value
 * \param alpha is the alpha opacity value
 *
 * \return integer that represents a specific pixel color with alpha opacity
 *
*/
#define RGBA2Pixel(red, green, blue, alpha) \
	((((((red) << 24)) | ((green) << 16)) | ((blue) << 8)) | ((alpha) << 0))

/*! \def RGB2Pixel(red, green, blue)
 * \brief A macro that returns and integer from RGB values.
 *
 * \param red is the red color value
 * \param green is the green color value
 * \param blue is the blue color value
 * \return integer that represents a specific pixel color without alpha opacity
*/
#define RGB2Pixel(red, green, blue) \
	RGBA2Pixel(red, green, blue, 0)

/*! \def getRedComponent(pixel)
 * \brief A macro that returns the red color component from the pixel.
 *
 * \param pixel is the pixel color value
 * \return Unsigned char that represents a the red component of the pixel
*/
#define getRedComponent(pixel) ((pixel) & (0xFF000000)) >> 24

/*! \def getGreenComponent(pixel)
 * \brief A macro that returns the green color component from the pixel.
 *
 * \param pixel is the pixel color value
 * \return unsigned char that represents a the green component of the pixel
*/
#define getGreenComponent(pixel) ((pixel) & (0xFF0000)) >> 16

/*! \def getBlueComponent(pixel)
 * \brief A macro that returns the blue color component from the pixel.
 *
 * \param pixel is the pixel color value
 * \return unsigned char that represents a the blue component of the pixel
*/
#define getBlueComponent(pixel) ((pixel) & (0xFF00)) >> 8 

/*! \def getAlphaComponent(pixel)
 * \brief A macro that returns the blue color component from the pixel.
 *
 * \param pixel is the pixel color value
 * \return unsigned char that represents a the blue component of the pixel
*/
#define getAlphaComponent(pixel) ((pixel) & (0xFF)) >> 0

#endif
