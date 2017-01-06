CC=gcc
CFLAGS=-Wall 

etape3: etape3.o fonctionUtile.o
	$(CC) -o etape4 etape4.o fonctionUtile.o
	rm etape4.o
	
fonctionUtile.o: fonctionUtile.c fonctionUtile.h
	$(CC) $(CFLAGS) -c fonctionUtile.c
	
		
etape3.o: etape4.c
	$(CC) $(CFLAGS) -c etape4.c fonctionUtile.h
