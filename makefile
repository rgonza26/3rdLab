make: main.c scanner.c print.c
	gcc -o main.o main.c
	gcc -o scanner.o scanner.c
	gcc -o print.o print.c
	gcc -o lab3 main.o scanner.o print.o
	rm *.o