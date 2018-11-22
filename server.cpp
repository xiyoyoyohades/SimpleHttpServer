#include "inc.hpp"
extern int on;
extern int flag;
extern std::string file_path;
std::string custom_ip="0.0.0.0";
int custom_ip_set = 0;

int main(int argc, char const *argv[])
{
    unsigned short port=8181;
    int thread_num=2;
	char c = '\0';
	listener* main_server = nullptr;
	bool running = false;

	printf("A Simple Http Server -- developed by ssz\n\n");
	printf("Guide:\n");
	printf("press s to start service\n");
	printf("press c to stop service\n");
	printf("press q to quit\n\n");
	printf("now press any key to continue...");
	_getch(); _getch();
	system("cls");

	printf("use custom listen ip?: (y/n)\n");
	c = _getch();  _getch();
	if (c == 'n') printf("using default ip: 0.0.0.0\n");
	else {
		printf("custom listen ip: ");
		std::cin >> custom_ip;
		custom_ip_set = 1;
	}
	c = '\0';

	printf("use custom listen port?: (y/n)\n");
	c = _getch(); _getch();
	if (c == 'n') printf("using default port: 8181\n");
	else {
		printf("custom listen port: ");
		std::cin >> port;
	}
	c = '\0';

	printf("use custom file path?: (y/n)\n");
	c = _getch(); _getch();
	if (c == 'n') printf("using default file path: F:/ServerTest\n");
	else {
		printf("custom file path: ");
		std::cin >> file_path;
	}

	try {
		if (custom_ip != "0.0.0.0" && custom_ip != "127.0.0.1") {
			throw std::logic_error("[ERROR] invalid IP address, may not work");
		}
	}
	catch (std::logic_error caught) {
		printf("%s\n", caught.what());
		return 0;
	}

	c = '\0';
	std::cout << std::endl;
	printf("[STATUS] server pending.....waiting for command\n");

	
	while (1)
	{
		c=_getch();
		if (c == 's') {
			if (!running) {
				on = 1;
				flag = 0;
				main_server = new listener(port, thread_num);
				running = true; 
				printf("[STATUS] server started\n");
				continue;
			}
			else {
				printf("[INFO] server is already running\n");
			}
			
		}
		else if (c == 'c') {
			if (running) {
				main_server->close_listener();
				on = 0;
				flag = 0;
				delete main_server;
				main_server = nullptr;
				running = false;
				Sleep(500);
				printf("[STATUS] server closed\n\n");
				printf("[STATUS] server pending...waiting for command\n");
				continue;
			}
			else {
				printf("[INFO] the server is not running\n");
				continue;
			}
		}
		else if (c == 'q') {
			if (running) {
				main_server->close_listener();
				on = 0;
				flag = 0;
				delete main_server;
				main_server = nullptr;
				running = false;
				Sleep(500);
				printf("[STATUS] server closed\n\n");
				break;
			}
			else {
				break;
			}
		}
		else {
			continue;
		}
	}
	printf("[INFO] server quit, press any key to exit\n");
	_getch(); _getch();

	/*
    getchar();
		listener server(port, thread_num);
	//auto serverp = new listener(port, thread_num);
    getchar();
		server.close_listener();
	//serverp->close_listener();
	//delete serverp;
	//getchar();
	//auto serverpp = new listener(port, thread_num);
	getchar();
	*/
    return 0;
}
