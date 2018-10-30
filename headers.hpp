#ifdef linux
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <unistd.h>
#else //WIN32
#include <winsock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib, "WS2_32.lib") //?? add it here?
#endif
#include "queue.hpp"
//#include "Handler.hpp"
//#include "socket.hpp"
#include <thread>
#include <iostream>

#define MAXLINE 100 