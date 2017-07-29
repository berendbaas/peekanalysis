#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<inttypes.h>

void print_ip(unsigned int ip)
{
    unsigned char bytes[4];
    bytes[0] = ip & 0xFF;
    bytes[1] = (ip >> 8) & 0xFF;
    bytes[2] = (ip >> 16) & 0xFF;
    bytes[3] = (ip >> 24) & 0xFF;   
    printf("%d.%d.%d.%d\n", bytes[3], bytes[2], bytes[1], bytes[0]);        
}

int main(int argc, char** argv) {
    // Assume correct arg count for now argc > 1
    char* number_string = argv[1];
    uintmax_t numeric_val = strtoumax(number_string, NULL, 0);
    print_ip(numeric_val);
}
