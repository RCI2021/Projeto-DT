#NDN


 CC=gcc

 CFLAGS = -g -Wall -std=c11

 OBJECTS = main.o user_interface.o registration.o

 ndn: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

 main.o: main.c user_interface.h registration.h

 user_interface.o: user_interface.c user_interface.h registration.h

 registration.o: registration.c registration.h

 clean:
	rm -f *.o ndn temp
	make
