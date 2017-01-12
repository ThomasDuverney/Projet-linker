CC=gcc
CFLAGS=-Wall -Werror

etape6: etape6.o fonctionUtile.o phase1.o
	$(CC) -o etape6 etape6.o fonctionUtile.o phase1.o
	
fonctionUtile.o: fonctionUtile.c fonctionUtile.h
	$(CC) $(CFLAGS) -c fonctionUtile.c
	
etape6.o: etape6.c
	$(CC) $(CFLAGS) -c etape6.c fonctionUtile.h
	
phase1.o: phase1.c 
	$(CC) $(CFLAGS) -c phase1.c	
	
clean:
	rm *.o
	rm *.h.gch
		
reload:
	cp ../elf_linker-1.0/Examples_fusion/file1.o .
	cp ../elf_linker-1.0/Examples_fusion/file2.o .
