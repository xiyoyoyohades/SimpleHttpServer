#pragma once

#include <string>
using std::string;
class MySocket
{
  private:
    int port;
    int socketId;
    //Socket struct

  public:
    MySocket(/* args */);
    MySocket(int af, int type, int protocol);

    ~MySocket();
    void connect(const string &ipAddr, unsigned short port);
    void bind(unsigned short port);
    void listen(int backlog);
    void accept(MySocket &SK, struct sockaddr_in *cliAddr = 0);
    void close();

    //socket IO
    int read(char *usrbuf, int n);
    int write(char *usrbuf, int n);
};
