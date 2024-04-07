#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define BUFLEN 1024
#define MAX_CLIENTS 5

// Structure to hold client information
typedef struct
{
    int client_socket;
    struct sockaddr_in client_address;
} client_info;

// Function to handle client communication
void *handle_client(void *arg)
{
    char buf[BUFLEN];
    int n;
    client_info *client = (client_info *)arg;

    // Receive and send messages
    while ((n = recv(client->client_socket, buf, sizeof(buf), 0)) > 0)
    {
        buf[n] = '\0'; // Null terminate the received data
        printf("Client %s:%d says: %s\n", inet_ntoa(client->client_address.sin_addr), ntohs(client->client_address.sin_port), buf);
        send(client->client_socket, buf, strlen(buf), 0); // Echo back to the client
    }

    // Client disconnected
    printf("Client disconnected: %s:%d\n", inet_ntoa(client->client_address.sin_addr), ntohs(client->client_address.sin_port));
    close(client->client_socket);
    free(client);
    pthread_exit(NULL);
}

int main(int argc, char **argv)
{
    int sd, new_sd, port;
    struct sockaddr_in server;
    pthread_t threads[MAX_CLIENTS];
    int thread_count = 0;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    port = atoi(argv[1]);

    // Create a socket
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    // Fill the server structure
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the address and port
    if (bind(sd, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        perror("bind");
        exit(1);
    }

    // Listen for connections
    if (listen(sd, MAX_CLIENTS) == -1)
    {
        perror("listen");
        exit(1);
    }

    printf("Server listening on port %d...\n", port);

    // Accept connections and handle them in separate threads
    while (1)
    {
        client_info *client = (client_info *)malloc(sizeof(client_info));
        int client_len = sizeof(client->client_address);

        if ((new_sd = accept(sd, (struct sockaddr *)&(client->client_address), (socklen_t *)&client_len)) == -1)
        {
            perror("accept");
            continue;
        }

        client->client_socket = new_sd;

        // Create a new thread to handle the client
        if (pthread_create(&threads[thread_count], NULL, handle_client, (void *)client) != 0)
        {
            perror("pthread_create");
            close(new_sd);
            free(client);
        }
        else
        {
            printf("New client connected: %s:%d\n", inet_ntoa(client->client_address.sin_addr), ntohs(client->client_address.sin_port));
            thread_count++;
        }

        // Prevent exceeding the max number of clients
        if (thread_count >= MAX_CLIENTS)
        {
            printf("Maximum clients reached. No longer accepting new connections.\n");
            break;
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < thread_count; i++)
    {
        pthread_join(threads[i], NULL);
    }

    close(sd);
    return 0;
}
