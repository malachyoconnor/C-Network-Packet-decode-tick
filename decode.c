#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned short chars_to_short(short a, short b) {
    return (((short) a)<<8) | b;
}
unsigned int chars_to_int(char a, char b, char c, char d) {
    return (((int) a)<<24)  | (((int) b)<<16)  | (((int) c)<<8) | (int) b;
}


int main(int argc, char *argv[]) {

    // const char* filename = "message1";
    FILE* fp;
    fp = fopen("message1", "rb");

    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    rewind(fp);
    unsigned char file_store[file_size];
    unsigned long r = fread(file_store, sizeof(unsigned char), file_size, fp);
    
    int ource_ip = chars_to_int(file_store[13+0], file_store[13+1], file_store[13+2], file_store[13+3]);
    printf("\n%u\n", ource_ip);

    printf("Source IP Address: ");
    for (int i=13; i<13+4; i++) {
        printf("%i.", file_store[i]);
    }
    printf("\nDestination IP Address: ");
    for (int i=17; i<17+4; i++) {
        printf("%i.", file_store[i]);
    }

    short FirstIPPacketSize = chars_to_short(file_store[2], file_store[3]);
    char FirstIPHeaderSize = (file_store[0] & 15); // Get the first 4 bits
    int FirstTCPPacketHeaderSize = file_store[FirstIPHeaderSize * 4 + 12]>>4;
                                
    printf("\nFirst IP Header Size: %d.", FirstIPHeaderSize);
    printf("\nFirst IP Packet Size: %d.", FirstIPPacketSize);
    printf("\nFirst TCP Header Size: %d.", FirstTCPPacketHeaderSize);

    short index = 0, NumberIPPackets = 0;
    while (index < file_size) {
        NumberIPPackets ++;
        index += chars_to_short(file_store[index + 2] , file_store[index + 3]); //IP Packet length
    }
    printf("\nNumber of IP Packets: %d.", NumberIPPackets);
  

    return 0;
}

