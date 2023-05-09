#include <func.h>

typedef struct node {
    int netfd;
    time_t lastactive;
    struct node* next;
} Node;

typedef struct list {
    Node* head;
    Node* tail;
    int size;
} List;

List* create_list(){
    List* l = (List*)calloc(1, sizeof(List));
    return l;
}

void add_before_head(List* l, int netfd){
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->next = NULL;
    newNode->netfd = netfd;
    if (l->size == 0){
        l->head = l->tail = newNode;
    } else {
        newNode->next = l->head;
        l->head = newNode;
    }
    ++l->size;
}

void delete_node(List* l, int netfd){
    Node* curr = l->head;
    Node* prev = NULL;
    while (curr){
        if (curr->netfd == netfd){
            if (prev == NULL){
                l->head = curr->next;
                if (l->size == 1){
                    l->tail = NULL;
                }
                free(curr);
            }else {
                prev->next = curr->next;
                if (curr->next == NULL){
                    l->tail = prev;
                }
                free(curr);
            }
            --l->size;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 3);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    int reuse = 1;
    int ret = setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(int));
    ERROR_CHECK(ret,-1,"setsockopt");
    ret = bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
    ERROR_CHECK(ret, -1, "bind");
    listen(sockfd, 10);

    int epfd = epoll_create(1);
    struct epoll_event event;
    event.data.fd = sockfd;
    event.events = EPOLLIN;
    epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &event);
    time_t currtime;
    List* netArr = create_list();

    char buf[4096];
    while (1){
        struct epoll_event readyArr[1024];
        int readyNum = epoll_wait(epfd, readyArr, 1024, 1000);
        for (int i = 0; i < readyNum; ++i){
            if (readyArr[i].data.fd == sockfd){
                struct sockaddr_in clientAddr;
                socklen_t clientAddrSize = sizeof(clientAddr);
                int netfd = accept(sockfd, (struct sockaddr*)&clientAddr, &clientAddrSize);
                printf("netfd = %d\n", netfd);
                printf("client ip = %s, port = %d\n",
                       inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
                printf("new client connects\n");
                event.data.fd = netfd;
                event.events = EPOLLIN;
                epoll_ctl(epfd, EPOLL_CTL_ADD, netfd, &event);
                add_before_head(netArr, netfd);
                netArr->head->lastactive = time(NULL);
            }
            else {
                bzero(buf, sizeof(buf));
                ssize_t sret = recv(readyArr[i].data.fd, buf, sizeof(buf), 0);
                if (sret == 0){
                    int delfd = readyArr[i].data.fd;
                    delete_node(netArr, delfd);
                    epoll_ctl(epfd, EPOLL_CTL_DEL, delfd, NULL);
                    close(delfd);
                }
                int srcfd = readyArr[i].data.fd;
                Node* curr = netArr->head;
                while (curr){
                    if (curr->netfd == srcfd){
                        curr->lastactive = time(NULL);
                        continue;
                    }
                    else {
                        send(curr->netfd, buf, strlen(buf), 0);
                    }
                    curr = curr->next;
                }

            }
        }
        currtime = time(NULL);
        printf("currtime = %s\n", ctime(&currtime));
        Node* curr = netArr->head;
        Node* prev = NULL;
        while (curr){
            if (currtime - curr->lastactive > 5){
                if (prev == NULL){
                    netArr->head = curr->next;
                    if (netArr->size == 1){
                        netArr->tail = NULL;
                    }
                    epoll_ctl(epfd, EPOLL_CTL_DEL, curr->netfd, NULL);
                    close(curr->netfd);
                    free(curr);
                }
                else {
				    prev->next = curr->next;
				    if (curr->next == NULL) {
					    netArr->tail = prev;
				    }
                    epoll_ctl(epfd, EPOLL_CTL_DEL, curr->netfd, NULL);
                    close(curr->netfd);
				    free(curr);
			    }
                --netArr->size;
            }
            prev = curr;
            curr = curr->next;
        }
    }
    return 0;
}

