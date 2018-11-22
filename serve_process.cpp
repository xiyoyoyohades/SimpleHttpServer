#include "inc.hpp"
#pragma warning(disable : 4996)
char error_404[] = "HTTP/1.1 404 Not Found\r\nContent-Type: \
text/html\r\nContent-Length: 78\r\n\r\n<HTML>\r\n<BODY>\r\nSorry, the page you requested was not found.\
\r\n</BODY>\r\n</HTML>\r\n\0";

std::string file_path = "F:/ServerTest";
int flag=0;
void connect_heap::serve(socket_wrap toserve, int i, int j){
	flag = 0;
    while(switcher){
		//printf("start process\n");
        char* buf=new char[maxline+1];
        int rec=toserve.myreceive(maxline, buf);
		if (!switcher) {
			flag = 1;
			break;
		}
		try {
			if (!rec) throw std::logic_error("[INFO] connection closed");
		}
		catch (std::logic_error clo) {
			printf("[INFO] connection from %s : %d closed\n", inet_ntoa(toserve.addr.sin_addr), toserve.addr.sin_port);
			break;
		}
		
        std::smatch header;
        std::regex regex_mask("([A-Za-z]+) +(.*) +(HTTP/[0-9][.][0-9])");
        std::string post(buf);

		if (!std::regex_search(post, header, regex_mask)) {
			//flag = 1;
			break;
		}
        std::string operation=header[1];
        std::string req_content=header[2];
		
        std::cout<<"[INFO] "<<"Client from "<<inet_ntoa(toserve.addr.sin_addr)<<":"<<toserve.addr.sin_port<<" request to "<<operation<<" "<<req_content<<std::endl;

        std::smatch type;
        std::regex regex_type("\\..*");
        std::string content_type;
        if (!regex_search(req_content, type, regex_type))
        {
            printf("[ERROR] no file extension found, use default!");
            content_type = "text/plain";
        }else{
            if(type[0]==".html") content_type="text/html";
            else if(type[0]==".css") content_type="text/css";
            else if(type[0]==".xml") content_type="text/xml";
            else if(type[0]==".jpg") content_type="image/jpg";
            else if(type[0]==".jpeg") content_type="image/jpeg";
            else if(type[0]==".png") content_type="image/png";
			else if (type[0] == ".pdf") content_type = "application/pdf";
            else content_type = "text/plain";

        }
		//printf("%s\n", content_type.c_str());
        std::ifstream data_file(file_path+req_content, std::ios::binary);
        if(!data_file){
            toserve.mysend(sizeof(error_404), error_404);
            printf("[ERROR] %s not found, sent 404 error\n",req_content.c_str());
            break;
        }
        std::filebuf* data=data_file.rdbuf();
        int data_size=data->pubseekoff(0, data_file.end, data_file.in);
        data->pubseekpos(0, data_file.in);
        char* data_buf=new char[data_size];
        data->sgetn(data_buf, data_size);
        data_file.close();

		//printf("prepare to conduct\n");
        std::stringstream stream;
        stream<<"HTTP/1.1 200 OK" <<"\r\n"<< "Server: Ssz\r\n"<< "Content-Length: " << data_size <<"\r\n"
	    <<"Connection: keep-alive\r\n"<<"Content-Type: " << content_type << "\r\n\r\n";
		std::string temp = stream.str();
        const char* stream_buf=temp.c_str();
		//std::cout << temp<< std::endl;
        toserve.mysend(strlen(stream_buf),stream_buf);
        toserve.mysend(data_size, data_buf);
        stream.clear();
        delete []data_buf;
        delete []buf;
		printf("[INFO] %s processed\n",req_content.c_str());
    }
	if (!flag) {
		toserve.myclose();
		using_connects.delete_item(toserve.socket_id);
		printf("[INFO] connection from %s : %d closed\n", inet_ntoa(toserve.addr.sin_addr), toserve.addr.sin_port);
	}
	printf("[INFO] serve thread %d-%d exit\n",i ,j);
}
