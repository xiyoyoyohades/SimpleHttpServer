#include "headers.hpp"
#include "Listener.hpp"
#include "HandlerPool.hpp"

int main(int argc, char const *argv[])
{
    //start thread pool
    //get a port from conf
	
    unsigned short port=8080;

    //start listener thread
    Listener localListener(port);

    
    return 0;
}
