#include <iostream>
#include "pcolparse.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    const char* message = "message1";
    result_store result = get_log_attributes(message);
    printf("%d ", result.first_ip_header_len);
    printf("%d ", result.first_ip_packet_len*4);
    printf("%d ", result.first_tcp_header_len);
    printf("%d ", result.num_ip_packets);
    return 0;
}