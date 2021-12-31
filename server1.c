// mro31  watermark=d62468a98d952d931fe296c1d940d016
/* server.c */
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#define MAXOPEN 5
#define BUFSIZE 1024

int main(int argc, char *argv[])
{

    int listenfd, connfd;
    FILE *fp;
    struct sockaddr_in server;

    if (argc != 3)
    {
        puts("Usage: server <port> <file>");
        return 1;
    }

    if ((fp = fopen(argv[2], "rb")) == 0)
    {
        perror("Cannot find file to serve");
        return 2;
    }
    // Sets the server to all zeros
    memset(&server, 0, sizeof(server));
    // Setting the type of address, the address and the port
    // for the server
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    // htons reverses the BYTE order, atoi converts a string to an int
    // "123" -> 123
    server.sin_port = htons(atoi(argv[1]));

    if ((listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        perror("Cannot create server socket");
        return 3;
    }

    if (bind(listenfd, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("Cannot open the interface");
        return 4;
    }

    if (listen(listenfd, MAXOPEN) < 0)
    {
        perror("Cannot listen on the interface");
        return 5;
    }

    while (1)
    {
        // Accepts a new connection from a user
        if ((connfd = accept(listenfd, (struct sockaddr *)NULL, NULL)) < 0)
        {
            perror("Error accepting a client connection");
            return 6;
        }
        
        // While not at the end of the file
        while (!feof(fp))
        {
            char bytes[BUFSIZE];
            int r, w;
            // Read from the input stream fp into bytes
            r = fread(bytes, sizeof(char), BUFSIZE, fp);

            while (w < r)
            {
                // Write to the user, the items in bytes
                int total = write(connfd, bytes, r);
                if (total < 0)
                {
                    perror("Error writing data to client");
                    return 7;
                }
                w += total;
            }
        }
        // Go back to the beginning of the file
        fseek(fp, 0, SEEK_SET);
        // Close the connection to the client
        close(connfd);
    }
    return 0;
}
// mro31  watermark=d62468a98d952d931fe296c1d940d016