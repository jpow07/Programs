#Directories


PROGRAM_NAME = ImageBuffer
CFLAGS = -Wall -g

COMPILE = gcc -c $< -c -o $@
LINK = gcc -o $(PROGRAM_NAME) $^ $(LIBS)

#Compile Program Libraries to link to program executable
PROGRAM_EXECUTABLES = main.o
PROGRAM_LIBRARIES = ImageBuffer.o Pixel.o

# Link and External Libraries
EXTERNAL_LIBRARIES =

all : $(PROGRAM_LIBRARIES) $(PROGRAM_EXECUTABLES)
	@echo Linking Project
	@$(LINK) $(LIBS) 

ImageBuffer.o : ImageBuffer.c 
	@echo Compiling ImageBuffer.c
	@$(COMPILE) $(CFLAGS)

Pixel.o : Pixel.c Pixel.h
	@echo Compiling Pixel.c
	@$(COMPILE) $(CFLAGS)

main.o : main.c
	@echo Compiling main.c
	@$(COMPILE) $(CFLAGS)



.PHONY : ImageBufferMacros generate-macros clean-all

ImageBufferMacros :
	@m4 $@.m4 > $@.h

generate-macros : ImageBufferMacros
	@echo Generating Macros: $^

clean-all :
	@echo Cleaning up directory
	@rm -f ImageBufferMacros.h *.o $(PROGRAM_NAME)
