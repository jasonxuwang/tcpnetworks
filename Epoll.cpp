#include "Epoll.h"
//This epoll class handles the interaction with epoll object


Epoll::Epoll(){

}

Epoll::~Epoll(){
    
}

int32_t Epoll::epoll_init(int32_t iTimeout, int32_t iMaxEvent ){
    m_timeout = iTimeout;
    m_max_event = iMaxEvent;

    memset(&m_event, 0, sizeof(m_event));
    m_event.events = EPOLLIN | EPOLLERR |EPOLLHUP;
    m_event.data.ptr = NULL;
    m_event.data.fd  = -1;
    m_events = (struct epoll_event *) malloc(m_max_event * sizeof(struct epoll_event));
    if (NULL == m_events) {
        return -1;
    }

    // create epoll fd
    m_epoll_fd = epoll_create(m_max_event);
	if (m_epoll_fd < 0){
		perror("epoll_create: creation failed");
        exit(EXIT_FAILURE);
	}   
    std::cout << "Epoll created ! " << m_epoll_fd << std::endl;
    return m_epoll_fd;
}

int32_t Epoll::epoll_add(int32_t iSockFd){
        m_event.events = EPOLLIN;
        m_event.data.fd = iSockFd;
        return epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, iSockFd, &m_event);


}

int32_t Epoll::wait(){
    int numfd = epoll_wait(m_epoll_fd, m_events ,m_max_event, m_timeout);
	if (numfd < 0){
			perror("epoll_wait: epoll wait error");
            exit(EXIT_FAILURE);
	}
    return numfd;
}
epoll_event* Epoll::get_event_by_id(int32_t iId){
    
    return &m_events[iId];
}


int32_t Epoll::epoll_close(int32_t iSockFd){
    return epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, iSockFd, &m_event);
}


