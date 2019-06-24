
CFLAGS = -Wall -g

all : BMPWriter

BMPWriter : main.o
	@echo Linking main
	@echo Creating BMPWriter
	gcc $^ -o $@ 

main.o : main.c
	@echo Linking main
	gcc $^ -c -o $@ $(CFLAGS)

.PHONY : clean

clean :
	@rm -f *.o BMPWriter
