/*
 *
 *
 *
 */
#include <iostream>
#include "AV_Writer.hpp"

int main(int argc, char *argv[]) {
	std::string fileName = "test.mp4";
	std::string codecName = "libx264";
	std::cout << "Print" << std::endl;
	AV_Writer muxer(fileName.c_str(), codecName.c_str(), 344, 288, 30);
	


	return 0;
}
