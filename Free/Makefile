CC=gcc
CFLAGS=-Wall -Werror

free: free.o fonctionUtile.o
	$(CC) -o free free.o fonctionUtile.o
	
fonctionUtile.o: fonctionUtile.c fonctionUtile.h
	$(CC) $(CFLAGS) -c fonctionUtile.c
	
free.o: free.c
	$(CC) $(CFLAGS) -c free.c fonctionUtile.h
	
clean:
	rm *.o
	rm *.h.gch
		
reload:
	cp ../elf_linker-1.0/Examples_fusion/file1.o .
	cp ../elf_linker-1.0/Examples_fusion/file2.o .
