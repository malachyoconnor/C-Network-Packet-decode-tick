#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pcolparse.h"

unsigned short chars_to_short(short a, short b) {
    return (((short) a)<<8) | b;
}

void get_log_attributes(char* message) {

    FILE* fp;
    fp = fopen(message, "rb");

    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    rewind(fp);
    unsigned char file_store[file_size];
    unsigned long r = fread(file_store, sizeof(unsigned char), file_size, fp);
    
    int source_ip[4];
    int destination_ip[4];
    int first_ip_packet_len;
    int first_ip_header_len;
    int first_tcp_header_len;
    int num_ip_packets=0;

    for (int i=0; i<4; i++) {
        source_ip[i] = file_store[13+i];
        destination_ip[i] = file_store[17+i];
    }

    first_ip_packet_len = chars_to_short(file_store[2], file_store[3]);
    first_ip_header_len = (file_store[0] & 15); // Get the first 4 bits
    first_tcp_header_len = file_store[first_ip_header_len * 4 + 12]>>4;
                                
    short index = 0;
    while (index < file_size) {
        num_ip_packets ++;
        index += chars_to_short(file_store[index + 2] , file_store[index + 3]); //IP Packet length
    }
}
