#include <iostream>
#include <stdio.h>
#include <string.h>
extern "C" {
#include "pcolparse.h"
}

int main(int argc, char** argv) {

    if(argc != 3){
        puts("Usage: extract <log source> <decoded file destination> ");
        return 1;
    }

    FILE* fp;
    if ((fp = fopen(argv[1], "rb")) == 0) {
        perror("Cannot find log file.");
        return 2;
    }


    char* message = argv[1];
    get_log_attributes(message);

    fp = fopen(message, "rb");

    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    rewind(fp);
    unsigned char* file_store = new unsigned char[file_size];
    fread(file_store, sizeof(unsigned char), file_size, fp);

    unsigned int index = 0, total_tcp_data=0, ip_header_len=0, packet_len = 0, tcp_header_len=0;

    while (index < file_size) {
        packet_len = file_store[index + 2]<<8 | file_store[index + 3];
        ip_header_len = 4 * (file_store[index] & 15);
        tcp_header_len = 4 * (file_store[index + ip_header_len + 12]>>4);

        total_tcp_data += packet_len - ip_header_len - tcp_header_len;
        index += packet_len;
    }
    
   
    unsigned char* tcp_data = new unsigned char[total_tcp_data];
    int previous_len = 0;
    index = 0, total_tcp_data=0, ip_header_len=0, packet_len = 0, tcp_header_len=0;

    while (index < file_size) {
        packet_len = file_store[index + 2]<<8 | file_store[index + 3];
        ip_header_len = 4 * (file_store[index] & 15);
        tcp_header_len = 4 * (file_store[index + ip_header_len + 12]>>4);
        total_tcp_data += packet_len - ip_header_len - tcp_header_len;

        if (packet_len - ip_header_len - tcp_header_len != 0)
            memcpy(&tcp_data[previous_len], &file_store[index + ip_header_len + tcp_header_len], packet_len - ip_header_len - tcp_header_len);
        index += packet_len;
        previous_len = total_tcp_data;
    }

    FILE *f = fopen(argv[2], "wb");
    fwrite(tcp_data, sizeof(char), total_tcp_data, f);
    fclose(f);

    delete file_store;
    return 0;
}