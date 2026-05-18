#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/time.h>

#define PORT 8090

int main()
{
    int sockfd, bytes;
    char packet[1024];
    char buffer[1024];
    struct sockaddr_in address;
    struct timeval tv;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd < 0)
    {
        perror("socket");
        exit(1);
    }

    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    inet_pton(AF_INET,"127.0.0.1",&address.sin_addr);

    connect(sockfd,(struct sockaddr*)&address,sizeof(address));

    printf("Client connected...\n");

    tv.tv_sec = 3;
    tv.tv_usec = 0;

    setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(const char*)&tv,sizeof(tv));

    while(1)
    {
        printf("Enter packet: ");
        fgets(packet, sizeof(packet), stdin);

        send(sockfd,packet,sizeof(packet), 0);

        if(strncmp(packet, "exit", 4) == 0)
        {
            break;
        }

        memset(buffer, 0, sizeof(buffer));
        bytes = read(sockfd,buffer,sizeof(buffer));

        if(bytes > 0 && strncmp(buffer, "ACK", 3) == 0)
        {
            printf("ACK received for packet %s",packet);
        }

        else
        {
            printf("Timeout...\n");
            printf("Retransmitting packet %s",packet);
            send(sockfd,packet, sizeof(packet), 0);
            memset(buffer, 0, sizeof(buffer));
            read(sockfd, buffer, sizeof(buffer));
            printf("ACK received after retransmission\n");
        }
    }

    close(sockfd);

    return 0;
}