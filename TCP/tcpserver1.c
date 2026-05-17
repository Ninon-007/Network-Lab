#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

#define PORT 8080

int main()
{

    int sockfd,newsock;
    char buff[1024];
    char msg[1024];
    struct sockaddr_in address;
    socklen_t addrlen=sizeof(address);
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0)
    {
        perror("socket");
        exit(1);
    }
    int opt=1;
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT,&opt,sizeof(opt));

    address.sin_family=AF_INET;
    address.sin_addr.s_addr=INADDR_ANY;
    address.sin_port=htons(PORT);

    bind(sockfd,(struct sockaddr*)&address,sizeof(address));
    listen(sockfd,3);

    printf("Server waiting for connection.....\n");
    newsock=accept(sockfd,(struct sockaddr*)&address,&addrlen);
    printf("Client connected..!\n");

    while(1)
    {
        memset(buff,0,sizeof(buff));
        read(newsock,buff,sizeof(buff));
        printf("Client: %s\n",buff);
        if(strncmp(buff,"exit",4)==0)
        {
            printf("Client disconected..\n");
            break;
        }
        printf("Server:");
        fgets(msg,sizeof(msg),stdin);
        send(newsock,msg,strlen(msg),0);
        if(strncmp(msg,"exit",4)==0)
        {
            printf("Server terminated connection..\n");
            break;

        }
    }
    close(sockfd);
    close(newsock);

 return 0;   
}