#include "stdafx.h"
#include "headers.hpp"
#include "debug.h"
#include "socket.hpp"

#include <string>
using std::string;

MySocket::MySocket()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(2, 0);
	int err = WSAStartup(wVersionRequested, &wsaData);
	// debug test
	check(err == 0);
	socketId = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	check(socketId > 0);
}
MySocket::MySocket(const MySocket & socket) {
	socketId = socket.socketId;
};
MySocket::MySocket(int Id)
{
	socketId = Id;
}
MySocket::MySocket(int af, int type, int protocol)
{
	socketId = socket(af, type, protocol);
	check(socketId > 0);
}

MySocket::~MySocket()
{
	//WSACleanup();
	//you must destroy socket maually
	//info( "delete socket");
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
	char optval = 1;
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = INADDR_ANY;
	servAddr.sin_port = htons(port);
	int err;
	err = setsockopt(socketId, SOL_SOCKET, SO_REUSEADDR, (char* )&optval, sizeof(char));
	check(err >= 0, "setsockopt failed!");
	err = ::bind(socketId, (struct sockaddr *)&servAddr, sizeof(servAddr));
	check(err >= 0,"bind failed!");
}

void MySocket::listen(int backlog)
{
	int err = ::listen(socketId, backlog);
	check(err >= 0,"Listen failed! ");
}

void MySocket::accept(MySocket& comming)
{
	SOCKET sk = -1;
	
		struct sockaddr_in cliaddr;
		memset(&cliaddr, 0, sizeof(cliaddr));
		socklen_t socklen = sizeof(cliaddr);
		sk = ::accept(socketId, (struct sockaddr *)&cliaddr, &socklen);
	//TODO print the cliaddr
	//	ssz TODO !
	check(sk > 0, "accept failed! ");

	comming.socketId = sk;
}
void MySocket::close()
{
	int err = closesocket(socketId);
	check(err >= 0);
}
int MySocket::read(char *usrbuf, int n){
	check(socketId >= 0);
	int left = n;
	char *bufp= usrbuf;
	int read = 0;
	int err = 0;
	//while (left>0)
	{
		read = ::recv(socketId, usrbuf, left,0);
		//if (read == 0) break;//end
		check(read > 0, "read failed! ");
		if (read < 0)std::terminate();
		left -= read;
		bufp += read;
	}
	bufp = '\0';
	return n - left;//left to read?
};
int MySocket::write(const char *usrbuf, int n){
	check(socketId >= 0);
	int left = n;
	const char *bufp = usrbuf;
	int written = 0;
	int err = 0;
	while (left>0)
	{
		written = send(socketId, usrbuf, left, 0);
		if (written <= 0) break;//end
		check(written > 0, "written failed! force thread to terminate");
		if (written < 0)std::terminate();
		left -= written;
		bufp += written;
	}
	info("left to written %d", left);
	return n - left;//left to written?
};
int MySocket::socketOutFlush() {
	int size = sizeof(socketStringStream);
	const char* buf=socketStringStream.str().c_str();
	write(buf, size);

	//https://stackoverflow.com/questions/20731/how-do-you-clear-a-stringstream-variable
     socketStringStream.str(std::string());

}	
	int MySocket::socketRead() {
	}

	