#include "socket.hpp"
#include "headers.hpp"
#include "Handler.hpp"

using namespace std;

class HandlerPool
{
  private:
    SafeQueue <MySocket> pendingConns;

    volatile boolean running;
    static const int num_threads = 1;// DO NOT change this!
	std::thread t[num_threads];
    //Handler    handlers[num_threads];
  public:
    HandlerPool();
    void start();
    void run(int tid);
    void addConn(MySocket commingConn);
    void stop();
	void process(MySocket coming);
    ~HandlerPool();
};

HandlerPool::HandlerPool()
{
}

void HandlerPool::start()
{
    running = true;
    
    for (int i = 0; i < num_threads; ++i)
    {
        t[i] = std::thread(&HandlerPool::run, this,i);
    }
}

void HandlerPool::run(int tid){
    //   the tid denote which of the the pendingConn[] to do
    // core loop here

    while(running){
        MySocket toResponse=pendingConns.dequeue();
        std::thread handlerThread=std::thread(&HandlerPool::process,this, toResponse);// start a new thread to process it
    }
    //once exit, close it's conn
    
}

void HandlerPool::addConn(MySocket commingConn){
    pendingConns.enqueue(commingConn);

}

void HandlerPool::stop(){
    running=false;
    //close conn here!
}

void HandlerPool::process(MySocket coming)
{
	// core process here!!! TODO

	// simple test for debug!!

	char buf[MAXLINE]; //,method[MAXLINE],uri[MAXLINE],version[MAXLINE];

	coming.read(buf, MAXLINE); //defined in header.h
	fputs(buf, stdout);        //print out
	char ret[MAXLINE];
	char body[MAXLINE];
	//send the response

	sprintf_s(body, "<html>\
			<title>test Web Server</title>\
			<body>\
				Hello World!Just a Web Server test:)\
			</body>\
			</html>\r\n");
	sprintf_s(ret, "HTTP/1.0 200 OK\r\n\
			Server: test Web Server\r\n\
			Content-type: text/html\r\n\
			Content-length: %d\r\n\r\n",
		strlen(body));
	coming.write(ret, strlen(ret));
	coming.write(body, strlen(body));
};

HandlerPool::~HandlerPool()
{
}
