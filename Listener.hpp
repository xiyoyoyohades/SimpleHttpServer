#pragma once
#include "socket.hpp"
#include "HandlerPool.hpp"
class Listener
{
private:
	MySocket socket;
	HandlerPool handlerPool;
	volatile boolean running;

public:
	Listener(unsigned short port);// I forget to add a seperate start() QAQ
	void Run(unsigned short port);
	void doStop();
	~Listener();
};
