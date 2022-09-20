all: task0
task0: start.o main.o util.o link

start.o: lab4_start.s
	nasm -f elf32 lab4_start.s -o start.o
main.o:main2.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector main2.c -o main.o
util.o:util.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o
link:start.o main.o util.o
	ld -m elf_i386 start.o main.o util.o -o task0
.PHONY: clean 
clean:
	rm -f *.o task0
