CC = gcc
CFLAGS = -g -Wall

default: assembler

assembler: assembler.o instruction.o instrlist.o table.o utilities.o
	$(CC) $(CFLAGS) assembler.o instruction.o instrlist.o table.o utilities.o -o  assembler

assembler.o: assembler.c assembler.h instruction.h instrlist.h table.h utilities.h constant.h
	$(CC) $(CFLAGS) -c assembler.c -o assembler.o

instrlist.o: instrlist.c instrlist.h instruction.h
	$(CC) $(CFLAGS) -c instrlist.c -o instrlist.o

instruction.o: instruction.c instruction.h utilities.h constant.h
	$(CC) $(CFLAGS) -c instruction.c -o instruction.o

table.o: table.c table.h utilities.h constant.h
	$(CC) $(CFLAGS) -c table.c -o table.o

utilities.o: utilities.c utilities.h constant.h
	$(CC) $(CFLAGS) -c utilities.c -o utilities.o


clean:
	rm -f assembler *.o
