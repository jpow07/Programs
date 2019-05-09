#Directory
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
CXX_FLAGS = -g 
#-Wno-deprecated-declarations -Wno-unused-variable

OBJ_DIR = obj

all: play
	@echo 'Compilation, linking and loading complete'

#Link
play : main.o func.o func.h
	@echo 'Linking files'
	g++ $(OBJ_DIR)/*.o -o $@ $(LDFLAGS)


#Compile
main.o : main.cpp 
	@echo 'Compiling files'
	@g++ $^ -c -o $(OBJ_DIR)/$@ $(CXX_FLAGS)

func.o : func.c
	@gcc $^ -c -o $(OBJ_DIR)/$@ $(CXX_FLAGS)

.PHONY: clean
clean :
	@rm obj/*.o
