#include "stdafx.h"

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
#include "Handler.hpp"
#include "debug.h"
#include <thread>
#include <iostream>
#include <sstream>
#include <string>
#include <ostream>
#include <fstream>
#include <regex>
#include <stdio.h>
#include <stdlib.h>

#include <streambuf>
//#include <regex.h>

#define MAXLINE 100 