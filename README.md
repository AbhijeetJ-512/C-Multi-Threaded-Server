# Simple C Client-Server Implementation

This repository contains a simple implementation of a client-server architecture in C. The server can handle multiple client connections simultaneously using threads.

## Features

- Server accepts incoming client connections and handles them in separate threads.
- Client can connect to the server, send messages, and receive responses.

## Additional Features

- **TCP Connection:** The communication between the client and server in this implementation utilizes TCP (Transmission Control Protocol), a reliable and connection-oriented protocol. TCP ensures that data is transmitted reliably and in order between the client and server, making it suitable for applications where data integrity is crucial, such as messaging or file transfer.

## Requirements

- C compiler (e.g., gcc)
- POSIX-compliant operating system (Linux, Unix, macOS)

## Usage
Change Directory
```bash
cd C-Multi-Threaded-Server
```
### Server

1. Compile the server code:
   ```bash
   make server
   ```
2. Run the server with a specified port number:
   ```bash
   ./server <port>
   ```
    Example
    ```bash
    ./server 5555
    ```
### Client

1. Compile the client code:
   ```bash
   make client
   ```
2. Run the server with a specified port number:
   ```bash
   ./client <server_ip> <port>
   ```
    Example
    ```bash
    ./client 127.0.0.1 5555
    ```
After execution 
```bash
make clean
```
### Important Note
- Ensure that the specified port is not blocked by any firewall settings or other network configurations.
- Both the server and client executables need to be running concurrently for communication to occur.
- Make sure to replace `<port>` with the desired port number for both server and client.
- For the client, `<server_ip>` refers to the IP address of the machine where the server is running.
This implementation provides a basic foundation for understanding client-server communication in C. Additional features such as error handling, authentication, and encryption can be added as needed.


### Code Links
- [Server code](https://github.com/AbhijeetJ-512/C-Multi-Threaded-Server/blob/main/server.c)
- [Client code](https://github.com/AbhijeetJ-512/C-Multi-Threaded-Server/blob/main/client.c)
