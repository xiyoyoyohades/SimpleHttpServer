#include "stdafx.h"
#include "socket.hpp"
#include "headers.hpp"
#include "Handler.hpp"

using namespace std;
extern string fileBase;
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
	void process(int coming);
	//void process(MySocket coming);
    ~HandlerPool();
};

HandlerPool::HandlerPool()
{
}

void HandlerPool::start()
{
    running = true;
    
	info( "HandlerPool init");
    for (int i = 0; i < num_threads; ++i)
    {
        t[i] = std::thread(&HandlerPool::run, this,i);
    }
	info("HandlerPool start done ");
}

void HandlerPool::run(int tid){
    //   the tid denote which of the the pendingConn[] to do
    // core loop here

    while(running){
		//std::cout << "fuck1 dequeue";
        MySocket toResponse=pendingConns.dequeue();
		//std::cout << "dequeue a new socket "<<toResponse.socketId<<"\n";
		//process(toResponse);
        std::thread handlerThread=std::thread(&HandlerPool::process,this, toResponse.socketId);// start a new thread to process it
		handlerThread.detach();
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


void HandlerPool::process(int  id)
{
	info(" process a new socket ");
	//revive the socket
	MySocket comming(id);

	while (running)
	{
		//read the buf
		char buf[MAXLINE+1]; //defined in header.hpp
		comming.read(buf, MAXLINE);
		

		//regex process
		//fputs(buf, stdout); 
		//std::cout << "\n then the regex get!\n";       //print out
		std::smatch sm;
		std::regex regex("([A-Za-z]+) +(.*) +(HTTP/[0-9][.][0-9])");
		std::string text(buf);
		 int match=std::regex_search(text, sm,regex);
		 //std::cout << match;
		 if (match==0)
		 {
			 //info("no header found, read again");
			 break;
		 }
		 else {
			 //info("\nthe matches were: \n");
			 for (unsigned i = 0; i < sm.size(); ++i) {
				 //std::cout << sm[i] << "\n ";
			 }
		 }

		 string operation1 = sm[1];// GET
		 string operation2 = sm[2];// /index.html
		 std::cout << operation1 << operation2 << std::endl;
		 //ssz TODO !
	
		
		  ;
		  std::cout <<"[debug] file at:"<< fileBase << operation2<<std::endl;
std::ifstream t(fileBase+"operation2");
//https://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
std::stringstream buffer;

buffer << t.rdbuf();
std::cout << "[debug] the file get:\n" << buffer.str() << std::endl;
int size = sizeof(buffer);

			//auto responseCode="200 OK";
auto responseCode = "404 Not Found";
/* { "htm", "text/html" },
		 { "html", "text/html" },
		 { "xml", "text/xml" },
		 { "txt", "text/plain" },
		 { "css", "text/css" },
		 { "png", "image/png" },
		 { "gif", "image/gif" },
		 { "jpg", "image/jpg" },
		 { "jpeg", "image/jpeg" },*/

auto contentType = "text/html";
comming.socketStringStream <<"HTTP/1.1 " <<responseCode <<"\r\n"
	<< "Server: Miao\r\n"<< "Content-Length: " << size <<"\r\n"
	<<"Connection: close\r\n"<<"Content-Type: " << contentType << "\r\n\r\n";
comming.socketStringStream << buffer.str();
		//comming.socketStringStream << ERROR_HTML_PAGE;
		std::cout <<"[debug] what we put:\n"<< comming.socketStringStream.str() << std::endl;
		comming.socketOutFlush();
		//info(" process done ");

	}
};






//https://github.com/Wiznet/WIZ550Web_STM32F103RB_CoIDE/blob/master/Wiz550web_App_CoIDE_Version/src/Internet/httpServer/httpParser.h
static const char ERROR_HTML_PAGE[] = "HTTP/1.1 404 Not Found\r\nContent-Type: \
text/html\r\nContent-Length: 78\r\n\r\n<HTML>\r\n<BODY>\r\nSorry, the page you requested was not found.\
\r\n</BODY>\r\n</HTML>\r\n\0";
//https://github.com/Z-Y00/seasocks/blob/master/src/main/c/seasocks/Request.cpp
char * debugBuf = "GET /index.html HTTP/1.1\
			Host: 127.0.0.1 : 8181\
			User - Agent : Mozilla / 5.0 (Windows NT 10.0; Win64; x64\0\n";

HandlerPool::~HandlerPool()
{
	running = false;
	cout << "HandlerPool stoped!\n";
}
