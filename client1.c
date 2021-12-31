// mro31  watermark=d62468a98d952d931fe296c1d940d016
/* client.c */
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#define BUFSIZE 512

int main(int argc, char *argv[]) {

  int sockfd;
  struct sockaddr_in servaddr;
  
  if (argc != 3) {
    puts("Usage: client <host> <port>");
    return 1;
  }

  if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    perror("Cannot create socket");
    return 2;
  }
  // Set server to all zeroes
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = inet_addr(argv[1]);
  servaddr.sin_port = htons(atoi(argv[2]));

  if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
    perror("Cannot connect to server");
    return 3;
  }

  {
    int n;
    char bytes[BUFSIZE-1];
    while((n = read(sockfd, bytes, BUFSIZE)) > 0) {
      // Output the results to stdout
      fwrite(bytes, n, sizeof(char), stdout);
    }
  }

  return 1;
}
// mro31  watermark=d62468a98d952d931fe296c1d940d016
