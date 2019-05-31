#ifndef PIXEL_H_
#define PIXEL_H_

#define RGBA_Pixel(r,g,b,a) (((( (r) << 8 ) | (g) ) << 8 | (b) >> 8) | (a))
#define RGB_Pixel(r,g,b) (0 | ( ( ( ( (r) << 8 ) | (g) ) << 8 | (b) ) ) )	




#endif
