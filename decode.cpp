#include <iostream>
#define BUFSIZE 256

struct TCP_header {
    short source_port;
    short destination_port;
    int sequence_number;
    int acknowledgement_number;
    
    int data_offset : 4;
    int reserved : 3;
    int padding : 1;
    int control_flags : 9;

    short window_size;
    short checksum;
    short urgent_pointer;

    int options : 24;
    char padding;
};

struct IP_header {
    int version: 4;
    int length: 4;
    char TOS;
    unsigned short packet_length;
    // Not using
    short identification;
    short df_mf_fragment_offset;
    int ttl: 4;
    int transport: 4;
    short header_checksum;
    // Using
    int source_ip;
    int destination_ip;
    // Not using
    int options : 12;
    int padding : 4;
    
    // Need pointer to data
};

int main() {

    IP_header test{10, 10, 'a', 30};

    std::cout << sizeof(test);

    FILE *fp;
    const char* filename = "answers.txt";

    if ((fp = fopen(filename, "rb")) == 0) {
        perror("Cannot find file read");
        return 1;
    }

    while (!feof(fp)) {
        char bytes[BUFSIZE];
        int r;
        // Read from the input stream fp into bytes
        r = fread(bytes, sizeof(char), BUFSIZE, fp);

        // std::cout << bytes;
    }

    return 0;
}

