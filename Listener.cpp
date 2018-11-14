#include "inc.hpp"

listener::listener(unsigned short port, int thread_num){
    switcher=1;
    connect_heap.thread_num=thread_num;
	//printf("create listener %d \n",port);
    std::thread wiretapper=std::thread(&listener::start_listen, this, port);
    wiretapper.detach();
}

listener::~listener(){}

void listener::start_listen(unsigned short port){
    socket_wrap* acc=nullptr;
    connect_heap.init();
    listen_soc.mybind(port);
    listen_soc.mylisten(maxwait);
	printf("[STATUS] listener started, prepare to accept\n");
    while(switcher){
        acc=new socket_wrap(-233);
        listen_soc.myaccept(*acc);
		try {
			if (!(acc->socket_id)) throw std::logic_error("[INFO] invalid socket received");
		}
		catch(std::logic_error bad_socket){
			printf("%s\n", bad_socket.what());
			delete acc;
			continue;
		}
		if (!switcher) break;
        connect_heap.insert_connect(*acc);
        delete acc;

    }
	printf("[INFO] listener thread exit\n");
}

void listener::close_listener(){
    listener::switcher=0;
	listen_soc.myclose();
	printf("[INFO] listener closed\n");
	connect_heap.close();
    //connect_heap.switcher=0;
}