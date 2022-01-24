#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pcolparse.h"

void print_ip(int* ip, int len, const char* text_to_print) {
    printf(text_to_print);
    unsigned char* to_search = (unsigned char*) ip;
    for (int i = 0; i < len-1; i++)
        printf("%u.", to_search[i]);
    printf("%d ", to_search[len-1]);
}


int main (int argc, char** argv) {
    if(argc != 2){
        puts("Usage: summary <file>");
        return 1;
    }

    result_store result = get_log_attributes(argv[1]);
    print_ip(&result.source_ip, 4, "");
    print_ip(&result.destination_ip, 4, "");
    printf("%d ", result.first_ip_header_len);
    printf("%d ", result.first_ip_packet_len*4);
    printf("%d ", result.first_tcp_header_len);
    printf("%d ", result.num_ip_packets);
}
