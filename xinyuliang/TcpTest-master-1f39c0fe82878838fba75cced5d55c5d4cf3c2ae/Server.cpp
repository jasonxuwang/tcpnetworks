#include "Server.h"
#include "Epoll.h"
#include "TcpSocket.h"
#include "Server.h"
#include "Message.pb.h"
#include <time.h> 

void initial();
void recv_msg();

int main() {
	initial();
	recv_msg();
	return 0;
}

struct stSocketInfo {
	TCPSocket* tcpsocket;
	int32_t m_socket_fd;
	enmFdType  m_fd_type;
};
TCPSocket* socketptr;
Epoll epoll;
stSocketInfo SocketArr[max_socket_count];
void initial() {

	socketptr = new TCPSocket();//!!

	socketptr->initialize(common_buffer_size, common_buffer_size);
	if (socketptr->open_as_server(SERVER_PORT) < 0) {
		printf("m_ServerSocket->open_as_server failed\n");
		return;
	}
	if (epoll.epoll_init(max_socket_count) < 0) {
		printf("epoll_init failed\n");
		return;
	}
	if (epoll.epoll_add(socketptr->get_fd()) < 0) {
		printf("m_epoll.epoll_add fd:%d failed\n", socketptr->get_fd());
		return;
	}
	SocketArr[socketptr->get_fd()].tcpsocket = socketptr;
	SocketArr[socketptr->get_fd()].m_socket_fd = socketptr->get_fd();
	SocketArr[socketptr->get_fd()].m_fd_type = fd_type_listen;
}
void recv_msg() {
	while (true) {
		int32_t timeout = 1;
		int32_t count_for_msgs = epoll.epoll_wait(timeout);
		for (int i = 0; i < count_for_msgs; i++) {
			int32_t socketfd = epoll.get_event(i)->data.fd;
			TCPSocket* thisSocket = SocketArr[socketfd].tcpsocket;

			if (thisSocket == NULL || thisSocket->get_fd() < 0) {
				printf("[Player::%s] get_server_tcpsocket failed fd:%d\n", __FUNCTION__, socketfd);
				return;
			}
			else if (SocketArr[socketfd].m_fd_type == fd_type_listen) {
				int32_t accepted_fd = thisSocket->accept_fd(socketfd);
				if (accepted_fd < 0) {
					printf("accept_fd failed\n");
					continue;
				}
				SocketArr[accepted_fd].tcpsocket = new TCPSocket();
				SocketArr[accepted_fd].tcpsocket->initialize(common_buffer_size, common_buffer_size);
				SocketArr[accepted_fd].m_socket_fd = accepted_fd;
				SocketArr[accepted_fd].m_fd_type = fd_type_socket;

				char buffer[max_ss_package_size];
				memset(buffer, 0, max_ss_package_size);
				srand(time(NULL));

				ServerMsg s_msg;
				s_msg.set_id(rand());
				s_msg.set_uid(accepted_fd);
				s_msg.set_msg("connection created");

				int32_t msg_len = s_msg.ByteSize();
				char* lenptr = new char[4];//!!
				memcpy(lenptr, &msg_len, sizeof(int32_t));
				uint8_t str[2 * msg_len+5];
				printf("%d \n", msg_len);
				printf("char arr of len is %s \n",lenptr);
				memset(str, 0, 2 * msg_len+5);
				s_msg.SerializeToArray(str, msg_len);
				printf("%s \n",str);
				for (int i = 0; i < 4; i++) {
					buffer[i] = lenptr[i];
					
				}
				for (int i = 4; i < 4 + msg_len; i++) {
					buffer[i] = str[i - 4];
				}
				for (int i = 0; i < msg_len + 5; i++) {
					std::cout << buffer[i];
				}
				send(accepted_fd, buffer, msg_len + 5,0);

				if (epoll.epoll_add(accepted_fd) < 0) {
					printf("accepted_fd failed fd:%d \n", accepted_fd);
					return;
				}


			}
			else {
				char buffer[max_ss_package_size];
				memset(buffer, 0, max_ss_package_size);
				printf("start send back");
				if (recv(socketfd, buffer, sizeof(buffer),0) != 0) {

					int32_t c_msg_len = sizeof(buffer);
					int32_t pst_read = 0;

					//while (msg_len - pst_read > 0) {
						ClientMsg c_msg;
						int32_t this_length = 0;
						if (c_msg_len - pst_read < 4)
							return;
						memcpy(&c_msg_len, buffer, sizeof(int32_t));
						pst_read += 4;
						if (c_msg_len - pst_read < this_length)
							return;
						char temp[max_ss_package_size];
						memcpy(temp, buffer + 4, this_length);
						c_msg.ParseFromArray(buffer + pst_read, this_length);
						pst_read += this_length;

						ServerMsg s_msg;
						s_msg.set_id(rand());
						s_msg.set_uid((int)c_msg.uid());
						s_msg.set_msg(c_msg.act());

						char send_out[max_ss_package_size];
						int32_t s_msg_len = s_msg.ByteSize();
						char* lenptr = new char[4];//!!
						memcpy(lenptr, &s_msg_len, sizeof(int32_t));
						uint8_t str[2 * s_msg_len];

						memset(str, 0, 2 * s_msg_len);
						s_msg.SerializeToArray(str, s_msg_len);
						for (int i = 0; i < 4; i++) {
							send_out[i] = lenptr[i];
						}
						for (int i = 4; i < 4 + s_msg_len; i++) {
							send_out[i] = str[i - 4];
						}

						for (stSocketInfo sk : SocketArr) {
							if (sk.m_fd_type == fd_type_socket) {
								printf("send to client %d \n",sk.m_socket_fd);
								send(sk.m_socket_fd, send_out, 5 + s_msg_len,0);
							}
						}


					//}


				}
			}
		}
	}

}

