#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>

#define PORT 8090

int main()
{
    int sockfd, newsock;

    struct sockaddr_in address;
    socklen_t len = sizeof(address);

    int bytes;

    char buffer[1024];

    int loss = 30;

    srand(time(0));

    sockfd = socket(AF_INET,
                    SOCK_STREAM,
                    0);

    if(sockfd < 0)
    {
        perror("socket");
        exit(1);
    }

    int opt = 1;

    setsockopt(sockfd,
               SOL_SOCKET,
               SO_REUSEADDR | SO_REUSEPORT,
               &opt,
               sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(sockfd,
         (struct sockaddr*)&address,
         sizeof(address));

    listen(sockfd,3);

    printf("Server waiting...\n");

    newsock = accept(sockfd,
                     (struct sockaddr*)&address,
                     &len);

    printf("Client connected...\n");

    int expected = 1;

    while(1)
    {
        memset(buffer,
               0,
               sizeof(buffer));

        bytes = read(newsock,
                     buffer,
                     sizeof(buffer));

        if(bytes <= 0)
        {
            printf("Client finished sending packets\n");
            break;
        }

        int pkt = atoi(buffer);

        printf("Packet %d received\n",
               pkt);

        if(pkt == expected)
        {
            expected++;
        }

        else
        {
            printf("Out-of-order packet discarded\n");
        }

        int ack = expected - 1;

        if(rand()%100 < loss)
        {
            printf("ACK %d lost\n",
                   ack);
        }

        else
        {
            printf("ACK %d sent\n",
                   ack);

            sprintf(buffer,
                    "%d",
                    ack);

            send(newsock,
                 buffer,
                 strlen(buffer)+1,
                 0);
        }
    }

    close(newsock);
    close(sockfd);

    return 0;
}