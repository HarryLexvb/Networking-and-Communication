/* Código del Cliente en C++ */

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
    struct sockaddr_in stSockAddr;
    int Res;
    int SocketFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    int n;
    char buffer[256];

    if (-1 == SocketFD)
    {
        perror("cannot create socket");
        exit(EXIT_FAILURE);
    }

    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));

    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(1100);
    Res = inet_pton(AF_INET, "127.0.0.1", &stSockAddr.sin_addr);

    if (0 > Res)
    {
        perror("error: first parameter is not a valid address family");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }
    else if (0 == Res)
    {
        perror("char string (second parameter does not contain valid ipaddress");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }

    if (-1 == connect(SocketFD, (const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)))
    {
        perror("connect failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }

    while (true)
    {
        cout << "Enter message: ";
        bzero(buffer, 256);
        fgets(buffer, 255, stdin);
        n = send(SocketFD, buffer, strlen(buffer), 0);

        if (n < 0)
        {
            perror("ERROR writing to socket");
            break;
        }

        bzero(buffer, 256);
        n = recv(SocketFD, buffer, 255, 0);

        if (n < 0)
        {
            perror("ERROR reading from socket");
            break;
        }

        cout << "Server: " << buffer << endl;

        if (strcmp(buffer, "exit\n") == 0)
        {
            break;
        }
    }

    shutdown(SocketFD, SHUT_RDWR);

    close(SocketFD);
    return 0;
}
