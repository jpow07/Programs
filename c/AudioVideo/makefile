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
	
muxing : muxing.o
	@gcc obj/$^ -o $@ $(LDFLAGS)

muxing.o : muxing.c
	@gcc $^ -c -o obj/$@ $(CXXFLAGS)

encoder : encoder.o
	@gcc obj/$^ -o $@ $(LDFLAGS)

encoder.o : encoder.c
	@gcc $^ -c -o obj/$@ $(CXXFLAGS)

prog : main.o
	@gcc obj/$^ -o $@ $(LDFLAGS)

main.o : main.c
	@gcc $^ -c -o obj/$@ $(CXXFLAGS)

