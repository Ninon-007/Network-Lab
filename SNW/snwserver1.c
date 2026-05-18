#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/time.h>
#include<time.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>

#define PORT 8080

int main()
{
   int sockfd,newsock,byte;
   char buff[1024];
   struct sockaddr_in address;
   socklen_t addrlen=sizeof(address);
    srand(time(0));
    int ackprob=70;

    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0)
    {
        perror("socket");
        exit(1);
    }

    int opt=1;
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT,&opt,sizeof(opt));

    address.sin_family=AF_INET;
    address.sin_port=htons(PORT);
    address.sin_addr.s_addr=INADDR_ANY;

    bind(sockfd,(struct sockaddr*)&address,sizeof(address));
    listen(sockfd,3);

    printf("Server waiting.....\n");
    newsock=accept(sockfd,(struct sockaddr*)&address,&addrlen);
    printf("Connected to client..\n");

    while(1)
    {
        memset(buff,0,sizeof(buff));
        byte=read(newsock,buff,sizeof(buff));
        if(byte<0)
        {
            break;
        }

        if(strncmp(buff,"exit",4)==0)
        {
            printf("Client closed connection..\n");
            break;
        }

        printf("Packet received: %s", buff);
        
        if(rand()%100<ackprob)
        {
            printf("ACK sent\n");
            send(newsock,"ACK",sizeof("ACK"),0);
        }
        else
        {
            printf("ACK Lost\n");
        }
    }

    close(newsock);
    close(sockfd);

    return 0;
}