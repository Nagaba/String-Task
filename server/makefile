############the make file to compile the server with all its dependencies

#/* declarations */
#path to the include files
INCLUDES = ../includes
VPATH = ../includes

#object files created after compling
OBJECTS = server.o utility.o

#the compiler to use to compile the server
CC = gcc

#the specific compilation flags
CFLAGS = -w -I

#the target to produce after compiling the server
TARGET = server

#/* Compile */

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS)  -o $@

#compile the server.c file
server.o: server.c server.h
	$(CC) -c server.c

#compile the utility.c file
utility.o: ../includes/utility.c ../includes/utility.h
	$(CC) -c ../includes/utility.c  


#/* clean the directrory (remove the object files we don't need them) */
clean:
	-rm *.o
