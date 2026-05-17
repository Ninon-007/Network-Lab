#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

#define PORT 8080

int main()
{
    int sockfd;
    char buffer[1024];

    struct sockaddr_in server;
    socklen_t len = sizeof(server);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

    printf("UDP Client Started...\n");

    while(1)
    {
        printf("Client: ");
        fgets(buffer, sizeof(buffer), stdin);

        // Remove newline
        buffer[strcspn(buffer, "\n")] = 0;

        sendto(sockfd, buffer, strlen(buffer)+1, 0,
              (struct sockaddr*)&server, len);

        if(strcmp(buffer, "exit") == 0)
            break;

        recvfrom(sockfd, buffer, sizeof(buffer), 0,
                (struct sockaddr*)&server, &len);

        printf("Server: %s\n", buffer);

        if(strcmp(buffer, "exit") == 0)
            break;
    }

    close(sockfd);

    return 0;
}