CFLAGS=-Wall -I..
CC=gcc

attacks: purge *.c
	$(CC) $(CFLAGS) -fPIC -c *.c
	for i in *.o; do $(CC) -shared -o lib$${i%*.o}.so $$i; done

clean:
	rm -f *.o

purge: clean
	rm -f *.so