all : echo
echo : echo.o
	gcc -Wall -o echo echo.o
echo.o : echo.c
	gcc -Wall -c echo.c -o echo.o

clean :
	rm -f echo echo.o
