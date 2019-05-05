
LIBS = -lavdevice -lavformat
CFLAGS = -Wall -g

all : devices
	
devices: main.o
	gcc $^ -o $@ ${LIBS}


main.o : main.c
	gcc $^ -c -o $@ ${CFLAGS}
