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
        for (j = 0; j < 8; j++) {
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
    
    unsigned char data_offset : 4;
    unsigned int unneeded1 : 12;
    unsigned short window;

    unsigned int unneeded2;

    unsigned int unneeded3;
 
    unsigned short checksum;
    unsigned short urgent_pointer;
 
    unsigned int options : 24;
    unsigned char final_padding;

    unsigned int data;
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
    return storage;
}

void populate_ip_header(IP_header* result, const char* input) {

    result->version = input[0]>>4;
    result->length = input[0];
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
}



int main(int argc, char *argv[]) {

    const char* filename = "message4";
    FILE *fp = fopen(filename, "rb");
    unsigned char* file_store = read_file(fp);

    IP_header* ip_ex;
    TCP_header* tcp_ex;

    populate_ip_header(ip_ex, file_store);

    
  

    free(file_store);
    return 0;
}

