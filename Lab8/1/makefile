all: myElf
myElf: main.o
	gcc -m32 -g -Wall -o myElf main.o
main.o:main.c
	gcc -g -Wall -m32 -c -o main.o main.c
.PHONY: clean 
clean:
	rm -f *.o myElf
