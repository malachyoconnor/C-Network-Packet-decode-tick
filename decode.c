#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFSIZE 256

void printIP(int ip_int) {
    unsigned char ip[4];
    memcpy(ip, &ip_int, 4);
    printf("%u.%u.%u.%u \n", ip[0], ip[1], ip[2], ip[3]);
}

void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;

    for (i = 0; i < size; i++) {
        if (i % 4 == 0) {puts("");}
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
        printf(" ");
    }
    puts("\n");
}

typedef struct {
    unsigned short source_port;
    unsigned short destination_port;

    unsigned int sequence_number;
    
    unsigned int acknowledgement_number;
    
    unsigned int data_offset : 4;
    unsigned int unneeded1 : 12;
    unsigned short window;

    unsigned int unneeded2;

    unsigned int unneeded3;
 
    unsigned short checksum;
    unsigned short urgent_pointer;
 
    unsigned int options : 24;
    unsigned char final_padding;

    unsigned int* data;
} TCP_header;

typedef struct {
    unsigned int version: 4;
    unsigned int length: 4; 
    unsigned char TOS;
    unsigned short packet_length;
    // Not using
    unsigned short identification;
    unsigned short df_mf_fragment_offset;

    unsigned int ttl: 8;
    unsigned int protocol: 8;
    unsigned short header_checksum;
    // Using
    unsigned int source_ip;

    unsigned int destination_ip;
    unsigned int* options;
} IP_header;

unsigned short chars_to_short(short a, short b) {
    return (((short) a)<<8) | b;
}

unsigned short chars_to_int(char a, char b, char c, char d) {
    return (((int) a)<<24)  | (((int) b)<<16)  | (((int) c)<<8) | b;
}

char* read_file(FILE* fp) {
    char* storage = malloc(BUFSIZE);
    int position = 0;

    while (!feof(fp)) {
        if (position != 0) 
            storage = realloc(storage, BUFSIZE + position);

        char buffer[BUFSIZE] = {0};
        int r = fread(buffer, 1, BUFSIZE, fp);

        for (int i=0; i<r; i++) {
            storage[position + i] = buffer[i];
        }
        position += BUFSIZE;
    }
    printf("Number of bytes in file: %i\n", position+BUFSIZE);
    return storage;
}

void populate_ip_header(IP_header* result, const char* input) {


    int x = 257;
    printBits(4, &x);

    printf("%i\n", input[0]);
    printf("%i\n", input[0]>>4);

    result->version = input[0]>>4;
    result->length = input[0];
    // Remember, packet length is the entire length in 8 byte blocks
    result->packet_length = chars_to_short(input[2], input[3]);
    result->source_ip = chars_to_int(input[15], input[16], input[17], input[18]);
    result->destination_ip = chars_to_int(input[19], input[20], input[21], input[22]);
    
    if (result->length > 5) {
        int *buffer = malloc(result->length - 5);
        for (int line = 6; line <= result->length; line++) {
            buffer[line-6] = chars_to_int(input[4*line], input[4*line+1], 
                                          input[4*line+2], input[4*line+3]);
        }
        result->options = buffer;
    }

}

void populate_tcp_header(TCP_header* result, const char* input) {
    result->data_offset = input[12]>>4;
    result->window = chars_to_short(input[14], input[15]);

    if (result->window > 0) {
        result->data = malloc(result->window);
        memcpy(result->data, &input[4 * (result->data_offset)], result->window);
    }
}



int main(int argc, char *argv[]) {

    const char* filename = "message1";
    FILE *fp = fopen(filename, "rb");
    unsigned char* file_store = read_file(fp);

    IP_header ip_ex = {0};
    TCP_header tcp_ex = {0};

    populate_ip_header(&ip_ex, file_store);
    printBits(10*4, &file_store);
    fflush(stdout);
    // populate_tcp_header(&tcp_ex, &file_store[4 * (ip_ex.length)]);
    // printBits(10*4, &file_store[4 * (ip_ex.length)]);


    printf("IP packet length: %i\n", ip_ex.packet_length);
    printf("IP header length: %i\n", ip_ex.length);
    printf("IP header version: %i\n", ip_ex.version);
    
    printf("Size of TCP header: %i\n", tcp_ex.data_offset);
    printf("Amount of data: %i\n", tcp_ex.window/2);
    
    // cant be right
  

    free(file_store);
    return 0;
}

