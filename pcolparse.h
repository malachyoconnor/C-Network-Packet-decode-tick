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

unsigned short chars_to_short(short a, short b);
unsigned int chars_to_int(char a, char b, char c, char d);
result_store get_log_attributes(char* message);

#endif