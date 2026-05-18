#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>

#define PORT 8080

int main()
{

    int sockfd;
    char buff[1024];
    char filename[1024];
    struct sockaddr_in address;
    socklen_t addrlen=sizeof(address);

    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0)
    {
        perror("socket");
        exit(1);
    }
    address.sin_family=AF_INET;
    address.sin_port=htons(PORT);
    inet_pton(AF_INET,"127.0.0.1",&address.sin_addr);

    connect(sockfd,(struct sockaddr*)&address,sizeof(address));

    printf("Enter the filename: ");
    fgets(filename,sizeof(filename),stdin);

    filename[strcspn(filename,"\n")]=0;

    send(sockfd,filename,sizeof(filename),0);

    printf("File Contents are...\n");

    while(read(sockfd,buff,sizeof(buff))>0)
    {
        printf("%s",buff);
        memset(buff,0,sizeof(buff));

    }
    close(sockfd);

    return 0;
}