#ifndef _EPOLL_
#define _EPOLL_

#include "config.h"


class Epoll{
    public:

    private:
        int32_t m_epoll_fd;
        int32_t m_timeout, m_max_event;
        struct epoll_event m_event;
        struct epoll_event* m_events;
    public:
        // constructors and destructors
        Epoll();
        ~Epoll();
        int32_t epoll_init(int32_t iTimeout, int32_t iMaxEvent);
        int32_t epoll_add(int32_t iSockFd);
        int32_t wait();
        int32_t get_epoll_fd();
        epoll_event* get_event_by_id(int32_t iId);
        int32_t epoll_close(int32_t iSockFd);
    private:



};

#endif