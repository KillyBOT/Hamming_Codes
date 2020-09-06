CC = gcc

all: hamming.o
	$(CC) -o HammingCodes hamming.o -lm

hamming.o: hamming.c hamming.h
	$(CC) -c hamming.c -lm