all: task0
task0: start.o count.o util.o link

start.o: lab4_start.s
	nasm -f elf32 lab4_start.s -o start.o
count.o:count.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector count.c -o count.o
util.o:util.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o
link:start.o count.o util.o
	ld -m elf_i386 start.o count.o util.o -o count
.PHONY: clean 
clean:
	rm -f *.o count
