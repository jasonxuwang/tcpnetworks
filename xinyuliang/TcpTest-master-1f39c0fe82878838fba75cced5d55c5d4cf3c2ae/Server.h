#pragma once
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
#include "util.h"

#define MAXEVENTS 64
void run();