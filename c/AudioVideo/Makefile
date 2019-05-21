USR_DIR = /usr/local
INCL_DIR = $(USR_DIR)/include
LIBAV_INCL = /usr/include/x86_64-linux-gnu/
LIB_DIR = $(USR_DIR)/lib

#Compiler Version
CPP_STD = -std=c++11

#Shared Libraries (.so, .dll)
LIBAV = -lavformat -lavcodec -lavutil -lswscale -lswresample
LIBOPENCV = -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_gapi -lopencv_photo -lopencv_video -lopencv_videoio
LIBC = -lx264 -lm -lc
LDLIBS = $(LIBAV) $(LIBC) $(LIBOPENCV) 

# Linking Flags
LDFLAGS = -L$(LIB_DIR) $(LDLIBS) 

# Compile Flags
CXX_FLAGS = -Wall -g

OBJ_DIR = obj

all : AVRecorder muxer
	
AVRecorder : main.o VideoWriter.o
	@echo Linking Translation Units 
	@echo Loading AVRecorder executable
	@g++ obj/main.o obj/VideoWriter.o -o $@ $(LDFLAGS)

main.o : main.cpp 
	@echo Compiling main.cpp
	@g++ $^ -c -o obj/$@ $(CXXFLAGS)

VideoWriter.o : VideoWriter.cpp
	@echo Compiling VideoWriter.cpp
	@g++ $^ -c -o obj/$@ $(CXXFLAGS)

muxer : muxing.o 
	@echo Loading muxer executable
	@g++ obj/$^ -o $@ $(LDFLAGS)

muxing.o : muxing.cpp
	@echo Compiling muxing.cpp
	@g++ $^ -c -o obj/$@ $(CXXFLAGS) $(CPP_STD)

a : mux.o 
	@echo Loading muxer executable
	@gcc obj/$^ -o $@ $(LDFLAGS)

mux.o : mux.c 
	@echo Compiling muxing.c
	@gcc $^ -c -o obj/$@ $(CXXFLAGS)

.PHONY : clean

clean :
	@echo cleaning up project
	@rm -f prog muxer $(OBJ_DIR)/*.o AVRecorder a muxer *.mp4 *.avi *.mkv
