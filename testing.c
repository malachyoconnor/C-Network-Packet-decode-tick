#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>


int main () {
    int x = htons(10);
    printf("%i", x);
    printf("%i", htons(x));

    return 0;
}


// 0000 0000 0000 1010
// 0000 1010 0000 0000