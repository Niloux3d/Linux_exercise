#pragma once
#include <func.h>

typedef struct node{
    int netfd;
    struct node* next;
    time_t lastActive;
} Node;

typedef struct list{
    Node* head;
    Node* tail;
    int size;
} NetfdList;

int add_netfd_list(NetfdList* netfdList, int netfd);
int del_netfd_list(NetfdList* netfdList, int netfd);
int tcp_init(int *psockfd, const char* ip, const char* port);
int handle_accept(int epfd, int sockfd, NetfdList* netfdList, struct epoll_event* event);
int handle_recv_send(int epfd, NetfdList* netfdList, int netfd, char* buf);
int timeout(int epfd, NetfdList* netfdList);
