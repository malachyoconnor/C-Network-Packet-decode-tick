#include <iostream>
#include <stdio.h>
#include <string.h>
#include "pcolparse.h"

int main() {
    const char* message = "message1";
    result_store result = get_log_attributes(message);


    FILE* fp;
    fp = fopen(message, "rb");

    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    rewind(fp);
    unsigned char* file_store = new unsigned char[file_size];
    fread(file_store, sizeof(unsigned char), file_size, fp);

    printf("%li", file_size);

    unsigned int index = 0, total_tcp_data=0, ip_header_len=0, packet_len = 0, tcp_header_len=0;

    while (index < file_size) {
        packet_len = file_store[index + 2]<<8 | file_store[index + 3];
        ip_header_len = 4 * (file_store[index] & 15);
        tcp_header_len = 4 * (file_store[index + ip_header_len + 12]>>4);

        total_tcp_data += packet_len - ip_header_len - tcp_header_len;
        index += packet_len;
    }
    
   


    unsigned char* tcp_data = new unsigned char[total_tcp_data];

    printf(" total_data: %d ", total_tcp_data);
    fflush(stdout);

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
        printf("packet len: %u\n", packet_len);
        printf("ip header len: %u\n", ip_header_len);
        printf("tcp_header len: %u\n\n", tcp_header_len);

        
        previous_len = total_tcp_data;
    }


    FILE *f = fopen("A_OUTPUT.txt", "wb");
    fwrite(tcp_data, sizeof(char), total_tcp_data, f);
    fclose(f);

    return 0;
}