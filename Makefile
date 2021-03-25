#Top level Makefile by António Vidais e Duarte Costa for the 2021 RCI Project

#Source Files
C_SOURCE=$(wildcard *.c)

#Header Files
H_SOURCE=$(wildcard *.h)

#Object Files
OBJ=$(C_SOURCE:.c=.o)

#Compiler
CC=gcc
DB=gdb

#flags for debugging
DB_FLAGS= -c -W -Wall -ansi -pedantic

#flags for Release
RE_FLAGS= -O3 -Wall -o ndn

all: $(OBJ)
	$(CC) $(RE_FLAGS) -O $@ $(OBJ)


#INPUT FILES WITH DEPENDENCIES

clean:
	rm -f *.o ndn temp

