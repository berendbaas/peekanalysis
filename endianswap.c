#include<stdio.h>
#include<string.h>
#include<inttypes.h>

int main(int argc, char** argv) {
	char* number_string = argv[1];
	uintmax_t numeric_val = strtoumax(number_string, NULL, 0);
	uint16_t val = (uint16_t) numeric_val;
	uint16_t val_swap = (val >> 8) | (val << 8);
	printf("%u\n", val_swap);
}
