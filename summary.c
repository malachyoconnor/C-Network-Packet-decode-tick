#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pcolparse.h"

void print_ip(int ip[], int len, const char* text_to_print) {
    printf(text_to_print);
    for (int i = 0; i < len-1; i++)
        printf("%d.", ip[i]);
    printf("%d ", ip[len-1]);
}

void print_log_attributes() {
    print_ip(source_ip, 4, "");
    print_ip(destination_ip, 4, "");
    printf("%d ", first_ip_header_len);
    printf("%d ", first_ip_packet_len*4);
    printf("%d ", first_tcp_header_len);
    printf("%d ", num_ip_packets);
}
