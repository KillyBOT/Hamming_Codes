#include "hamming.h"

void byte_setBit(byte_t* b, byte_t pos, byte_t val){
	byte_t temp;

	temp = 128;

	temp >>= pos;
	temp ^= 255;
	*b &= temp;

	temp = val ? 128 : 0;
	temp >>= pos;
	*b |= temp;

}
byte_t byte_getBit(byte_t b, byte_t pos) {
	byte_t temp;

	temp = 128;
	temp >>= pos;
	return b & temp ? 1 : 0;
}
void byte_flipBit(byte_t* b, byte_t pos) {
	byte_t temp;

	temp = 128;
	temp >>= pos;
	*b ^= temp;
}
void byte_print(byte_t b) {
	for(int n = 0; n < 8; n++){
		printf("%d",( (b & 128) ? 1 : 0 ));
		b <<= 1;
	}
	printf("\n");
}
byte_t byte_getParity(byte_t b) {
	byte_t final;

	final = 0;

	for(int n = 0; n < 8; n++){
		final ^= (b & 128 ? 1 : 0);
		b <<= 1;
	}

	return final;
}

/*
int72_t* int72_new() {
	int72_t* i = malloc(sizeof(int72_t));

	for(int n = 0; n < 32; n++) i->data[n] = 0;

	return i;
}*/

int72_t int72_genZero(){
	int72_t final;
	for(int n = 0; n < 9; n++) final.data[n] = 0;

	return final;
}
int64_t int64_genRandom(){
	int64_t final;
	int i;

	final = 0;

	i = rand();
	final |= i;
	final <<= 32;
	i = rand();
	final |= i;

	return final;
}
int72_t int72_genRandom(){
	int72_t final;
	for(int n = 0; n < 9; n++) final.data[n] = (byte_t)(rand() % 256);

	return final;
}

int64_t int64_genFromInt72(int72_t i) {
	int64_t final = 0;
	int pos, acc;

	pos = acc = 0;

	for(int n = 0; n < 9; n++){
		for(int m = 0; m < 8; m++){

			if(pos != 0 &&
				pos != 1 &&
				pos != 2 &&
				pos != 4 &&
				pos != 8 &&
				pos != 16 &&
				pos != 32 &&
				pos != 64)
			{
				final |= (int72_getBit(i,pos) ? 1 : 0);
				if(acc < 63)final <<= 1;
				acc++;
			}

			pos++;
		}
	}

	return final;
}

int72_t int72_genFromInt64(int64_t i) {
	int72_t final = int72_genZero();

	int pos;

	pos = 0;

	for(int n = 0; n < 9; n++){
		for(int m = 0; m < 8; m++){
			if(pos != 0 &&
				pos != 1 &&
				pos != 2 &&
				pos != 4 &&
				pos != 8 &&
				pos != 16 &&
				pos != 32 &&
				pos != 64)
			{
				byte_setBit(&(final.data[n]),m,(i & 0x8000000000000000LL) ? 1 : 0);
				i <<= 1;
			}
			pos++;
		}
	}

	return final;
}
void int72_setBit(int72_t* i, byte_t pos, byte_t val){
	byte_setBit(&(i->data[pos / 8]), pos % 8, val);
}
byte_t int72_getBit(int72_t i, byte_t pos) {
	return byte_getBit(i.data[(int)pos / 8], pos % 8);
}
void int72_flipBit(int72_t* i, byte_t pos) {
	byte_flipBit(&(i->data[pos / 8]), pos % 8);
}

void int64_printBinary(int64_t i) {
	for(int pos = 0; pos < 64; pos++){
		printf("%d",( (i & 0x8000000000000000LL) ? 1 : 0 ));
		i <<= 1;
	}
	printf("\n");
}
void int72_printBinary(int72_t i) {
	byte_t temp;
	for(int n = 0; n < 9; n++){
		temp = i.data[n];
		for(int pos = 0; pos < 8; pos++){
			printf("%d",( (temp & 128) ? 1 : 0 ));
			temp <<= 1;
		}
		//printf(" ");
	}
	printf("\n");
}

void int72_printHex(int72_t i) {
	for(int n = 0; n < 9; n++){
		printf("%.2X ", i.data[n]);
	}
	printf("\n");
}
void int72_printDecimal(int72_t i) {
	for(int n = 0; n < 9; n++){
		printf("%d ", i.data[n]);
	}
	printf("\n");
}

byte_t int72_getParity(int72_t i) {
	byte_t temp, final;

	final = 0;

	for(int n = 0; n < 9; n++){
		temp = i.data[n];
		for(int pos = 0; pos < 8; pos++){

			final ^= ((temp & 128) ? 1 : 0);
			temp <<= 1;
		}
	}

	return final;
}
byte_t hamming_syndrome(int72_t i) {
	byte_t temp, final, acc;

	final = acc = 0;

	for(int n = 0; n < 9; n++){
		temp = i.data[n];
		for(int pos = 0; pos < 8; pos++){
			if(temp & 128) final ^= acc;

			temp <<= 1;
			acc++;
		}
	}

	return final;
}

int72_t hamming_encode(int64_t data){
	int72_t final;
	byte_t check;

	final = int72_genFromInt64(data);
	check = hamming_syndrome(final);

	for(int n = 1; n < 128; n*=2){
		if(check & 1) int72_flipBit(&final,n);
		check >>= 1;
	}


	if(int72_getParity(final))int72_flipBit(&final, 0);
	return final;
}

int64_t hamming_decode(int72_t data){
	byte_t check;
	byte_t parity;

	check = hamming_syndrome(data);
	parity = int72_getParity(data);

	byte_print(check);
	byte_print(parity);

	if(check){
		if(parity) int72_flipBit(&data,check);
		else {
			printf("!!!WARNING!!!\nTwo errors detected! Data recovery not ensured!\nData recieved:\n");
			int72_printHex(data);
		}

	} 

	return int64_genFromInt72(data);
}

int main(){

	srand(time(0));

	int64_t testInt64, finalTestInt64;
	int72_t testInt72;
	byte_t testByte;
	int randTimes = rand() % 3;
	printf("Number of random bits flipped (some flips may be on the same bit):\n%d\n",randTimes);

	printf("Original data:\n");
	testInt64 = int64_genRandom();
	int64_printBinary(testInt64);

	testInt72 = hamming_encode(testInt64);

	for(int n = 0; n < randTimes; n++) int72_flipBit(&testInt72,rand() % 72);

	finalTestInt64 = hamming_decode(testInt72);

	printf("Final data:\n");
	int64_printBinary(finalTestInt64);

	printf("Are the two pieces of data the same?\n");
	if(testInt64 == finalTestInt64) printf("Yes\n");
	else printf("No\n");

	return 0;
}