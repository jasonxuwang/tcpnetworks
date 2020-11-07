#include "TCPSocket.h"
#include "Epoll.h"

int main(){

        TCPSocket tsock;
        tsock.as_server(PORT);

    return 0;
}