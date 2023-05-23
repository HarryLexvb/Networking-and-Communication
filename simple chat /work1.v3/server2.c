#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>

void main(void){
    char str[100];
    int listen_fd, comm_fd;
    struct sockaddr_in servaddr;
    FILE *myf = fopen("server_conversation.txt", "a");
    time_t t;
    struct tm *tm;
    char MY_TIME[100];
    char *tmp;
    char sendline[100] = "using the port number 1234\n";

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    bzero( &servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(1234);

    bind(listen_fd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    listen(listen_fd, 10);
    comm_fd = accept(listen_fd, (struct sockaddr*) NULL, NULL);

    printf("\n\nChat started\n\n");
    fputs("\n\nchat started\n\n", myf);

    while(!strstr(str, "bye") && !strstr(sendline, "bye")){
        bzero( str, 100);
        t = time(NULL);
        tm = localtime(&t);
        strftime(MY_TIME, 100, "\n other user (%H:%M) -> ", tm);

        read(comm_fd, str, 100);
        tmp = strcat(MY_TIME, str);
        printf("%s", tmp);
        fputs(tmp, myf);
        if(!strstr(str, "bye")){
            strftime(MY_TIME, 100, "\n me (%H:%M) -> ", tm);
            printf("%s", MY_TIME);
            gets(sendline);
            tmp = strcat(MY_TIME, sendline);
            write(comm_fd, sendline, strlen(sendline)+1);
            fputs(tmp, myf);
        }
    }

    printf("\n\nchat ended\n\n");
    printf("conversation saved in server_conversation.txt\n\n");
    fclose(myf);
}
