#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")
#pragma command(lib, "mswsock.lib")
//#pragma command(lib, "advapi32.lib")

#define DEFAULT_BUFLEN 512

#define DEFAULT_PORT "25"
#define DEFAULT_ADDRESS "smtp.live.com"

char *recvbuf = NULL;
int recvbuflen = 0;
int recvbufused = 0;

int sendData(SOCKET s, char *buffer, int buflen)
{
    int iResult;

    while (buflen > 0)
    {
        // TODO: if the connection is secure, encrypt the data being sent...

        iResult = send(s, buffer, buflen, 0);
        if (iResult == SOCKET_ERROR)
        {
            printf("send failed with error: %d\n", WSAGetLastError());
            return -1;
        }

        buffer += iResult;
        buflen -= iResult;
    }

    return 0;
}

int sendLine(SOCKET s, char *line)
{
    int iResult = sendData(s, line, strlen(line));
    if (iResult == 0)
        iResult = sendData(s, "\r\n", 2);
    return iResult;
}

int readLine(SOCKET s, char **line)
{
    char buf[DEFAULT_BUFLEN];
    int iResult;
    int offset = 0, len, total;
    char *p;

    *line = NULL;

    do
    {
        // check if the line terminator is already in the buffer
        p = memchr(recvbuf+offset, '\n', recvbufused-offset);
        if (p != NULL)
        {
            ++p;
            total = (p - recvbuf);

            // check if the line terminator is actually "\r\n"
            len = total - 1;
            if ((len > 0) && (recvbuf[len-1] == '\r'))
                --len;

            // extract the line and remove it from the buffer
            *line = malloc(len+1);
            if (*line == NULL)
            {
                printf("Unable to allocate memory! Bytes: %d\n", len+1);
                return -1;
            }

            memcpy(*line, recvbuf, len);
            (*line)[len] = 0;

            memmove(recvbuf, recvbuf+total, recvbufused-total);
            recvbufused -= total;

            printf("%s\n", *line);
            return len;
        }

        // line terminator is not in the buffer, keep reading

        // do not search the buffer that was already searched
        offset = recvbufused;

        // read more data from the socket
        iResult = recv(s, buf, sizeof(buf), 0);
        if (iResult <= 0)
        {
            if (iResult == 0)
                printf("server closed the connection\n");
            else
                printf("recv failed with error: %d\n", WSAGetLastError());

            break;
        }

        // TODO: if the connection is secure, decrypt the data that was received...

        // if the buffer is too small for the new data, grow it
        if ((recvbufused+iResult) > recvbuflen)
        {
            len = (recvbufused+iResult) + DEFAULT_BUFLEN;
            p = realloc(recvbuf, len);
            if (p == NULL)
            {
                printf("Unable to allocate memory! Bytes: %d\n", len);
                break;
            }

            recvbuf = p;
            recvbuflen = len;
        }

        // now add the data to the buffer
        memcpy(recvbuf+recvbufused, buf, iResult);
        recvbufused += iResult;
    }
    while (1);

    return -1;
}

int readResponse(SOCKET s)
{
    char *line, term[5];
    int len, code = 0;

    // read the first line
    len = readLine(s, &line);
    if (len == -1)
        return -1;

    // check if the response has multiple lines
    if ((len > 3) && (line[3] == '-'))
    {
        // keep reading until the last line is received
        memcpy(term, line, 3);
        term[3] = ' ';
        term[4] = '\0';

        do
        {
            free(line);
            len = readLine(s, &line);
            if (len == -1)
                return -1;
        }
        while ((len > 3) && (strncmp(line, term, 4) != 0));
    }

    // now extract the response code and return it

    if (len > 3) line[3] = '\0';
    code = strtol(line, NULL, 10);
    free(line);

    if (code == 0)
        return -1;

    return code;
}

int __cdecl main(int argc, char **argv)
{
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo hints, *result, *ptr;
    char messaggio[100];
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0)
    {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(DEFAULT_ADDRESS, DEFAULT_PORT, &hints, &result);
    if (iResult != 0)
    {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL; ptr=ptr->ai_next)
    {
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET)
        {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            break;
        }

        //Connect to server.
        iResult = connect(ConnectSocket, ptr->ai_addr, ptr->ai_addrlen);
        if (iResult == 0)
            break;

        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET)
    {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    printf("Connected to server\n");

    // read the greeting
    iResult = readResponse(ConnectSocket);
    if (iResult == 220)
    {
        do
        {
            // ask the user for a command
            printf("messaggio: ");
            if (!fgets(messaggio, sizeof(messaggio), stdin))
                break;

            // remove the line terminator if present
            iResult = strlen(messaggio);
            if ((iResult > 0) && (messaggio[iResult-1] == '\n'))
                messaggio[iResult-1] = '\0';

            // send the command
            iResult = sendLine(ConnectSocket, messaggio);
            if (iResult == -1)
                break;

            // read the response
            iResult = readResponse(ConnectSocket);
            if (iResult == -1)
                break;

            // need to stop now?
            if (strcmp(messaggio, "QUIT") == 0)
                break;

            // need to turn on SSL/TLS now?
            if ((strcmp(messaggio, "STARTTLS") == 0) && (iResult == 220))
            {
                // TODO: activate SSL/TLS, stop if failed...
            }
        }
        while(1);
    }

    shutdown(ConnectSocket, SD_SEND);
    do
    {
        iResult = recv(ConnectSocket, messaggio, sizeof(messaggio), 0);
    }
    while (iResult > 0);

    closesocket(ConnectSocket);
    WSACleanup();

    free(recvbuf);

    system("PAUSE");
    return 0;
}
