#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define PORT 8080

int main()
{
    int sockfd, newsock;

    char filename[1024];
    char buff[1024];

    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    FILE *fp;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd < 0)
    {
        perror("socket");
        exit(1);
    }

    int opt=1;
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT,&opt,sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(sockfd,
         (struct sockaddr*)&address,
         sizeof(address));

    listen(sockfd, 3);

    printf("Server waiting...\n");

    newsock = accept(sockfd,
                      (struct sockaddr*)&address,
                      &addrlen);

    printf("Client connected...\n");

    read(newsock,
         filename,
         sizeof(filename));

    printf("Requested file: %s\n", filename);

    fp = fopen(filename, "r");

    if(fp == NULL)
    {
        strcpy(buff, "File not found\n");

        send(newsock,
             buff,
             sizeof(buff),
             0);
    }

    else
    {
        while(fgets(buff, sizeof(buff), fp) != NULL)
        {
            send(newsock,
                 buff,
                 sizeof(buff),
                 0);
        }

        fclose(fp);
    }

    close(newsock);
    close(sockfd);

    return 0;
}