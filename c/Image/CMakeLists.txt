cmake_minimum_required(VERSION 3.10)
project(Image LANGUAGES C)

set (Image_VERSION_MAJOR 0)
set (Image_VERSION_MINOR 0)
set (Image_VERSION_PATCH 0)
set (Image_VERSION_TWEAK 0)

add_subdirectory(src)
add_subdirectory(src/core)
add_subdirectory(include)

#configure_file(
#	"${PROJECT_SOURCE_DIR}/include/Version.h.in"
#	"${PROJECT_SOURCE_DIR}/include/Version.h"
#	)

add_executable(image.out src/core/main.c)
