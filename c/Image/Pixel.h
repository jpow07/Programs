#ifndef PIXEL_H_
#define PIXEL_H_
/*! \file Pixel.h
 *   \brief Define Pixel Object 
 *   
 *   Pixel is an unsigned int that stores individual values as a single value
 *   This also contains macros for manipulating elementary data types and treating
 *   them as Pixel data types. 
 */

/*! \def Pixel
 * \brief Pixel is a macro definition for unsigned int.
*/
#define Pixel unsigned int

/*! \def RGBA2Pixel(red, green, blue, alpha)
 * \brief A macro that returns and integer from RGBA values.
 *
 * \param red is the red color value
 * \param green is the green color value
 * \param blue is the blue color value
 * \param alpha is the alpha opacity value
 *
 * \return Integer that represents a specific pixel color with alpha opacity
 *
*/
#define RGBA2Pixel(red, green, blue, alpha) \
	(((((red) << 8 ) | (green) ) << 8 | (blue) >> 8) | (alpha)) 

/*! \def RGB2Pixel(red, green, blue)
 * \brief A macro that returns and integer from RGB values.
 *
 * \param red is the red color value
 * \param green is the green color value
 * \param blue is the blue color value
 * \return Integer that represents a specific pixel color without alpha opacity
*/
#define RGB2Pixel(red, green, blue) \
	(0 | ( ( ( ( (red) << 8 ) | (green) ) << 8 | (blue) ) ) )	


#endif
