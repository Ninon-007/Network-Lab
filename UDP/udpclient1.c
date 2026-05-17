#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>

#define PORT 8086

int main()
{

    int sockfd;
    char msg[1024];
    char buff[1024];
    struct sockaddr_in client;
    socklen_t addrlen=sizeof(client);

    client.sin_family=AF_INET;
    client.sin_port=htons(PORT);
    inet_pton(AF_INET,"127.0.0.1",&client.sin_addr);

    sockfd=socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd<0)
    {
        perror("socket");
        exit(1);
    }
   
    printf("Client started...\n");

    while(1)
    {
        printf("Client: ");
        fgets(msg,sizeof(msg),stdin);
        sendto(sockfd,msg,sizeof(msg),0,(struct sockaddr*)&client,addrlen);
        if(strncmp(msg,"exit",4)==0)
        {
            printf("Client exited..\n");
            break;
        }
         memset(buff,0,sizeof(buff));
        recvfrom(sockfd,buff,sizeof(buff),0,(struct sockaddr*)&client,&addrlen);
          printf("Server:%s\n",buff); 
        if(strncmp(buff,"exit",4)==0)
        {
            printf("Server exited..\n");
            break;
        }

    }
    close(sockfd);

    return 0;  
}