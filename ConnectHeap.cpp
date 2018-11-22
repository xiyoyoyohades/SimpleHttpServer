#include "inc.hpp"
#pragma warning(disable : 4996)
int on=1;

connect_heap::connect_heap(){}
connect_heap::~connect_heap(){
	connect_heap::switcher = 0;
}

void connect_heap::init(){
    switcher=1;
    for(int i=0; i<thread_num; i++)
    {
        thread_pool.push_back(std::thread(&connect_heap::assign_serv, this, i));
		printf("[INFO] assigner thread %d created\n",i);
		thread_pool[i].detach();
    }
	printf("[STATUS] connect heap initialized\n");
}

void connect_heap::insert_connect(socket_wrap talk_soc){
    waiting_connects.enqueue(talk_soc);
	printf("[INFO] add a connection from %s : %d\n", inet_ntoa(talk_soc.addr.sin_addr), talk_soc.addr.sin_port);
}

void connect_heap::assign_serv(int i){
	int j = 0;
    while(switcher){
        socket_wrap custom=waiting_connects.dequeue();
		if (!switcher) break;
        std::thread serv_thread=std::thread(&connect_heap::serve, this, custom,i, j);
		using_connects.insert_item(custom.socket_id);
		printf("[INFO] serve thread %d-%d assigned\n",i ,j );
		j++;
        serv_thread.detach();
    }
	printf("[INFO] assigner thread %d exit\n",i);
}

/*
void connect_heap::serve(int Id){

}
*/

void connect_heap::close(){
    connect_heap::switcher=0;
	on = 0;
	int toclose;
	for (int i = 0; i < thread_num; i++) {
		waiting_connects.notify();
	}
	while (!using_connects.empty()) {
		//
		//printf("1\n");
		toclose = using_connects.pop();
		closesocket(toclose);
	}

	
	printf("[STATUS] connect heap closed\n");
}

void connect_heap::set_thread_num(int num){
    thread_num=num;
}