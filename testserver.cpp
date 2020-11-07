#include "TCPSocket.h"
#include "Epoll.h"

int main(){

        TCPSocket m_Sock;
        Epoll m_Epoll;
        struct epoll_event * m_epoll_event;
        std::map<int32_t, TCPSocket> m_user_map;

        // set a server, listening on PORT
        if (m_Sock.as_server(PORT) < 0 ){
            return -1;
        }

        // add sock fd to epoll
        m_Epoll.epoll_add(m_Sock.get_socket_fd());

        // loop 1
        while(1){

            // poll once 
            int numfd = m_Epoll.wait();


            for (int i =0; i < numfd ; i++){
                m_epoll_event  = m_Epoll.get_event_by_id(i); // get event from epoll
                if (m_epoll_event->data.fd == m_Sock.get_socket_fd()){
                    // if new client
                    int32_t conn_sock = m_Sock.taccept();
                    std::cout << "new client ! \n";

                    TCPSocket new_sock;
                    new_sock.set_socket_fd(conn_sock);
                    m_user_map[conn_sock] = new_sock;

                    // set non blocking 
				    int flags = fcntl(conn_sock, F_GETFL, 0);
				    if (fcntl(conn_sock, F_SETFL, flags | O_NONBLOCK) < 0){
					    std::cout << "set non blocking failed \n";
				    }

                    // epoll listening on connected socket
                    if (m_Epoll.epoll_add(conn_sock) < 0){
                        std::cout << "epoll add error for socket " << conn_sock << std::endl;
                        continue;
                    }
                }else {
                    // if new message from new client
                    // read message 
                    char * buff;
                    int totLen = m_user_map[m_epoll_event->data.fd].tread(buff);
                    if ( totLen>0){
                        // broadcast to all users
                        std::map<int32_t, TCPSocket>::iterator iter = m_user_map.begin();
                        while (iter != m_user_map.end()){
                            iter->second.twrite(buff,totLen);
                        }

                    }else{
                        std::cout <<"client leave!";
                    }
                }

            }

        }





    return 0;
}