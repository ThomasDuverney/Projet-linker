CC=gcc
CFLAGS=-Wall -Werror -g

myReadElf: myReadElf.o fonctionUtile.o phase.o
	$(CC) -o myReadElf myReadElf.o fonctionUtile.o phase.o

fonctionUtile.o: fonctionUtile.c
	$(CC) $(CFLAGS) -c fonctionUtile.c


phase.o: phase.c
	$(CC) $(CFLAGS) -c phase.c


myReadElf.o: myReadElf.c
	$(CC) $(CFLAGS) -c myReadElf.c fonctionUtile.h phase.h

clean:
	rm *.o
	rm *.h.gch
	cp ../elf_linker-1.0/Examples_fusion/file1.o .
	cp ../elf_linker-1.0/Examples_fusion/file2.o .

dist:
	@echo "Archivage des sources\n\n"
	tar zcvf Groupe15.tgz *.c *.h Makefile
