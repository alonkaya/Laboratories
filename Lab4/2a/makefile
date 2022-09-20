all: task0
task0: start.o flame2.o util.o link

start.o: lab4_start.s
	nasm -f elf32 lab4_start.s -o start.o
flame2.o:flame2.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector flame2.c -o flame2.o
util.o:util.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o
link:start.o flame2.o util.o
	ld -m elf_i386 start.o flame2.o util.o -o flame2
.PHONY: clean 
clean:
	rm -f *.o flame2
