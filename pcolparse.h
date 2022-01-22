
#ifndef PCOL_PARSE_H
#define PCOL_PARSE_H

unsigned short chars_to_short(short a, short b);
int source_ip[4];
int destination_ip[4];
int first_ip_packet_len;
int first_ip_header_len;
int first_tcp_header_len;
int num_ip_packets;

#endif