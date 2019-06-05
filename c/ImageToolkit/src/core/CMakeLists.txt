project(ImageToolkit)

include_directories(${Image_SOURCE_DIR}/src/core)

add_library(core Pixel.c PixelBuffer.c 
	../../include/Pixel.h ../../include/PixelBuffer.h)


