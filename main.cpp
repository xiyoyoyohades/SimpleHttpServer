#include "stdafx.h"
#include "headers.hpp"
#include "Listener.hpp"
#include "HandlerPool.hpp"
string fileBase = "E:\\document\\computer\\www";
int main(int argc, char const *argv[])
{
    //start thread pool
   // HandlerPool handlerPool;
    //handlerPool.start();
    //get a port from conf
	
    unsigned short port=8181;

	//read from input

	//cin >> fileBase;


    //start listener thread
    Listener localListener(port);

	//wait for input to close
	getchar();
	localListener.doStop();
	info("server stopped!");
    return 0;
}
