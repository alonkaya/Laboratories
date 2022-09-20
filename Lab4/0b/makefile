all: task0
task0: start.o patch.o util.o link

start.o: lab4_start.s
	nasm -f elf32 lab4_start.s -o start.o
patch.o:patch.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector patch.c -o patch.o
util.o:util.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o
link:start.o patch.o util.o
	ld -m elf_i386 start.o patch.o util.o -o patch
.PHONY: clean 
clean:
	rm -f *.o patch
