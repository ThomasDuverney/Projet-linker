CC=gcc
CFLAGS=-Wall -Werror

etape1: etape1.o fonctionUtile.o
	$(CC) -o etape1 etape1.o fonctionUtile.o
	
	
fonctionUtile.o: fonctionUtile.c fonctionUtile.h
	$(CC) $(CFLAGS) -c fonctionUtile.c
	
		
etape1.o: etape1.c
	$(CC) $(CFLAGS) -c etape1.c fonctionUtile.h
	


	
	


