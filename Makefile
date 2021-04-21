#Created with Makefile Creator

CC=gcc
CFLAGS=-g -Wall

OBJECTS= main.o net.o registration.o user_interface.o

all: ndn

ndn: $(OBJECTS)
	$(CC) $(CFLAGS) -o ndn $(OBJECTS)

#expedition.o: expedition.c expedition.h

#linked_list.o: linked_list.c linked_list.h

main.o: main.c user_interface.h definition.h registration.h net.h

net.o: net.c net.h definition.h registration.h

registration.o: registration.c registration.h definition.h

user_interface.o: user_interface.c user_interface.h definition.h


clean:
	rm -f ndn *.o temp