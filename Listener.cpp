
#include "headers.hpp"
#include "socket.hpp"
#include "HandlerPool.hpp"
#include "Listener.hpp"

Listener::Listener(unsigned short port)
{
	running = true;
	std::thread listenerThread = std::thread(&Listener::Run, this, port); // start it in a thread
	listenerThread.join();
	//std::cout << "qq";
}

void Listener::Run(unsigned short port)
{
    handlerPool.start();
	std::cout << "handlerPool start 1";

    socket.bind(port); // bind to a port
    socket.listen(50);
    MySocket tmp;
    
    while (running)
    {
        socket.accept(tmp);
        handlerPool.addConn(tmp);
    }
}

void Listener::doStop()
{
    running = false;
    // close conn here !
}

Listener::~Listener()
{
}
