
all: myShell
	
myShell: task2.o LineParser.o
	gcc -g -m32 -Wall -o task2 task2.o LineParser.o
	
task2.o: task2.c
	gcc -g -m32 -Wall -c -o task2.o task2.c
	
LineParser.o: LineParser.c
	gcc -g -m32 -Wall -c -o LineParser.o LineParser.c
	
	
clean:
	rm -f *.o task2
