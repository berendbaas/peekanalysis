#include<stdio.h>
#include<string.h>
#include<inttypes.h>

uint32_t swap_uint32( uint32_t val )
{
    val = ((val << 8) & 0xFF00FF00 ) | ((val >> 8) & 0xFF00FF ); 
    return (val << 16) | (val >> 16);
}


int main(int argc, char** argv) {
    char* number_string = argv[1];
    uintmax_t numeric_val = strtoumax(number_string, NULL, 0);
    uint32_t val = (uint32_t) numeric_val;
    uint32_t val_swap = swap_uint32(val);
    printf("%u\n", val_swap);
}
