CC=gcc
CFLAGS=-Wall -Werror

etape6: etape6.o fonctionUtile.o
	$(CC) -o etape6 etape6.o fonctionUtile.o
	rm etape6.o
	
fonctionUtile.o: fonctionUtile.c fonctionUtile.h
	$(CC) $(CFLAGS) -c fonctionUtile.c
	
		
etape6.o: etape6.c
	$(CC) $(CFLAGS) -c etape6.c fonctionUtile.h
