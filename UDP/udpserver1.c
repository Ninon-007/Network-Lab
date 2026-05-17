#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>
#define PORT 8086

int main()
{
    int sockfd;
    char buff[1024];
    char msg[1024];
    struct sockaddr_in server,client;
    socklen_t addrlen=sizeof(client);

    sockfd=socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd<0)
    {
        perror("socket");
        exit(1);
    }
    server.sin_family=AF_INET;
    server.sin_addr.s_addr=INADDR_ANY;
    server.sin_port=htons(PORT);

    bind(sockfd,(struct sockaddr*)&server,sizeof(server));

    printf("Server waiting....\n");
    while(1)
    {
        memset(buff,0,sizeof(buff));
        recvfrom(sockfd,buff,sizeof(buff),0,(struct sockaddr*)&client,&addrlen);
        printf("Client:%s\n",buff);
        if(strncmp(buff,"exit",4)==0)
        {
            printf("Connection Ended..\n");
            break;
        }
        printf("Server: ");
        fgets(msg,sizeof(msg),stdin);
        sendto(sockfd,msg,sizeof(msg),0,(struct sockaddr*)&client,addrlen);
        if(strncmp(msg,"exit",4)==0)
        {
            printf("Connection Ended..\n");
            break;
        }

    }
    close(sockfd);
    return 0;
}
