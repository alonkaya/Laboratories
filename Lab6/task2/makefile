# All Targets
all: myShell

# Tool invocations
# Executable "hello" depends on the files numbers.o and main.o and add.s.
myShell: shelly.o LineParser.o
	gcc -g -m32 -Wall -o shelly shelly.o LineParser.o

shelly.o: shelly.c
	gcc -g -m32 -Wall -c -o shelly.o shelly.c

LineParser.o: LineParser.c
	gcc -g -m32 -Wall -c -o LineParser.o LineParser.c

#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean:
	rm -f *.o shelly
