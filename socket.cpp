#include "inc.hpp"

#pragma warning(disable : 4996)
extern std::string custom_ip;
extern int custom_ip_set;

socket_wrap::socket_wrap(){
    WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(2, 0);
	WSAStartup(wVersionRequested, &wsaData);
    socket_id=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

socket_wrap::socket_wrap(int id){
    socket_id=id;
}

socket_wrap::~socket_wrap(){

}

void socket_wrap::mybind(unsigned short port){
    struct sockaddr_in servAddr;
	char optval = 1;
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	
		
	if (custom_ip_set) servAddr.sin_addr.s_addr = inet_addr(custom_ip.c_str());
	else servAddr.sin_addr.s_addr = INADDR_ANY;
	servAddr.sin_port = htons(port);
	setsockopt(socket_id, SOL_SOCKET, SO_REUSEADDR, (char* )&optval, sizeof(char));
	bind(socket_id, (struct sockaddr *)&servAddr, sizeof(servAddr));
    addr=servAddr;
	printf("[STATUS] listening on address: %s: %d\n", inet_ntoa(servAddr.sin_addr),port);
}

void socket_wrap::mylisten(int backlog){
    listen(socket_id, backlog);
}

void socket_wrap::myaccept(socket_wrap& talk){
    struct sockaddr_in cliaddr;
	memset(&cliaddr, 0, sizeof(cliaddr));
	int length = sizeof(cliaddr);
	SOCKET soc = accept(socket_id, (struct sockaddr *)&cliaddr, &length);
	printf("[INFO] get a socket from %s\n",inet_ntoa(cliaddr.sin_addr));

	talk.socket_id = soc;
    talk.addr=cliaddr;

}

void socket_wrap::myclose(){
    closesocket(socket_id);
}

int socket_wrap::mysend(int length, const char* buffer){
    int left_length=length;
    int sent=0;
    while(left_length){
        sent=send(socket_id, buffer, left_length, 0);
        left_length-=sent;
    }
    return left_length;
}

int socket_wrap::myreceive(int length, char* buffer){
    int left_length=length;
    int received=0;
	char* buf = buffer;
	
    
        received=recv(socket_id, buffer, left_length,0);
        left_length-=received;
		buf += received;
		*buf = '\0';
   
	
    return received;
}