USR_DIR = /usr/local
INCL_DIR = $(USR_DIR)/include
LIBAV_INCL = /usr/include/x86_64-linux-gnu/
LIB_DIR = $(USR_DIR)/lib

#Compiler Version
CSTD = -std=c++11

#Shared Libraries (.so, .dll)
LIBAV = -lavformat -lavcodec -lavutil -lswscale
LIBOPENCV = -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_gapi -lopencv_photo -lopencv_video -lopencv_videoio
LIBC = -lx264 -lm -lc
LDLIBS = $(LIBOPENCV) -I /usr/local/include -L /usr/local/lib $(LIBAV) $(LIBC) 

# Linking Flags
LDFLAGS = -L$(LIB_DIR) $(LDLIBS) 

# Compile Flags
CXX_FLAGS = -Wall -g

OBJ_DIR = obj

all : muxer

muxer : muxing.o 
	@echo Loading muxer executable
	@g++ obj/$^ -o $@ $(LDFLAGS)

muxing.o : muxing.cpp
	@echo Compiling muxing.cpp
	@g++ $^ -c -o obj/$@ $(CXXFLAGS) $(CSTD)


record : main.o AVWriter.o VideoEncoder.o AVWriter

AVWriter : $(OBJ_DIR)/main.o $(OBJ_DIR)/AVWriter.o $(OBJ_DIR)/VideoEncoder.o 
	@echo Linking AVWriter.o to third-party libraries 
	@g++ $^ -o $@ $(LDFLAGS)

main.o : main.cpp
	@echo Compiling main.cpp
	@g++ $^ -c -o obj/$@ $(CXXFLAGS) $(CSTD)

AVWriter.o : AVWriter.cpp
	@echo Compiling AVWriter.cpp
	@g++ $^ -c -o obj/$@ $(CXXFLAGS) $(CSTD)

VideoEncoder.o : VideoEncoder.cpp
	@echo Compiling VideoEncoder.cpp
	@g++ $^ -c -o obj/$@ $(CXXFLAGS) $(CSTD)

AudioEncoder.o : AudioEncoder.cpp
	@echo Compiling AudioEncoder.cpp
	@g++ $^ -c -o obj/$@ $(CXXFLAGS) $(CSTD)

.PHONY : clean

clean :
	@echo cleaning up project
	@rm -f muxer $(OBJ_DIR)/*.o muxer *.mp4 *.avi *.mkv
