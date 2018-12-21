/*
 * <class CSockServer>
 *
 * 2007/04/05 Shuhei IKEMOTO
 *
 */

#ifndef _CSOCKSERVER_H
#define _CSOCKSERVER_H

// for socket
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
 #include <string.h>

#define DEFAULT_PORT 8088

class CSockServer
{
  protected:
    // for socket
    int server_sockfd, client_sockfd;
    int server_len; 
    socklen_t client_len;
    struct sockaddr_in server_address, client_address;
    // for select
    fd_set fds;
    struct timeval tv;

  public:
    CSockServer();
    virtual ~CSockServer();

    int initializeSocket(int port = DEFAULT_PORT);
    int terminateSocket();
    //int select();

    int writeToSock(char* writeBuf, int size);
    int readFromSock(char* readBuf, int size);
    int readFromSockWithSelect(char* readBuf, int size);
    int writeToSockWithSelect(char* writeBuf, int size);
};

#endif //_CSOCKSERVER_H
