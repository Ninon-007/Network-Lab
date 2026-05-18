#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/time.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define PORT 8090

int main()
{
    int sockfd;

    struct sockaddr_in address;
    struct timeval tv;

    char buffer[1024];
    char message[1024];

    int bytes, pack;
    int packet, window;

    printf("Enter total packets and window size: ");
    scanf("%d%d",&packet,&window);

    sockfd = socket(AF_INET,
                    SOCK_STREAM,
                    0);

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

    tv.tv_sec = 3;
    tv.tv_usec = 0;

    setsockopt(sockfd,
               SOL_SOCKET,
               SO_RCVTIMEO,
               (const char*)&tv,
               sizeof(tv));

    printf("Client connected...\n");

    int base = 1;
    int next = 1;
    int pckack = 0;

    while(pckack < packet)
    {
        while(next < base + window &&
              next <= packet)
        {
            printf("Packet %d sent\n",
                   next);

            memset(buffer,
                   0,
                   sizeof(buffer));

            sprintf(buffer,
                    "%d",
                    next);

            send(sockfd,
                 buffer,
                 strlen(buffer)+1,
                 0);

            next++;
        }

        memset(message,
               0,
               sizeof(message));

        bytes = read(sockfd,
                     message,
                     sizeof(message));

        if(bytes > 0)
        {
            pack = atoi(message);

            printf("ACK for packet %d received\n",
                   pack);

            if(pack >= base)
            {
                base = pack + 1;
                pckack = pack;
            }
        }

        else
        {
            printf("Timeout... Resending from packet %d\n",
                   base);

            next = base;
        }
    }

    printf("All packets sent successfully\n");

    close(sockfd);

    return 0;
}