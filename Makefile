CONFIGS = -Wall -Wextra -std=c99

all: print.o ls.o ls-program

clean: 
	rm print.o ls-program ls.o

ls-program: main.c ls.o print.o ls.h
	gcc -o ls-program main.c print.o ls.o -g $(CONFIGS)

ls.o: ls.c 
	gcc -o ls.o ls.c -c $(CONFIGS)  

print.o: print.c 
	gcc -o print.o print.c -c $(CONFIGS)
