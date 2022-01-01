#include <stdio.h>
#define BUFSIZE 256

struct TCP_header {
    unsigned short source_port;
    unsigned short destination_port;
    unsigned int sequence_number;
    unsigned int acknowledgement_number;
     
    unsigned int data_offset : 4;
    unsigned int reserved : 3;
    unsigned int padding : 1;
    unsigned int control_flags : 9;
 
    unsigned short window_size;
    unsigned short checksum;
    unsigned short urgent_pointer;
 
    unsigned int options : 24;
    unsigned char final_padding;
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

void reverseEndian(int size, void const * const ptr) {
    int elem_sizes[] = {4, 4, 8, 16,  16, 16,  8, 8, 16,  32,  32,  24, 8};
    unsigned int *b = (unsigned int*) ptr;

    
    for (int current_int = 0; current_int < size/4; current_int++) {
        
        unsigned int doubleword = b[current_int];
        unsigned int result = 0, traversed = 0;

        for (int size_index = 0; size_index < sizeof(elem_sizes) / sizeof(elem_sizes[0]); size_index++) {
            // If we've done a 32 bit chunk, go to the next chunk
            if (traversed >= 32) break;
            int elem_size = elem_sizes[size_index];

            for (int bit_index = elem_size-1; bit_index >= 0; bit_index--) {

                int bit = (doubleword >> (traversed + bit_index)) & 1;
                // printf("%i", bit);
                
                result = result | (bit << traversed + elem_size-1-bit_index);
            }
            traversed += elem_size;
            elem_sizes[size_index] = 0;
        
        }
        // puts("");

        b[current_int] = result;

    }
}

int main(int argc, char *argv[]) {

    int test[] = {1,2,3,4};

    FILE *fp;
    const char* filename = "message2";

    if ((fp = fopen(filename, "rb")) == 0) {
        perror("Cannot find file read");
        return 1;
    }

    struct IP_header* try;
    int r;
    r = fread(try, 24, 24, fp);

    printf("%x", try->packet_length);
    puts("");
    printf("%x", try->version);
    puts("");

    reverseEndian(24, try);

    
    printf("%x", try->packet_length);
    puts("");
    printf("%x", try->version);
    puts("");
    
    

    // while (!feof(fp)) {
    //     char bytes[BUFSIZE];
    //     int r;
    //     // Read from the input stream fp into bytes
    //     r = fread(bytes, sizeof(char), BUFSIZE, fp);
    // }
    return 1337;
}

