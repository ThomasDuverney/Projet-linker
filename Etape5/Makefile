CC=gcc
CFLAGS=-Wall -Werror

etape5: etape5.o fonctionUtile.o
	$(CC) -o etape5 etape5.o fonctionUtile.o
	rm etape5.o
	
fonctionUtile.o: fonctionUtile.c fonctionUtile.h
	$(CC) $(CFLAGS) -c fonctionUtile.c
	
		
etape5.o: etape5.c
	$(CC) $(CFLAGS) -c etape5.c fonctionUtile.h
