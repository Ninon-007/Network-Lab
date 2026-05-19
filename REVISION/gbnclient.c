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
    int sockfd,bytes;
    struct sockaddr_in address;
    struct timeval tv;
    char buff[1024];
    char msg[1024];
    int packet,window,pack;

    address.sin_family=AF_INET;
    address.sin_port=htons(PORT);
    address.sin_addr.s_addr=inet_addr("127.0.0.1");

    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0)
    {
        perror("socket");
        exit(1);
    }

    connect(sockfd,(struct sockaddr*)&address,sizeof(address));

    tv.tv_sec=3;
    tv.tv_usec=0;
    setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(const char*)&tv,sizeof(tv));


    printf("Client connectd....\n");
    printf("Enter the number of packets: ");
    scanf("%d",&packet);
    printf("Enter the window size: ");
    scanf("%d",&window);
    
    int next=1;
    int base=1;
    int packsack=0;
    while(packsack<packet)
    {
        while(next<base+window && next<=packet)
        {
            printf("Packet %d sent\n",next);
            memset(msg,0,sizeof(msg));
            sprintf(msg,"%d",next);
            send(sockfd,msg,strlen(msg)+1,0);
            next++;
        }
        memset(buff,0,sizeof(buff));
        bytes=read(sockfd,buff,sizeof(buff));
        if(bytes>0)
        {
            pack=atoi(buff);
            printf("ACK received for %d\n",pack);
            if(pack>=base)
            {
                base=pack+1;
                packsack=pack;
            }
            
        }
        else
        {
            printf("Timeout,retransmittinf...\n");
            next=base;
        }

    }
    printf("All packets sent successfully\n");

    close(sockfd);


    return 0;
}