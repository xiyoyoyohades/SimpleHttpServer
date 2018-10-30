#include "headers.hpp"
#include "debug.h"
#include "socket.hpp"

#include <string>
using std::string;
/*
class Socket
{
  private:
	int port;
	int socketId;
	//Socket struct

  public:						*/
	//Socket(/* args */);
	/*Socket(int af, int type, int protocol);

	~Socket();
	void connect(const string &ipAddr, unsigned short port);
	void bind(unsigned short port);
	void listen(int backlog);
	void accept(Socket &SK, struct sockaddr_in *cliAddr = 0);
	void close();
	int read(char *usrbuf, int n);
	int write(char *usrbuf, int n);
};

*/

MySocket::MySocket(/* args */)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(2, 0);
	int err = WSAStartup(wVersionRequested, &wsaData);
	// debug test
	check(err == 0);
}
MySocket::MySocket(int af, int type, int protocol)
{
	socketId = socket(af, type, protocol);
	check(socketId > 0);
}

MySocket::~MySocket()
{
	WSACleanup();
}

void MySocket::connect(const string &ipAddr, unsigned short port)
{
	struct sockaddr_in servAddr; // delete this TODO

	memset(&servAddr, 0, sizeof(servAddr));
	int err;
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(port);
	err = inet_pton(AF_INET, ipAddr.c_str(), &servAddr.sin_addr);
	check(err == 1);

	err = ::connect(socketId, (struct sockaddr *)&servAddr, sizeof(servAddr)); //why :: ?
	check(err >= 0);
}

void MySocket::bind(unsigned short port)
{
	struct sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = INADDR_ANY;
	servAddr.sin_port = htons(port);
	int err;
	err = ::bind(socketId, (struct sockaddr *)&servAddr, sizeof(servAddr));
	check(err >= 0);
}

void MySocket::listen(int backlog)
{
	int err = ::listen(socketId, backlog);
	check(err >= 0);
}

void MySocket::accept(MySocket &SK, struct sockaddr_in *cliAddr)
{
	SOCKET sk = -1;
	if (cliAddr == NULL) //why?????????
	{
		struct sockaddr_in cliaddr;
		memset(&cliaddr, 0, sizeof(cliaddr));
		socklen_t socklen = sizeof(cliaddr);
		sk = ::accept(socketId, (struct sockaddr *)&cliaddr, &socklen);
	}
	else
	{
		socklen_t socklen = sizeof(sockaddr_in);
		sk = ::accept(socketId, (struct sockaddr *)cliAddr, &socklen);
	}
	SK.socketId = sk;
}
void MySocket::close()
{
	int err = closesocket(socketId);
	check(err >= 0);
}
int MySocket::read(char *usrbuf, int n){
	return 1;
};
int MySocket::write(char *usrbuf, int n){
	return 1;
};