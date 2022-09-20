all:encoder
encoder : encoder.o
	gcc -g -Wall -m32 -o encoder encoder.o
encoder.o : encoder.c
	gcc -g -Wall -m32 -c encoder.c -o encoder.o
clean:
	rm -f encoder encoder.o
