#include "head.h"

int handle_accept(int epfd, int sockfd, NetfdList* netfdList, struct epoll_event* event){
    struct sockaddr_in clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);
    int netfd = accept(sockfd, (struct sockaddr*)&clientAddr, &clientAddrSize);
    printf("netfd = %d\n", netfd);
    printf("client ip = %s, port = %d\n", 
            inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
    printf("new client connects\n");
    add_netfd_list(netfdList, netfd);
    event->data.fd = netfd;
    event->events = EPOLLIN;
    epoll_ctl(epfd, EPOLL_CTL_ADD, netfd, event);
}

int handle_recv_send(int epfd, NetfdList* netfdList, int netfd, char* buf){
    bzero(buf, sizeof(buf));
    ssize_t sret = recv(netfd, buf, sizeof(buf), 0);
    if (sret == 0){
        del_netfd_list(netfdList, netfd);
        epoll_ctl(epfd, EPOLL_CTL_DEL, netfd, NULL);
        close(netfd);
        printf("Notice: a client left\n");
        printf("clients = %d\n", netfdList->size);
    }
    else {
        Node* cur = netfdList->head;
        while (cur){
            if (cur->netfd == netfd){
                cur->lastActive = time(NULL);
                cur = cur->next;
                continue;
            }
            else {
                send(cur->netfd, buf, sret, 0);
            }
            cur = cur->next;
        }
    }
}
