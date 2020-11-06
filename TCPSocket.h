#include "config.h"

class TCPSocket{


    public:
        TCPSocket();
        ~TCPSocket();

        int32_t as_server(int32_t port);
        int32_t as_client(char* ipstr,int32_t port);
        int32_t get_socket_fd();
        void set_socket_fd(int32_t i_socket_fd)
        // wrapper for socket functions

        int32_t read(char* start);
        int32_t write(char* start, int32_t iLength);

        int32_t  socket();
        int32_t  bind( char* ipstr,int port );
        int32_t  listen();
        int32_t  connect( char* ipstr,int port );
        int32_t accept_conn();


    private:
        int32_t m_socket_fd, addrlen;
        char recvbuf[BUFFSIZE], sendbuf[BUFFSIZE];
        struct sockaddr_in m_socket_addr;
	    struct sockaddr_in m_client_addr;
};
