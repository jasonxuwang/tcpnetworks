#include "client.h"
#include "Message.pb.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h> 

void initialized();
void recv_messgaes_client();

Epoll epoll;

int main() {
	initialized();
	while (true) {
		recv_messgaes_client();
	}
	return 0;
}
int sock;
int con_return;
void initialized() {
	sock = socket(AF_INET, SOCK_STREAM, 0);

	//向服务器（特定的IP和端口）发起请求
	struct sockaddr_in serv_addr;

	memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
	serv_addr.sin_family = AF_INET;  //使用IPv4地址
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
	serv_addr.sin_port = htons(8888);  //端口

	con_return = connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if (con_return < 0) {
		printf("connection failed for  %s:%d ", SERVER_IP, SERVER_PORT);
		return;
	}
	int epoll_fd = epoll.epoll_init(max_socket_count);
	epoll.epoll_add(sock);
	epoll.epoll_add(0);
}
void recv_messgaes_client() {

	int count_fd = epoll.epoll_wait(1);
	std::cout << "num of count_fd: " << count_fd << std::endl;
	for (int i = 0; i < count_fd; i++) {
		std::cout << epoll.get_event(i)->data.fd << " ";
	}
	std::cout << "end round\n";

	for (int i = 0; i < count_fd; i++) {
		//读取服务器传回的数据
		int32_t socketfd = epoll.get_event(i)->data.fd;
		if (socketfd == sock) {
			char buffer[max_ss_package_size];
			memset(buffer, 0, max_ss_package_size);
			int32_t num_read = recv(sock, buffer, sizeof(buffer),0);
			if (num_read < 0) {
				return;
			}
			int buffer_size = sizeof(buffer);
			int pst_read = 0;
			// while (buffer_size - pst_read > 0) {
				ServerMsg s_msg;
				int length = 0;
				if (buffer_size - pst_read < 4)
					return;
				memcpy(&length, buffer, sizeof(int32_t));

				printf("length is : %d \n",length);
				if(length<=0)
					return;
				pst_read += 4;
				if (buffer_size - pst_read < length)
					return;
				s_msg.ParseFromArray(buffer+4, length);

				pst_read += length;

				buffer + length;
				printf("Message ID is : %d. User ID is :%d, do : %s \n",s_msg.id(),s_msg.uid(),s_msg.msg().c_str());
			// }
		}
		else if (socketfd == 0) {
			char buffer[max_ss_package_size];
			memset(buffer,0, max_ss_package_size);
			ClientMsg c_msg;
			gets(buffer);

			
			// printf("Please type in your Action : 1.Attack 2.Defend \n");
			// int act = 0;
			// std::cin >> act;
			if (strlen(buffer) >0 )
			{
				srand(time(NULL));
			c_msg.set_id(rand());
			c_msg.set_uid(sock);
			c_msg.set_act(std::string(buffer));
			// switch (act) {
			// case 1:
	
			// 	break;
			// case 2:
			// 	c_msg.set_act("Defend");
			// 	break;
			// default:
			// 	return;
			// }

			int32_t p_len = c_msg.ByteSize();
			char* lenptr = new char[4];//!!
			
			memcpy(lenptr, &p_len, sizeof(int32_t));
			uint8_t str[2 * p_len];

			memset(str, 0, 2 * p_len);
			c_msg.SerializeToArray(str, p_len);
			for (int i = 0; i < 4; i++) {
				buffer[i] = lenptr[i];
			}
			for (int i = 4; i < 4 + p_len; i++) {
				buffer[i] = str[i - 4];
			}
			for(int i = 0 ; i < 40 ; i++){
				std::cout<<buffer[i];
			}
			printf("send to server %d \n", sock);
			int send_return = send(sock, buffer, p_len+5,0);
			if(send_return < 0)
				printf("fail");
				return;

			}
			
		}
		else {
			return;
		}
		
	}
}


