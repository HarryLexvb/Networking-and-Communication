//server code 
#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

int main(void)
{
    struct sockaddr_in stSockAddr; // Socket address
    struct sockaddr_in cli_addr;  // Client address
    int client; // Client size
    int SocketFD; // Socket file descriptor
    char buffer[256]; // Buffer
    int n; // Number of bytes

    if ((SocketFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)   // Create socket
    {
        perror("Socket"); // Error
        exit(1); // Exit
    }

    if (setsockopt(SocketFD, SOL_SOCKET, SO_REUSEADDR, "1", sizeof(int)) == -1) // Set socket options
    {
        perror("Setsockopt"); // Error
        exit(1); // Exit 
    }

    memset(&stSockAddr, 0, sizeof(struct sockaddr_in)); // Clear socket address

    stSockAddr.sin_family = AF_INET; // Set socket family to IPv4
    stSockAddr.sin_port = htons(1100); // Set socket port to 1100
    stSockAddr.sin_addr.s_addr = INADDR_ANY; // Set socket address to any

    if (bind(SocketFD, (struct sockaddr *)&stSockAddr, sizeof(struct sockaddr)) == -1) // Bind socket
    {
        perror("Unable to bind"); // Error
        exit(1); // Exit
    }

    if (listen(SocketFD, 5) == -1)  // Listen for connections
    {
        perror("Listen");   // Error
        exit(1);    // Exit
    }

    cout << "Server started. Waiting for connections..." << endl;   // Server started

    client = sizeof(struct sockaddr_in);    // Client size

    int ConnectFD = accept(SocketFD, (struct sockaddr *)&cli_addr, (socklen_t *)&client);   // Accept connection

    while (true) // While client doesn't send "bye"
    {
        bzero(buffer, 256); // Clear buffer
        n = recv(ConnectFD, buffer, 255, 0);    // Receive message

        if (n < 0)  // if number of bytes is less than 0
        {
            perror("ERROR reading from socket");    // Error
            break;  // Exit
        }

        cout << "Client: " << buffer;   // Print message

        if (strcmp(buffer, "bye\n") == 0)   // If client sends "bye"
        {
            cout << "Closing connection..." << endl;
            break;
        }

        cout << "Enter message: ";   // Enter message
        bzero(buffer, 256); // Clear buffer
        fgets(buffer, 255, stdin); // Get message

        if (strcmp(buffer, "bye\n") == 0)  // If client sends "bye"
        {
            cout << "Closing connection..." << endl; // Close connection
            n = send(ConnectFD, buffer, strlen(buffer), 0); // Send message
            break;
        }

        n = send(ConnectFD, buffer, strlen(buffer), 0); // Send message

        if (n < 0)
        {
            perror("ERROR writing to socket");  // Error
            break;  // Exit
        }
    }

    close(ConnectFD);   // Close connection
    close(SocketFD);    // Close socket
    return 0;
}

