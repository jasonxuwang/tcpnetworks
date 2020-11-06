#include "TCPSocket.h"

TCPSocket::TCPSocket(){
    
}


TCPSocket::~TCPSocket(){
    
}


int32_t TCPSocket::twrite(char* start, int32_t iLength){
    // 发送start中的数据， iLength这么长
    return send(m_socket_fd, start, iLength,0 );
}

int32_t TCPSocket::tread(char* start){
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
    tsocket();
    tconnect(ipstr, port);
}


int32_t TCPSocket::as_server(int32_t port){
        tsocket();
        tbind(INADDR_ANY,port);
        tlisten();
    
    return m_socket_fd;
}



int32_t TCPSocket::taccept(){
    return  accept(m_socket_fd, (struct sockaddr *) &m_client_addr, (socklen_t*) &addrlen);

}
int32_t TCPSocket::get_socket_fd(){
    return m_socket_fd;
}

void TCPSocket::set_socket_fd(int32_t i_socket_fd){
    m_socket_fd = i_socket_fd;
}


int32_t TCPSocket::tsocket(){
    m_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket_fd < 0){
        perror("tcp socket: socket creation failed");
        exit(EXIT_FAILURE);
    }
    return 0;
}

int32_t TCPSocket::tbind( char* ipstr,int port ){

    if (strcmp(ipstr, "ANY")){

    }


    addrlen = 0;
    memset(&m_socket_addr, 0, sizeof(m_socket_addr)); // empty memory
    // set ip, port and size
    m_socket_addr.sin_family = AF_INET;
    m_socket_addr.sin_addr.s_addr = htonl(ipstr);
    if (strcmp(ipstr, "ANY")){
        m_socket_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    inet_pton(AF_INET,ipstr,&m_socket_addr.sin_addr.s_addr); 
    socklen_t addr_size = socklen_t(sizeof(m_socket_addr));
    if (bind(m_socket_fd,  (const sockaddr*) &m_socket_addr ,addr_size) != 0) {
        perror("tcp socket: bind failed");
        exit(EXIT_FAILURE);
    }
    return 0;
}

int32_t TCPSocket::tlisten(){
    if (listen(m_socket_fd, SOCK_LISTEN_MAX)!= 0){
        perror("tcp socket: listen failed");
        exit(EXIT_FAILURE);
    }
    return 0;
}
int32_t TCPSocket::tconnect( char* ipstr,int port){
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


    


