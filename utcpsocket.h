#ifndef UTCPSOCKET_H
#define UTCPSOCKET_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

class UTcpSocket
{
public:
    UTcpSocket();
    ~UTcpSocket();

    /*connect server*/
    int Connect(const char* host,int port);
    /*shut down and close socket*/
    void Close();

    int Recv(char * pBuf, unsigned int nLen);
    int Send(char * pBuf, unsigned int nLen);


private:
    int client_sockfd;
    static void* test(void* f_arg);
};

#endif // UTCPSOCKET_H
