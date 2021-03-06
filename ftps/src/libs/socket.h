#ifndef SOCKET_H
#define SOCKET_H

#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#define USE_SSL

#define BACKLOG 20
#define MAXRECV	2048

class Socket{
private:
	int _sockfd;
	SSL* _sockssl;
	struct sockaddr_in _addr;
	bool is_valid();
	
public:
	Socket();
	~Socket();
	bool create();
	bool bind(int);
	bool connect(int,int);
	bool listen();
	bool accept(Socket&);
	int send(std::string);
	int recv(std::string&);
	bool close();
	int fd();
	void fd(int);
	SSL* ssl();
	void ssl(SSL*);
	int port();
	std::string host();
};

#endif