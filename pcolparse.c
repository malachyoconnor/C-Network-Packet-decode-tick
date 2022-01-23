#include "pcolparse.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


result_store get_log_attributes(const char* message) {
    int source_ip;
    int destination_ip;
    int first_ip_packet_len;
    int first_ip_header_len;
    int first_tcp_header_len;
    int num_ip_packets;

    FILE* fp;
    fp = fopen(message, "rb");

    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    rewind(fp);
    unsigned char* file_store = (unsigned char *) malloc(file_size);
    fread(file_store, sizeof(unsigned char), file_size, fp);

    memcpy(&source_ip, &file_store[13], 4);
    memcpy(&destination_ip, &file_store[17], 4);
    first_ip_packet_len = file_store[0 + 2]<<8 | file_store[0 + 3];
    first_ip_header_len = (file_store[0] & 15); // Get the first 4 bits
    first_tcp_header_len = file_store[first_ip_header_len * 4 + 12]>>4;
    
    unsigned int index = 0;
    num_ip_packets=0;
    while (index < file_size) {
        num_ip_packets++;
        index += file_store[index + 2]<<8 | file_store[index + 3]; //IP Packet length
    }
    free(file_store);
    return (result_store) {source_ip, destination_ip, first_ip_packet_len, first_ip_header_len, first_tcp_header_len, num_ip_packets};
}
