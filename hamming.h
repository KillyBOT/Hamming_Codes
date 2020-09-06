#ifndef HAMMING_H
#define HAMMING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

typedef unsigned char byte_t;
typedef struct {
	byte_t data[9];
} int72_t;

void byte_setBit(byte_t* b, byte_t pos, byte_t val);
byte_t byte_getBit(byte_t b, byte_t pos);
void byte_flipBit(byte_t* b, byte_t pos);

void byte_print(byte_t b);

byte_t byte_getParity(byte_t b);

int72_t int72_genZero();
int64_t int64_genRandom();
int72_t int72_genRandom();
int64_t int64_genFromInt72(int72_t i);
int72_t int72_genFromInt64(int64_t i);
void int72_setBit(int72_t* i, byte_t pos, byte_t val);
byte_t int72_getBit(int72_t i, byte_t pos);
void int72_flipBit(int72_t* i, byte_t pos);

void int64_printBinary(int64_t i);
void int72_printBinary(int72_t i);
void int72_printHex(int72_t i);
void int72_printDecimal(int72_t i);

byte_t int72_getParity(int72_t i);
byte_t hamming_syndrome(int72_t i);
int72_t hamming_encode(int64_t data);
int64_t hamming_decode(int72_t data);

#endif