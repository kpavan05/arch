CC = gcc
CFLAGS = -g -Wall

default: emulator

emulator: emulator.o instructimp.o  rtype.o instructstr.o utility.o
	$(CC) $(CFLAGS) emulator.o instructimp.o instructstr.o rtype.o utility.o -o  emulator

emulator.o: main.c emulator.h instructimp.h datatype.h utility.h
	$(CC) $(CFLAGS) -c main.c -o emulator.o

instructimp.o: instructimp.c instructimp.h rtype.h instructstr.h datatype.h
	$(CC) $(CFLAGS) -c instructimp.c -o instructimp.o

rtype.o: rtype.c rtype.h datatype.h
	$(CC) $(CFLAGS) -c rtype.c -o rtype.o

instructstr.o: instructstr.c instructstr.h datatype.h 
	$(CC) $(CFLAGS) -c instructstr.c -o instructstr.o

utility.o: utility.c utility.h
	$(CC) $(CFLAGS) -c utility.c -o utility.o


clean:
	rm -f emulator *.o
