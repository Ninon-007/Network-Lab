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
    int sockfd,newsock,bytes;
    int ackprob=70;
    struct sockaddr_in address;
    char buff[1024];
    socklen_t addrlen=sizeof(address);

     srand(time(0));

    address.sin_family=AF_INET;
    address.sin_port=htons(PORT);
    address.sin_addr.s_addr=INADDR_ANY;

    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0)
    {
        perror("socket");
        exit(1);
    }

    int opt=1;
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT,&opt,sizeof(opt));

    bind(sockfd,(struct sockaddr*)&address,sizeof(address));
    listen(sockfd,3);

    int expected=1;
    printf("Server waiting....\n");
    newsock=accept(sockfd,(struct sockaddr*)&address,&addrlen);
    printf("Client connected...\n");

    while(1)
    {
        memset(buff,0,sizeof(buff));
        bytes=read(newsock,buff,sizeof(buff));
        if(bytes<=0)
        {
            printf("Client finished sending packets..\n");
            break;
        }
        int pkt=atoi(buff);
        printf("Packet received %d\n",pkt);
        if(pkt==expected)
        {
            expected++;
        }
        else
        {
            printf("Packet out of order,discarded...\n");
        }
        int ack=expected-1;

        if(rand()%100<ackprob)
        {
            printf("ACK sent for packet %d\n",ack);
            sprintf(buff,"%d",ack);
            send(newsock,buff,strlen(buff)+1,0);
        }
        else
        {
            printf("ACK lost %d\n",ack);
        }

    }
    close(newsock);
    close(sockfd);
    return 0;
}