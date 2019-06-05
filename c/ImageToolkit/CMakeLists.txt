cmake_minimum_required(VERSION 3.1)
project(ImageToolkit LANGUAGES C)

set (ImageToolkit_VERSION_MAJOR 0)
set (ImageToolkit_VERSION_MINOR 0)
set (ImageToolkit_VERSION_PATCH 0)
set (ImageToolkit_VERSION_TWEAK 0)

add_subdirectory(src)
add_subdirectory(src/core)
add_subdirectory(include)
add_subdirectory(include/core)

target_include_directories(core PUBLIC include)

#configure_file(
#	"${PROJECT_SOURCE_DIR}/include/Version.h.in"
#	"${PROJECT_SOURCE_DIR}/include/Version.h"
#	)

add_executable(itk src/main.c)
target_link_libraries(itk PUBLIC core)

add_executable(itk_tests EXCLUDE_FROM_ALL tests/test.c)
target_link_libraries(itk_tests PUBLIC core)
