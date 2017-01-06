CC=gcc
CFLAGS=-Wall -Werror

myReadElf: myReadElf.o fonctionUtile.o phase1.o
	$(CC) -o myReadElf myReadElf.o fonctionUtile.o phase1.o
	
fonctionUtile.o: fonctionUtile.c 
	$(CC) $(CFLAGS) -c fonctionUtile.c

	
phase1.o: phase1.c 
	$(CC) $(CFLAGS) -c phase1.c
	
		
myReadElf.o: myReadElf.c
	$(CC) $(CFLAGS) -c myReadElf.c fonctionUtile.h phase1.h
	
clean:
	rm *.o
	rm *.h.gch
	
