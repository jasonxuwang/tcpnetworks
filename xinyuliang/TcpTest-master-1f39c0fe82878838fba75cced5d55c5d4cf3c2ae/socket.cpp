#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <errno.h>
#include <arpa/inet.h>

#define MAXEVENTS 64

void run();

int main()
{
    run();
    return 0;
}
void run()
{

    //create socket
    int listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    printf("listen:%d\n", listen_fd);
    if (listen_fd == -1)
    {
        return;
    }

    //bind the socket with port and ip address
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));           //fill up all the bytes with 0
    serv_addr.sin_family = AF_INET;                     //using ipv4
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //specify the ipv4 addr
    serv_addr.sin_port = htons(1234);                   //specify the port, like 8080 or 1234
    int bind_ret = bind(listen_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    printf("bind ret:%d\n", bind_ret);
    if (bind_ret < 0)
    {
        return;
    }

    //start listening with specifice socket, using listen_fd to sepcify it.
    int listen_ret = listen(listen_fd, 20);
    printf("listen ret:%d\n", listen_ret);

    int epoll_fd = epoll_create(10); //epoll_create, create pool for epoll_event
    if (epoll_fd < 0) {
		return;
	}
    struct epoll_event event;
    event.data.fd = listen_fd;
    event.events = EPOLLIN;

    int ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &event);
    if (-1 == ret)
    {
        printf("epoll ctl error\n");
        return;
    }

    struct epoll_event *events = (struct epoll_event *)malloc(sizeof(event) * MAXEVENTS);

    while (1)
    {
        int n = epoll_wait(epoll_fd, events, MAXEVENTS, 1);
        //printf("readable fd count:%d\n", n);
        int i = 0;
        for (i = 0; i < n; ++i)
        {
            int fd = events[i].data.fd;
            int fd_events = events[i].events;
            if ((fd_events & EPOLLERR) ||
                (fd_events & EPOLLHUP) ||
                (!(fd_events & EPOLLIN)))
            {
                printf("fd:%d error\n", fd);
                close(fd);
                continue;
            }
            else if (listen_fd == fd)
            {
                //while(1)
                //{
                // accept
                struct sockaddr client_addr;
                socklen_t client_addr_len = sizeof(client_addr);
                int new_fd = accept(listen_fd, &client_addr, &client_addr_len);
                printf("new socket:%d\n", new_fd);
                //send string to client
                char str[] = "epoll_start";
                write(new_fd, str, sizeof(str));
                if (new_fd < 0)
                {
                    printf("new socket error\n");
                    return;
                }
                // set nonblock
                // to do...
                //
                event.data.fd = new_fd;
                event.events = EPOLLIN;
                int ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_fd, &event);
                if (-1 == ret)
                {
                    printf("epoll ctl error\n");
                    return;
                }
                //}
            }
            else
            {
                //while(1)
                //{
                // send/recv
                char data[1024] = {0};
                if (ret = recv(fd, data, 1024, 0) > 0)
                {
                    send(fd, data, strlen(data), 0);
                    memset(data, 0, 1024);
                }
                //end
                if (ret == 0)
                {
                    printf("socket:%d, closed\n", fd);
                    close(fd);
                    break;
                }
                else if (ret == -1)
                {
                    //if (errno == EAGAIN)
                    //{
                    //    break;
                    //}
                    printf("recv failed\n");
                }
                //}
            }
        }
    }

    return;
}
