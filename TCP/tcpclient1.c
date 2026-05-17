#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#define PORT 8080

int main()
{
    int sockfd;
    char buff[1024];
    char msg[1024];
    struct sockaddr_in address;

    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0)
    {
        perror("socket");
        exit(1);
    }

    address.sin_family=AF_INET;
    address.sin_port=htons(PORT);
    inet_pton(AF_INET,"127.0.0.1",&address.sin_addr);

    connect(sockfd,(struct sockaddr*)&address,sizeof(address));
    printf("Connected to server...\n");

    while(1)
    {
        printf("Client: ");
        fgets(msg,sizeof(msg),stdin);
        send(sockfd,msg,strlen(msg),0);
        if(strncmp(msg,"exit",4)==0)
        {
            printf("Connection terminated...\n");
            break;
        }
        memset(buff,0,sizeof(buff));
        read(sockfd,buff,sizeof(buff));
        printf("Server:%s",buff);
        if(strncmp(buff,"exit",4)==0)
        {
            printf("Connection terminated..!");
            break;
        }
    }

    close(sockfd);
    return 0;    
}