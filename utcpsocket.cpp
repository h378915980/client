#include "utcpsocket.h"


UTcpSocket::UTcpSocket()
{

}

UTcpSocket::~UTcpSocket()
{

}

int UTcpSocket::Connect(const char *host, int port)
{
    if((client_sockfd=socket(PF_INET,SOCK_STREAM,0))<0){
        printf("UTcpSocket:socket error:%s\n",strerror(errno));
        return  -1;
    }

    struct sockaddr_in remote_addr;
    memset(&remote_addr,0,sizeof(remote_addr));
    remote_addr.sin_family=AF_INET;
    remote_addr.sin_addr.s_addr=inet_addr(host);
    remote_addr.sin_port=htons(port);


    if(connect(client_sockfd,(struct sockaddr*)&remote_addr,sizeof(sockaddr))<0){
        printf("UTcpSocket: connect error :%s\n",strerror(errno));
        return -1;
    }else{
        printf("UTcpSocket: connect success!\n");
        //pthread_t T_tid;
        //pthread_create(&T_tid,NULL,test,(void*)this);

    }

    return 0;
}

void UTcpSocket::Close()
{
    shutdown(client_sockfd,SHUT_RDWR);
    close(client_sockfd);
}

int UTcpSocket::Recv(char * pBuf, unsigned int nLen)
{
    return recv(client_sockfd,pBuf,nLen,0);
}

int UTcpSocket::Send(char * pBuf, unsigned int nLen)
{
    return send(client_sockfd,pBuf,nLen,0);
}

void* UTcpSocket::test(void* f_arg)
{
    UTcpSocket* l_ptr=(UTcpSocket*)f_arg;

    while(1){
        sleep(1);
        char sendbuff[]="first tcp ...";
        send(l_ptr->client_sockfd,sendbuff,sizeof(sendbuff),0);
    }

    return (void*)0;
}


