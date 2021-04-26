#Created with Makefile Creator

CC=gcc
CFLAGS=-g -Wall

OBJECTS= cache.o expedition.o linked_list.o main.o net.o registration.o user_interface.o 

all: ndn

ndn: $(OBJECTS)
	$(CC) $(CFLAGS) -o ndn $(OBJECTS)

cache.o: cache.c cache.h definition.h

expedition.o: expedition.c expedition.h net.h definition.h

linked_list.o: linked_list.c linked_list.h

main.o: main.c user_interface.h definition.h registration.h net.h \
 expedition.h linked_list.h

net.o: net.c net.h definition.h expedition.h registration.h linked_list.h \
 cache.h

registration.o: registration.c registration.h definition.h

user_interface.o: user_interface.c user_interface.h definition.h


clean:
	rm -f ndn *.o temp