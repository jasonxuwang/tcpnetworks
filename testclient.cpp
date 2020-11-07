#include "TCPSocket.h"
#include "Epoll.h"

int main(){
        TCPSocket tsock;
        tsock.as_client(IPSTR,PORT);

        while (1){

            std::cout << "Input: ";
            char* buff;
            memset(buff, '\0', sizeof(buff));
            gets(buff);
            tsock.twrite(buff,strlen(buff));

        }

        return 0;
}