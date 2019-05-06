USR_DIR = /usr/local
INCL_DIR = $(USR_DIR)/include
LIBAV_INCL = /usr/include/x86_64-linux-gnu/
LIB_DIR = $(USR_DIR)/lib

#Shared Libraries (.so, .dll)
LIBAV = -lavcodec -lavfilter -lavformat -lavutil -lswresample -lswscale -lavutil 
LIBOPENCV = -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_gapi -lopencv_photo -lopencv_video -lopencv_videoio
LIBC = -lpthread -lm
LDLIBS = $(LIBAV) $(LIBOPENCV) $(LIBC)  

# Flags (Loaded during the linking phase not the compiling phase)
LDFLAGS = -L$(LIB_DIR) $(LDLIBS) -I$(INCL_DIR) -I$(LIBAV_INCL) 
CXX_FLAGS = -g -Wall 
#-Wno-deprecated-declarations -Wno-unused-variable

OBJ_DIR = obj

all : muxing
	
muxing : muxing.o
	@gcc $^ -o $@ ${LDFLAGS}


muxing.o : muxing.c
	@gcc $^ -c -o $@ ${CXXFLAGS}
