#include<iostream>
#include<fstream>
#include<thread>
#include<string>
#include<ostream>
#include<sstream>
#include<streambuf>
#include<vector>
#include<map>
#include<algorithm>
#include<regex>
#include<exception>
#include<stdexcept>

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "thread_safe_queue.hpp"
#include "thread_safe_set.hpp"

#pragma comment(lib, "WS2_32.lib")

#define maxwait 20
#define maxline 1000

//extern int on;




class socket_wrap{

    public:
    
    socket_wrap();
    socket_wrap(int id);
    ~socket_wrap();

    unsigned short port;
    int socket_id;
    sockaddr_in addr;

    void mybind(unsigned short port);
    void mylisten(int backlog);
    void myaccept(socket_wrap& talk);
    int mysend(int length, const char* buffer);
    int myreceive(int length, char* buffer);
    void myclose();
};

class connect_heap{
    public:

    connect_heap();
    ~connect_heap();

    int switcher=0;
    int thread_num=1;
    std::vector<std::thread> thread_pool;
    SafeQueue<socket_wrap> waiting_connects;
	SafeSet<int> using_connects;

    void init();
    void insert_connect(socket_wrap talk_soc);
    void assign_serv(int i);
    void serve(socket_wrap toserve, int i, int j);
    void close();
    void set_thread_num(int num);
    
};

class listener{

    public:

    listener(unsigned short port, int thread_num);
    ~listener();

    int switcher=0;
    socket_wrap listen_soc;
    connect_heap connect_heap;

    void start_listen(unsigned short port);
    void close_listener();

};