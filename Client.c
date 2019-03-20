// Write CPP code here 
#include <netdb.h> 
#include <stdio.h> 
#include <unistd.h>
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <arpa/inet.h>
#define PORT 5000 

int main(int argc , char const *argv[])
{
     int sockfd = 0, vread;
     char *hello = "hello from client";
     char buff[1000] = {0};
     struct sockaddr_in adrress;
     struct sockaddr_in serv_addr;
     
     //create socket
     if ((sockfd = socket(AF_INET, SOCK_STREAM,0)) < 0)
     {
          printf ("could not create socket");
	  return -1;
     }
     memset(&serv_addr, '0', sizeof(serv_addr));
     
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_port = htons(5000);
     
     if (inet_pton(AF_INET, "192.168.184.131", &serv_addr.sin_addr) <= 0)
     {
          //print the error message
          printf("\n it's error \n");
          return -1;
     }
	
	//Accept and incoming connection
	send(sockfd, hello, strlen(hello) , 0);
	  printf("hello message sent");
	  vread = read(sockfd, buff , 1000);
	  printf("%s\n" , buff);
	  
	
	return 0;
}
