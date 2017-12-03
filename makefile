CC=gcc
CFLAGS=-I.
DEPS= help.h
OBJ = main.c help.c

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

project4make: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o project4make
