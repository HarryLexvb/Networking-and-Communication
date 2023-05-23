/* Código del Servidor en C++ */

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
    struct sockaddr_in stSockAddr;
    struct sockaddr_in cli_addr;
    int client;
    int SocketFD;
    char buffer[256];
    int n;

    if ((SocketFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
    {
        perror("Socket");
        exit(1);
    }

    if (setsockopt(SocketFD, SOL_SOCKET, SO_REUSEADDR, "1", sizeof(int)) == -1)
    {
        perror("Setsockopt");
        exit(1);
    }

    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));

    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(1100);
    stSockAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(SocketFD, (struct sockaddr *)&stSockAddr, sizeof(struct sockaddr)) == -1)
    {
        perror("Unable to bind");
        exit(1);
    }

    if (listen(SocketFD, 5) == -1)
    {
        perror("Listen");
        exit(1);
    }

    cout << "Server started. Waiting for connections..." << endl;

    client = sizeof(struct sockaddr_in);

    int ConnectFD = accept(SocketFD, (struct sockaddr *)&cli_addr, (socklen_t *)&client);

    while (true)
    {
        bzero(buffer, 256);
        n = recv(ConnectFD, buffer, 255, 0);

        if (n < 0)
        {
            perror("ERROR reading from socket");
            break;
        }

        cout << "Client: " << buffer;

        if (strcmp(buffer, "exit\n") == 0)
        {
            break;
        }

        cout << "Enter message: ";
        bzero(buffer, 256);
        fgets(buffer, 255, stdin);
        n = send(ConnectFD, buffer, strlen(buffer), 0);

        if (n < 0)
        {
            perror("ERROR writing to socket");
            break;
        }
    }

    close(ConnectFD);
    close(SocketFD);
    return 0;
}
