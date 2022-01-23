#ifndef TEST
#define TEST

typedef struct Output {
    int source_ip;
    int destination_ip;
    int first_ip_packet_len;
    int first_ip_header_len;
    int first_tcp_header_len;
    int num_ip_packets;
} result_store;

result_store get_log_attributes(const char* message);

#endif