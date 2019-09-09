all: dictionary.o

dictionary.o: dictionary.c dictionary.h
	gcc -c dictionary.c
