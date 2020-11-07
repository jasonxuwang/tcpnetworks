#include "Epoll.h"
#include <cstring>
#include <cstdio>

Epoll::Epoll() {
}
Epoll::~Epoll() {
	close(m_epoll_fd);
	free(m_pevents);
}
int32_t Epoll::epoll_init(int32_t size) {
	m_size = size;
	memset(&m_epoll_event,0,sizeof(m_epoll_event));//字符串用0填充
	m_epoll_event.events = EPOLLIN|EPOLLERR|EPOLLHUP;//??为啥要加err和hup？
	m_epoll_event.data.ptr = NULL;
	m_epoll_event.data.fd = -1;

	m_pevents = (struct epoll_event *) malloc(m_size * sizeof(struct epoll_event));//  a block of uninitialized memory and returns a void pointer to the first byte of the allocated memory block if the allocation succeeds.
																					//which equals to new !!!!!
																					//so it just initilized a space for m_size multiple epoll_event size

	if (m_pevents == NULL) {
		return -1;
	}

	m_epoll_fd = epoll_create(m_size);//epoll_create
	if (m_epoll_fd < 0) {
		return -1;
	}
	return 0;
}
int32_t Epoll::epoll_wait(int32_t timeout) {
	return ::epoll_wait(m_epoll_fd,m_pevents,m_size,timeout);//using epoll_waite in sys/epoll.h not the local function
}

int32_t Epoll::epoll_add(int32_t fd) { // add a new event into epoll by using epoll_clt function
	if (fd < 0) {
		return -1;
	}
	m_epoll_event.data.fd = fd;
	return epoll_ctl(m_epoll_fd,EPOLL_CTL_ADD,fd,&m_epoll_event);
}

epoll_event* Epoll::get_event(int32_t idx) {//get the epoll_event of the particular number
	return &m_pevents[idx];
}


