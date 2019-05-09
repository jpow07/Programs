USR_DIR = /usr/local
INCL_DIR = $(USR_DIR)/include
LIBAV_INCL = /usr/include/x86_64-linux-gnu/
LIB_DIR = $(USR_DIR)/lib

#Shared Libraries (.so, .dll)
LIBAV = -lavformat -lavcodec -lavutil 
LIBC = -lx264
LDLIBS = $(LIBAV) #$(LIBC)  

# Linking Flags
LDFLAGS = -L$(LIB_DIR) $(LDLIBS) # -I$(INCL_DIR) -I$(LIBAV_INCL) 

# Compile Flags
CXX_FLAGS = -Wall -g

OBJ_DIR = obj

all : muxing encoder
	
muxing : muxing.o
	@gcc obj/$^ -o $@ ${LDFLAGS}


muxing.o : muxing.c
	@gcc $^ -c -o obj/$@ ${CXXFLAGS}

encoder : encoder.o
	@gcc obj/$^ -o $@ ${LDFLAGS}


encoder.o : encoder.c
	@gcc $^ -c -o obj/$@ ${CXXFLAGS}
