CC=gcc
CFLAGS=-Wall -Werror

etape3: etape3.o fonctionUtile.o
	$(CC) -o etape3 etape3.o fonctionUtile.o
	rm etape3.o
	
fonctionUtile.o: fonctionUtile.c fonctionUtile.h
	$(CC) $(CFLAGS) -c fonctionUtile.c
	
		
etape3.o: etape3.c
	$(CC) $(CFLAGS) -c etape3.c fonctionUtile.h
