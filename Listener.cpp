#include "stdafx.h"
#include "headers.hpp"
#include "socket.hpp"
#include "HandlerPool.hpp"
#include "Listener.hpp"

Listener::Listener(unsigned short port)
{
	running = true;

	std::thread listenerThread = std::thread(&Listener::Run, this, port); // start it in a thread
	//Sleep(100000);
	listenerThread.detach();
}

void Listener::Run(unsigned short port)
{
    handlerPool.start();
	//std::cout << "handlerPool start 1";
	
    socket.bind(port); // bind to a port
    socket.listen(50);
    
    
    while (running)
    {
		info("Listener waiting");
		MySocket tmp(-1);
		//std::cout << "\nListener \n" << tmp.socketId <<" "<< socket.socketId << "\n";
		socket.accept(tmp);
		info("Listener get one conn %d", tmp.socketId);
        handlerPool.addConn(tmp);
		
    }
	//Sleep(100000);
}

void Listener::doStop()
{
    running = false;
    // close conn here !
}

Listener::~Listener()
{
}
