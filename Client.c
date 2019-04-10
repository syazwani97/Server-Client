#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include<string.h>
int main()
{
     printf("this is a client program\n");
     struct sockaddr_in myaddr,clientaddr;
     int newsockid;
     int len;
     int sockid,client_add;
     sockid=socket(AF_INET,SOCK_STREAM,0);
     if(sockid==-1)
     perror("socket");
     memset(&myaddr,0,sizeof myaddr);
     myaddr.sin_port=htons(4444);
     myaddr.sin_family=AF_INET;
     myaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
     len=sizeof myaddr;
      if((bind(sockid,(struct sockaddr*)&myaddr,sizeof myaddr))==-1)
    perror("bind");
     int p=connect(sockid,(struct sockaddr*)&myaddr,len);
     if(p==-1)
     perror("connect");
     char msg[200];
     int i=0;
     char c;int l=0; char *buffer=(char *)(malloc(sizeof(char)*200)); int buffsize;
     while(i<10)
     {l=0;
                printf("Client: ");
                while((c=getchar())!='\n')
                {
            msg[l++]=c;
                }
                msg[l]='\0'; l++;
                send(sockid,msg,l,0);
                fflush(stdin);
                newsockid=accept(sockid,(struct sockaddr*)&clientaddr,&client_add);
                recv(newsockid,buffer,buffsize,0);
                   l=0;
                 fprintf(stdout, "server: %s", buffer);
                  printf("\n");
                i++;
     }
     close(sockid);
     return 0;
}
