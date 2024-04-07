#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFLEN 1024

int main(int argc, char **argv)
{
    int sd, port, n;
    struct sockaddr_in server;
    char buf[BUFLEN];

    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <server_ip> <port>\n", argv[0]);
        exit(1);
    }

    port = atoi(argv[2]);

    // Create a socket
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    // Fill the server structure
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    if (inet_aton(argv[1], &server.sin_addr) == 0)
    {
        fprintf(stderr, "Invalid address\n");
        close(sd);
        exit(1);
    }

    // Connect to the server
    if (connect(sd, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        perror("connect");
        close(sd);
        exit(1);
    }

    printf("Connected to server at %s:%d\n", argv[1], port);

    while (1)
    {
        printf("Enter message: ");
        fgets(buf, sizeof(buf), stdin);
        buf[strcspn(buf, "\n")] = '\0'; // Remove trailing newline character

        // Send message to the server
        if (send(sd, buf, strlen(buf), 0) == -1)
        {
            perror("send");
            break;
        }

        // Receive response from the server
        if ((n = recv(sd, buf, sizeof(buf), 0)) > 0)
        {
            buf[n] = '\0'; // Null-terminate the received data
            printf("Server response: %s\n", buf);
        }
        else
        {
            perror("recv");
            break;
        }
    }

    close(sd);
    return 0;
}
