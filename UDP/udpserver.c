#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

#define PORT 8080

int main()
{
    int sockfd;
    char buffer[1024];

    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);

    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Server details
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    // Bind
    bind(sockfd, (struct sockaddr*)&server, sizeof(server));

    printf("UDP Server Started...\n");

    while(1)
    {
        recvfrom(sockfd, buffer, sizeof(buffer), 0,
                (struct sockaddr*)&client, &len);

        printf("Client: %s\n", buffer);

        if(strncmp(buffer, "exit", 4) == 0)
            break;

        printf("Server: ");
        fgets(buffer, sizeof(buffer), stdin);

        sendto(sockfd, buffer, strlen(buffer), 0,
              (struct sockaddr*)&client, len);

        if(strncmp(buffer, "exit", 4) == 0)
            break;
    }

    close(sockfd);

    return 0;
}