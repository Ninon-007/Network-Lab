
#include<stdio.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<asm-generic/socket.h>
#define PORT 8080

void main()
{
 int sockfd,newsock;
 struct sockaddr_in address;
 socklen_t addrlen = sizeof(address);
 char buff[1024];
 char msg[1024];

 sockfd = socket(AF_INET,SOCK_STREAM,0);
   if(sockfd<0)
   {  perror("socket");
        exit(1);
   } 
 int opt=1;
 setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT, &opt,sizeof(opt));

 address.sin_family=AF_INET;
 address.sin_addr.s_addr=INADDR_ANY;
 address.sin_port=htons(PORT);

 bind(sockfd,(struct sockaddr *)&address,sizeof(address));
 listen(sockfd,3);

 printf("SERVER IS WAITING....\n");
 newsock= accept(sockfd,(struct sockaddr *)&address,&addrlen);
 printf("CLIENT CONNECTED...\n");

 while(1)
 {
    memset(buff,0,sizeof(buff));
    read(newsock,buff,sizeof(buff));
    printf("CLIENT: %s\n",buff);

    if(strcmp(buff,"exit")==0)
    {
        printf("CLIENT DISCONNECTED...\n");
        break;
    }
    printf("SERVER: ");
    fgets(msg,sizeof(msg),stdin);
    send(newsock,msg,sizeof(msg),0);
 }
 close(newsock);
 close(sockfd);

}