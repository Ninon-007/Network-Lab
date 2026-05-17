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

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr*)&server, sizeof(server));

    printf("UDP Server Started...\n");

    while(1)
    {
        recvfrom(sockfd, buffer, sizeof(buffer), 0,
                (struct sockaddr*)&client, &len);

        printf("Client: %s\n", buffer);

        if(strcmp(buffer, "exit") == 0)
            break;

        printf("Server: ");
        fgets(buffer, sizeof(buffer), stdin);

        // Remove newline
        buffer[strcspn(buffer, "\n")] = 0;

        sendto(sockfd, buffer, strlen(buffer)+1, 0,
              (struct sockaddr*)&client, len);

        if(strcmp(buffer, "exit") == 0)
            break;
    }

    close(sockfd);

    return 0;
}