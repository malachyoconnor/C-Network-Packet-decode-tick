#include "pcolparse.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_ip(int ip[], int len, const char* text_to_print) {
    printf(text_to_print);
    for (int i = 0; i < len-1; i++)
        printf("%d.", ip[i]);
    printf("%d ", ip[len-1]);
}


int main (int argv, char* argc) {
    result_store result = get_log_attributes("message1");
    print_ip(result.source_ip, 4, "");
    print_ip(result.destination_ip, 4, "");
    printf("%d ", result.first_ip_header_len);
    printf("%d ", result.first_ip_packet_len*4);
    printf("%d ", result.first_tcp_header_len);
    printf("%d ", result.num_ip_packets);
}
