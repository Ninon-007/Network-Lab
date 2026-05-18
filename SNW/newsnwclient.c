#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/time.h>
#include<arpa/inet.h>
#include<netinet/in.h>

#define PORT 8080

int main()
{
    int sockfd, byte;

    char packet[1024];
    char buff[1024];

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

    inet_pton(AF_INET,
              "127.0.0.1",
              &address.sin_addr);

    connect(sockfd,
            (struct sockaddr*)&address,
            sizeof(address));

    printf("Connected to server...\n");

    tv.tv_sec = 3;
    tv.tv_usec = 0;

    setsockopt(sockfd,
               SOL_SOCKET,
               SO_RCVTIMEO,
               (const char*)&tv,
               sizeof(tv));

    while(1)
    {
        printf("Enter packet: ");

        fgets(packet,
              sizeof(packet),
              stdin);

        if(strncmp(packet,"exit",4)==0)
        {
            send(sockfd,
                 packet,
                 strlen(packet),
                 0);

            printf("Connection terminated...\n");
            break;
        }

        while(1)
        {
            send(sockfd,
                 packet,
                 strlen(packet),
                 0);

            printf("Packet sent: %s", packet);

            memset(buff,
                   0,
                   sizeof(buff));

            byte = read(sockfd,
                        buff,
                        sizeof(buff));

            if(byte > 0 &&
               strncmp(buff,"ACK",3)==0)
            {
                printf("ACK received for packet %s",
                       packet);

                break;
            }

            else
            {
                printf("Timeout... Retransmitting packet %s",
                       packet);
            }
        }
    }

    close(sockfd);

    return 0;
}