project(Image VERSION 0.0 LANGUAGES C)

include_directories(${Image_SOURCE_DIR}/src/core)

add_library(core STATIC Pixel.c ImageBuffer.c ../../include/Pixel.h ../../include/ImageBuffer.h)


