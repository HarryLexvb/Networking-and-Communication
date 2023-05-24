//client code 
#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[])
{
    struct sockaddr_in stSockAddr; // Socket address
    int Res; // Result of the function
    int SocketFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // Socket file descriptor
    int n; // Number of bytes sent/received
    char buffer[256]; // Buffer for the message

    if (-1 == SocketFD)
    {
        perror("cannot create socket"); // Print error message
        exit(EXIT_FAILURE); // Exit the program
    }

    memset(&stSockAddr, 0, sizeof(struct sockaddr_in)); // Clear the structure

    stSockAddr.sin_family = AF_INET; // Set the address family
    stSockAddr.sin_port = htons(1100); // Set the port number
    Res = inet_pton(AF_INET, "127.0.0.1", &stSockAddr.sin_addr); // Set the IP address

    if (0 > Res) // Check if the address is valid
    {
        perror("error: first parameter is not a valid address family"); // Print error message
        close(SocketFD); // Close the socket file descriptor
        exit(EXIT_FAILURE); // Exit the program
    }
    else if (0 == Res)
    {
        perror("char string (second parameter does not contain valid ipaddress"); // Print error message
        close(SocketFD); // Close the socket file descriptor
        exit(EXIT_FAILURE); // Exit the program
    }

    if (-1 == connect(SocketFD, (const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)))  // Connect to the server
    {
        perror("connect failed"); // Print error message
        close(SocketFD); // Close the socket file descriptor
        exit(EXIT_FAILURE); // Exit the program
    }

    while (true) // While server doesn't send "bye"
    {
        cout << "Enter message: "; // Prompt the user to enter a message
        bzero(buffer, 256); // Clear the buffer
        fgets(buffer, 255, stdin); // Get the message from the user
        n = send(SocketFD, buffer, strlen(buffer), 0); // Send the message to the server

        if (n < 0) // Check if the message was sent
        {
            perror("ERROR writing to socket"); // Print error message
            break; // Exit the loop
        }

        bzero(buffer, 256); // Clear the buffer
        n = recv(SocketFD, buffer, 255, 0); // Receive the message from the server

        if (n < 0) // Check if the message was received
        {
            perror("ERROR reading from socket"); // Print error message
            break; // Exit the loop
        }

        cout << "Server: " << buffer; // Print the message from the server

        if (strcmp(buffer, "bye\n") == 0) { // Check if the message is "bye"
            cout << "Closing connection..." << endl; // Print message
            break; // Exit the loop
        }
    }

    shutdown(SocketFD, SHUT_RDWR);  // Shutdown the socket
    close(SocketFD); // Close the socket file descriptor
    return 0;
}

