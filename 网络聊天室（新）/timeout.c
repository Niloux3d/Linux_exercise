#include "head.h"

int timeout(int epfd, NetfdList* netfdList){
    time_t curtime = time(NULL);
    printf("curtime = %s\n", ctime(&curtime));
    Node* cur = netfdList->head;
    Node* prev = NULL;
    while (cur){
        if (curtime - cur->lastActive > 5){
            if (prev == NULL){
                netfdList->head = cur->next;
                if (netfdList->size == 1){
                    netfdList->tail = NULL;
                }
            }
            else {
                prev->next = cur->next;
                if (cur->next == NULL){
                    netfdList->tail = prev;
                }
            }
            epoll_ctl(epfd, EPOLL_CTL_DEL, cur->netfd, NULL);
            close(cur->netfd);
            free(cur);
            --netfdList->size;
            printf("timeout: a client was kicked\n");
            printf("clients = %d\n", netfdList->size);
            break;
        }
        prev = cur;
        cur = cur->next;
    }
    return 0;
}
