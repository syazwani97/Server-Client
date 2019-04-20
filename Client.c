#include <sys/socket.h>
    #include <arpa/inet.h>
    #include <stdio.h>
    #include <unistd.h>
    #include <time.h>
 
    #define MAX_BUFFER            128
    #define DAYTIME_SERVER_PORT   13
 
   int main ( )
   {
     int connectionFd, in;
     struct sockaddr_in servaddr;
     char timebuffer[MAX_BUFFER+1];

     connectionFd = socket(AF_INET, SOCK_STREAM, 0);

     memset(&servaddr, 0, sizeof(servaddr));
     servaddr.sin_family = AF_INET;
     servaddr.sin_port = htons(DAYTIME_SERVER_PORT);
           servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

     connect(connectionFd, (struct sockaddr_in *)&servaddr,
              sizeof(servaddr));

     while ( (in = read(connectionFd,
                         timebuffer, MAX_BUFFER)) > 0) {
       timebuffer[in] = 0;
       printf("\n%s", timebuffer);
     }

    close(connectionFd);

     return(0);
   }

