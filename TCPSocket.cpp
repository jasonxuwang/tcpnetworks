#include "TCPSocket.h"

TCPSocket(){
    
}


~TCPSocket(){
    
}


int32_t TCPSocket::write(char* start, int32_t iLength){
    // 发送start中的数据， iLength这么长
    return send(m_socket_fd, start, iLength,0 );
}

int32_t TCPSocket::read(char* start){
    // 从自身内核的数据中读取字节流，读完为止.
    int32_t tLength, totLength = 0;
    while (1){
        tLength = recv(m_socket_fd, start+totLength, BUFFSIZE, 0); //读当前数据
        if (tLength <=0){ // 
            break;
        }
        totLength+=tLength; //向后移动指针
    }
    return totLength;
}

int32_t TCPSocket::as_client(char* ipstr,int32_t port){
    socket();
    connect(ipstr, port);
}


int32_t TCPSocket::as_server(int32_t port){
    socket();
        bind(INADDR_ANY,port);
        listen();
    }
    return m_socket_fd;
}



int32_t TCPSocket::accept_conn(){
    return  accept(m_socket_fd, (struct sockaddr *) &m_client_addr, (socklen_t*) &addrlen);

}
int32_t TCPSocket::get_socket_fd(){
    return m_socket_fd;
}

void TCPSocket::set_socket_fd(int32_t i_socket_fd){
    m_socket_fd = i_socket_fd;
}


int32_t TCPSocket::socket(){
    m_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket_fd < 0){
        perror("tcp socket: socket creation failed");
        exit(EXIT_FAILURE);
    }
    return 0;
}

int32_t TCPSocket::bind( char* ipstr,int port ){
    addrlen = 0;
    memset(&m_socket_addr, 0, sizeof(m_socket_addr)); // empty memory
    // set ip, port and size
    m_socket_addr.sin_family = AF_INET;
    m_socket_addr.sin_addr.s_addr = htonl(ipstr);
    m_socket_addr.sin_port = (u_short)htons(port);
    socklen_t addr_size = socklen_t(sizeof(m_socket_addr));
    if (bind(m_socket_fd,  (const sockaddr*) &m_socket_addr ,addr_size) != 0) {
        perror("tcp socket: bind failed");
        exit(EXIT_FAILURE);
    }
    return 0;
}

int32_t TCPSocket::listen(){
    if (listen(m_socket_fd, SOCK_LISTEN_MAX)!= 0){
        perror("tcp socket: listen failed");
        exit(EXIT_FAILURE);
    }
    return 0;
}
int32_t TCPSocket::connect( char* ipstr,int port){
    bzero(&m_client_addr,sizeof(m_client_addr));
	m_client_addr.sin_family = AF_INET;
	m_client_addr.sin_port = htons(port);
	inet_pton(AF_INET,ipstr,&m_client_addr.sin_addr.s_addr); 
    int res = connect(m_socket_fd,(struct sockaddr *)&m_client_addr,sizeof(m_client_addr));	
    if (res < 0){
        perror("tcp socket: connection failed");
        exit(EXIT_FAILURE);    
    }
    return 0;
}


    


