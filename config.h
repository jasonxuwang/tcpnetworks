
#include <stdint.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <stdlib.h>
#include <signal.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <netinet/tcp.h>
#include <net/if.h>
#include <dirent.h>
#include <dlfcn.h>
#include <sys/select.h>
#include <assert.h>
#include <string>
#include <sys/epoll.h>
#include <map>
#include <iostream>



#define SOCK_LISTEN_MAX 20
#define PORT 10009
#define TIMEOUT 1000
#define MAXEVENT 100
#define BUFFSIZE 1024
#define IPSTR "127.0.0.1"




