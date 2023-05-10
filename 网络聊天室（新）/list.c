#include "head.h"

int add_netfd_list(NetfdList* netfdList, int netfd){
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL){
        return -1;
    }
    newNode->next = NULL;
    newNode->netfd = netfd;
    newNode->lastActive = time(NULL);
    if (netfdList->size == 0){
        netfdList->head = netfdList->tail = newNode;
    }
    else {
        newNode->next = netfdList->head;
        netfdList->head = newNode;
    }
    ++netfdList->size;
    return 0;
}

int del_netfd_list(NetfdList* netfdList, int netfd){
    Node* prev = NULL;
    Node* cur = netfdList->head;
    while (cur){
        if (cur->netfd == netfd){
            if (prev == NULL){
                netfdList->head = cur->next;
            } else {
                prev->next = cur->next;
            }
            if (cur == netfdList->tail){
                netfdList->tail = prev;
            }
            free(cur);
            --netfdList->size;
            return 0;
        }
        prev = cur;
        cur = cur->next;
    }
    return -1;
}

