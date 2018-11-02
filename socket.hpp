#pragma once

#include <string>
#include <ostream>
#include <fstream>
#include <sstream>
using std::string;
class MySocket
{
	public:
    int port;
    int socketId;
	//std::ofstream out;
	std::stringstream socketStringStream;
	//stream here
    

  
    MySocket(/* args */);
    MySocket(int af, int type, int protocol);
	MySocket::MySocket(int socketId);
	MySocket::MySocket(const MySocket &);
    ~MySocket();
    void connect(const string &ipAddr, unsigned short port);
    void bind(unsigned short port);
    void listen(int backlog);
    void accept(MySocket& comming);
    void close();

    //socket IO
    int read(char *usrbuf, int n);
    int write(const char *usrbuf, int n);

	//stream IO
	int socketOutFlush();
	int socketRead();
};
