#include <stdio.h>
#include <string.h>
#define BUFSIZE 256

struct TCP_header {
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
};

struct IP_header {
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
    unsigned int options : 24;
    unsigned int padding : 8;
    
    // Need pointer to data
};

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
    }
    puts("\n");
}

void reverseEndianness(const int size, void const * const ptr) {

    unsigned char *b = (unsigned char*) ptr;
    unsigned char result[size];
    unsigned char byte;

    for (int char_num=0; char_num < size; char_num++) {
        result[size - char_num - 1] = b[char_num];
    }
    memcpy(b, result, size);
}    

void printIP(int ip_int) {
    unsigned char ip[4];
    memcpy(ip, &ip_int, 4);
    printf("%u.%u.%u.%u \n", ip[0], ip[1], ip[2], ip[3]);
}

int main(int argc, char *argv[]) {

    FILE *fp;
    const char* filename = "message1";

    if ((fp = fopen(filename, "rb")) == 0) {
        perror("Cannot find file read");
        return 1;
    }
    struct TCP_header* tcp_ex;
    struct IP_header* ip_ex;
    int r;
    r = fread(ip_ex, sizeof(struct IP_header), 1, fp);
    reverseEndianness(sizeof(struct IP_header), ip_ex);

    printf("length: %d\n", ip_ex->length);
    printf("version: %d\n", ip_ex->version);
    
    printf("Source ip: ");
    printIP(ip_ex->source_ip);
    printf("Destination ip: ");
    printIP(ip_ex->destination_ip);
    
    r = 0;
    r = fread(tcp_ex, sizeof(struct TCP_header), 1, fp);
    // 
    // printf("source_port: %d\n", tcp_ex->source_port);
    // printf("destination_port: %d\n", tcp_ex->destination_port);


    
    return 0;
}

