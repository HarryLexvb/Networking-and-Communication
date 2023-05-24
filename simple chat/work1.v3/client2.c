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
    int sockfd;
    struct sockaddr_in servaddr;
    FILE *myf = fopen("client_conversation.txt", "a");
    time_t t; 
    struct tm *tm;
    char MY_TIME[100];
    char *tmp; 
    char sendline[100] = "using the port number 1234\n";
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(1234);

    inet_pton(AF_INET, "127.0.0.1", &(servaddr.sin_addr));
    connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    

    printf("\n\nChat started\n\n");
    fputs("\n\nChat started\n\n", myf);

    while(!strstr(str, "bye") && !strstr(sendline, "bye")){
		bzero( str, 100);
		t = time(NULL);    
		tm = localtime(&t);
		strftime(MY_TIME, 100, "\n me (%H:%M) -> ", tm);
		printf("%s", MY_TIME);
		gets(sendline);
		tmp = strcat(MY_TIME, sendline);
		fputs(tmp, myf);
		write(sockfd, sendline, strlen(sendline)+1);
		if(!strstr(str, "bye")){
			strftime(MY_TIME, 100, "\n other user (%H:%M) -> ", tm);
			read(sockfd, str, 100);
			tmp = strcat(MY_TIME, str);
			printf("%s", tmp);
			fputs(tmp, myf);
		}
    }
    printf("\n\nChat ended\n\n");
    printf("Conversation saved in server_conversation.txt\n\n");
    fclose(myf);
  }
