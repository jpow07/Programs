USR_DIR = /usr/local
INCL_DIR = $(USR_DIR)/include
LIBAV_INCL = /usr/include/x86_64-linux-gnu/
LIB_DIR = $(USR_DIR)/lib

#Shared Libraries (.so, .dll)
LIBAV = -lavformat -lavcodec -lavutil -lswscale -lswresample
LIBC = -lx264 -lm -lc
LDLIBS = $(LIBAV) $(LIBC)  

# Linking Flags
LDFLAGS = -L$(LIB_DIR) $(LDLIBS) 

# Compile Flags
CXX_FLAGS = -Wall -g

OBJ_DIR = obj

all : prog 
	
prog : main.o AV_Writer.o
	@g++ obj/main.o obj/AV_Writer.o -o $@ $(LDFLAGS)

main.o : main.cpp 
	@g++ $^ -c -o obj/$@ $(CXXFLAGS)

AV_Writer.o : AV_Writer.cpp
	@g++ $^ -c -o obj/$@ $(CXXFLAGS)




mux : muxing.o 
	@gcc obj/$^ -o $@ $(LDFLAGS)

muxing.o : muxing.c 
	@gcc $^ -c -o obj/$@ $(CXXFLAGS)
