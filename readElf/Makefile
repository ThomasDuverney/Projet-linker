CC=gcc
CFLAGS=-Wall -Werror -g

myReadElf: myReadElf.o fonctionUtile.o phase.o
	$(CC) -o myReadElf myReadElf.o fonctionUtile.o phase.o

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

fonctionUtile.o : fonctionUtile.h	
phase.o : phase.h fonctionUtile.h
myReadElf.o : phase.h fonctionUtile.h

clean:
	rm phase.o
	rm fonctionUtile.o
	rm myReadElf.o
	
dist:
	@echo "Archivage des sources\n\n"
	tar zcvf Groupe15.tgz *.c *.h *.o oMakefile
