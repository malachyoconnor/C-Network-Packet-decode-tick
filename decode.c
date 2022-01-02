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

void reverseEndianness(const int size, void const * const ptr) {

    unsigned char *b = (unsigned char*) ptr;
    unsigned char result[size];

    for (int char_num=0; char_num < size; char_num++) {
        result[size - char_num - 1] = b[char_num];
    }
    memcpy(b, result, size);
}    

void testReverseEndianness(const int size, void const * const ptr) {

    unsigned char *b = (unsigned char*) ptr;
    unsigned char result[size];

    for (int word = 0; word < size / 4; word++) {
        
        for (int char_ind = 0; char_ind < 4; char_ind++)

            result[4 * word + 3 - char_ind] = b[4 * word + char_ind];
    }

    memcpy(b, result, size);
}    

typedef struct {
    unsigned short source_port;
    unsigned short destination_port;

    unsigned int sequence_number;
    
    unsigned int acknowledgement_number;
     
    unsigned int unneeded1;

    unsigned int unneeded2;

    unsigned int unneeded3;
 
    unsigned short checksum;
    unsigned short urgent_pointer;
 
    unsigned int options : 24;
    unsigned char final_padding;

    unsigned int data;
    // ensures extra padding not added    
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
    // Not using
    unsigned int* options;
    // ensures extra padding not added    
} IP_header;

IP_header populate_ip_header(IP_header* result, const char* input, int length) {


    // Copy the lines of the header we're sure will be present
    memcpy(result, input, 4 * 5);
    printBits(20, input);
    testReverseEndianness(20, result);
    printBits(20, result);


    result->version = 15;
    result->length = 10;
    printBits(sizeof(IP_header), result);

}

int main(int argc, char *argv[]) {

    FILE *fp;
    const char* filename = "message1";

    if ((fp = fopen(filename, "rb")) == 0) {
        perror("Cannot find file read");
        return 1;
    }

    IP_header* ip_ex;
    TCP_header* tcp_ex;

    fseek(fp, 0L, SEEK_END);
    int file_size = ftell(fp);
    rewind(fp);

    unsigned char file_store[10000];
    int r = fread(file_store, file_size, 1, fp);

    unsigned char file_store2[file_size+1];

    for (int i = 0; i < file_size; i++) {
        file_store2[file_size - i -1 ] = file_store[i];
    }

    // 
    
    

    populate_ip_header(ip_ex, file_store2, sizeof(IP_header));

    
    // TODO:
        // We know the ip header is at least 5 32 bit lines
        // so load them first initialize rest to zero
        // Only get everything we need once we get the length
        // okay!



    // fflush(stdout);

    // memcpy(ip_ex, file_store, 24);


    // printf("version: %d\n", ip_ex->version);
    
    // printf("Source ip: ");
    // printIP(ip_ex->source_ip);
    // printf("Destination ip: ");
    // printIP(ip_ex->destination_ip);
    
    // r = fread(tcp_ex, sizeof(struct TCP_header), 1, fp);
    // 
    // printf("source_port: %d\n", tcp_ex->source_port);
    // printf("destination_port: %d\n", tcp_ex->destination_port);


    return 0;
}

