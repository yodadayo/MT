#include "CSockServer.h"


CSockServer::CSockServer()
{}

CSockServer::~CSockServer()
{}

int CSockServer::initializeSocket(int port)
{
    // make socket
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(port);

    server_len = sizeof(server_address);
    int Ret = bind(server_sockfd,
		   (struct sockaddr *)&server_address,
		   server_len);
    if(Ret == -1){
	perror("bind");
	exit(EXIT_FAILURE);
    }
    listen(server_sockfd, 5);
    
    // accept
    client_len = sizeof(client_address);
    
    // following line may depend on your programming environment.
    // if you found a corresponding error, please toggle following lines.
    
    // client_sockfd = accept(server_sockfd,
// 			   (struct sockaddr *)&client_address,
// 			   (size_t *)&client_len);
    client_sockfd = accept(server_sockfd,
			   (struct sockaddr *)&client_address,
			   &client_len);
    
    // regist for select
    FD_ZERO(&fds);
    FD_SET(client_sockfd, &fds);
    tv.tv_sec = 0;
    tv.tv_usec = 0;

    return 0;
}

int CSockServer::terminateSocket()
{
    close(client_sockfd);
    
    return 0;
}

int CSockServer::writeToSock(char* writeBuf, int size)
{
    return write(client_sockfd, writeBuf, size);
}

int CSockServer::readFromSock(char* readBuf, int size)
{
    return read(client_sockfd, readBuf, size);
}

int CSockServer::readFromSockWithSelect(char* readBuf, int size)
{
    // tmp fd_set and timeval
    fd_set fdset;
    struct timeval tvout;

    memcpy(&fdset, &fds, sizeof(fd_set));
    memcpy(&tvout, &tv, sizeof(timeval));

    int n = select(client_sockfd+1, &fdset, NULL, NULL, &tvout);
    if(n == -1){
	// something error was caused
	perror("select");
	exit(-2);
    }else if(n == 0){
	// time out
	return -1;
    }else{
	return readFromSock(readBuf, size);
    }
    
    return 0;
}
