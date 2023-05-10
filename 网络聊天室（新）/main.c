#include "head.h"

int main(int argc, char* argv[])
{
    // ./charroom_epoll_sever 0.0.0.0 2333
    ARGS_CHECK(argc, 3);
    int sockfd;
    tcp_init(&sockfd, argv[1], argv[2]);

    int epfd = epoll_create(1);
    struct epoll_event event;
    event.data.fd = sockfd;
    event.events = EPOLLIN;
    epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &event);
    NetfdList* netfdList = (NetfdList*)calloc(1, sizeof(NetfdList));

    char buf[4096];
    while (1){
        struct epoll_event readyArr[1024];
        int readyNum = epoll_wait(epfd, readyArr, 1024, 1000);
        for (int i = 0; i < readyNum; ++i){
            if (readyArr[i].data.fd == sockfd){
                handle_accept(epfd, sockfd, netfdList, &event);
            }
            else {
                int fd = readyArr[i].data.fd;
                handle_recv_send(epfd, netfdList, fd, buf);
            }
        }
        timeout(epfd, netfdList);
    }
    
    return 0;
}


