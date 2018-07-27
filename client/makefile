############the make file to compile the client with all its dependencies

#/* declarations */
#path to the include files
INCLUDES = ../includes

VPATH = ../includes

#object files created after compling
OBJECTS = client.o utility.o

#the compiler to use to compile the cleint
CC = gcc

#the specific compilation flags
CFLAGS = -w -I

#the target to produce after compiling the client
TARGET = client

#/* Compile */

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS)  -o $@

#compile the client.c file
client.o: client.c client.h
	$(CC) -c client.c

#compile the utility.c file
utility.o: ../includes/utility.c ../includes/utility.h
	$(CC) -c ../includes/utility.c  


#/* clean the directrory */
clean:
	-rm *.o
