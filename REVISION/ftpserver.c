#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>

#define PORT 8080

int main()
{
    int sockfd,newsock;
    char buff[1024];
    char filename[1024];
    struct sockaddr_in address;
    socklen_t addrlen=sizeof(address);
    FILE *fp;

    address.sin_family=AF_INET;
    address.sin_port=htons(PORT);
    address.sin_addr.s_addr=INADDR_ANY;

    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0)
    {
        perror("socket");
        exit(1);
    }

    int opt=1;
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT,&opt,sizeof(opt));

    bind(sockfd,(struct sockaddr*)&address,sizeof(address));
    listen(sockfd,3);

    printf("Server Waiting...\n");
    newsock=accept(sockfd,(struct sockaddr*)&address,&addrlen);
    printf("Client connected...\n");

        memset(filename,0,sizeof(filename));
        read(newsock,filename,sizeof(filename));

        printf("Requested filename is %s\n",filename);

        fp=fopen(filename,"r");

        if(fp==NULL)
        {
            printf("File not found");
            strcpy(buff,"File not found\n");
            send(newsock,buff,sizeof(buff),0);
        }

        else
        {
            while(fgets(buff,sizeof(buff),fp)!=0)
            {
                send(newsock,buff,sizeof(buff),0);

            }
            fclose(fp);

        }

    close(newsock);
    close(sockfd);

    return 0;
}