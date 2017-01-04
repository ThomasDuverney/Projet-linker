CC=gcc
CFLAGS=-Wall -Werror

etape2: etape2.o fonctionUtile.o
	$(CC) -o etape2 etape2.o fonctionUtile.o
	rm etape2.o
	
fonctionUtile.o: fonctionUtile.c fonctionUtile.h
	$(CC) $(CFLAGS) -c fonctionUtile.c
	
		
etape2.o: etape2.c
	$(CC) $(CFLAGS) -c etape2.c fonctionUtile.h
