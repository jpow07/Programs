#Directory
USR_DIR = /usr/local
INCL_DIR = $(USR_DIR)/include
LIB_DIR = $(USR_DIR)/lib

#Shared Libraries (.so, .dll)
LIBAV = -lavcodec -lavfilter -lavformat -lavutil -lswresample -lswscale -lavutil 
LIBOPENCV = -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_gapi -lopencv_photo -lopencv_video -lopencv_videoio -lx264
LDLIBS = $(LIBAV) $(LIBOPENCV)

# Flags (Loaded during the linking phase not the compiling phase)
LDFLAGS = -L$(LIB_DIR) -I$(INCL_DIR) $(LDLIBS) 
CXX_FLAGS = -Wall -g 
#-Wno-deprecated-declarations -Wno-unused-variable

OBJ_DIR = obj


all: play
	@echo 'Compilation, linking and loading complete'

play : main.o
	@echo 'Linking files'
	@g++ $(OBJ_DIR)/$^ -o $@ $(LDFLAGS)

main.o : main.cpp
	@echo 'Compiling files'
	@g++ $^ -c -o $(OBJ_DIR)/$@ $(CXX_FLAGS)


.PHONY: clean
clean :
	@rm obj/*.o
.PHONY : run
run :
	@./play gohan.jpeg out.264 libx264rgb
