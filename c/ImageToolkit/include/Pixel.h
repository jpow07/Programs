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

//Pixel Definitions

/*! \def Pixel1
 * \brief Pixel1 is a uchar capable of representing 1-bit monochrome color (Black or White)
*/
typedef struct Pixel1 {
	unsigned char monochome : 1;

} Pixel1;

/*! \def Pixel8
 * \brief Pixel8 is a uint32_t capable of representing 8-bit grayscale and 8-bit color
*/
#define Pixel8 uint32_t

/*! \def Pixel16
 * \brief Pixel16 is an uint64_t capable of representing 16-bit color depth (High color).
*/
#define Pixel16 uint64_t

/*! \def Pixel24
 * \brief Pixel24 is an struct capable of representing 24-bit color depth (True Color).
*/
typedef struct Pixel24 {

	uint32_t red; 
	uint32_t green; 
	uint32_t blue; 
	uint32_t alpha; 

} Pixel24;

/*! \def Pixel48
 * \brief Pixel48 is an struct capable of representing 48-bit color depth (Deep Color).
*/
typedef struct Pixel48 {

	uint64_t red; 
	uint64_t green; 
	uint64_t blue; 
	uint64_t alpha; 

} Pixel48;


/*! \def RGBA2Pixel16(red, green, blue, alpha)
 * \brief Pass color components to create a Pixel8 object. 
 *
 * \param red is the red color value
 * \param green is the green color value
 * \param blue is the blue color value
 * \param alpha is the alpha opacity value
 *
 * \return integer that represents a specific pixel color with alpha opacity
 *
*/
#define RGBA2Pixel8(red, green, blue, alpha) \
	((((((red) << 24)) | ((green) << 16)) | ((blue) << 8)) | ((alpha) << 0))

/*! \def RGB2Pixel(red, green, blue)
 * \brief A macro that returns and integer from RGB values.
 *
 * \param red is the red color value
 * \param green is the green color value
 * \param blue is the blue color value
 * \return integer that represents a specific pixel color without alpha opacity
*/
#define RGB2Pixel8(red, green, blue) \
	RGBA2Pixel8(red, green, blue, 0)

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
