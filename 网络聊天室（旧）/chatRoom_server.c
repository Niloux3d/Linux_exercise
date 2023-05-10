#include <func.h>

#define MAX_CLIENT 10

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 3);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    int reuse = 1;
    int ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));
    ERROR_CHECK(ret, -1, "setsockopt");
    ret = bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
    ERROR_CHECK(ret, -1, "bind");
    listen(sockfd, 10);

    char buf[4096];
    fd_set rdset;
    FD_ZERO(&rdset);
    FD_SET(sockfd, &rdset);
    int netfd[MAX_CLIENT] = {0};
    int idx = 0;
    struct timeval timeout;
    time_t active_time[MAX_CLIENT] = {0};
    while (1){
        fd_set tempset = rdset;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        int fdNum = select(20, &tempset, NULL, NULL, &timeout);
        if (fdNum == 0){
            for (int i = 0; i < idx; ++i){
                if (time(NULL) - active_time[i] > 5){
                    printf("Client %d is inactive for too long, closing connection.\n", i);
                    printf("A client has left, clients = %d\n", idx-1);
                    close(netfd[i]);
                    FD_CLR(netfd[i], &rdset);
                    active_time[i] = 0;
                    --idx;
                }
            }
        }
        if (FD_ISSET(sockfd, &tempset)){
            if (idx == MAX_CLIENT){
                printf("chatRoom is full now, pls wait...\n");
                continue;
            }
            struct sockaddr_in clientAddr;
            socklen_t clientAddrSize = sizeof(clientAddr);
            int newfd = accept(sockfd, (struct sockaddr*)&clientAddr, &clientAddrSize);
            printf("newfd = %d\n", newfd);
            printf("client ip = %s, port = %d\n", 
                   inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
            netfd[idx] = newfd;
            ++idx;
            FD_SET(newfd, &rdset);
            active_time[idx] = time(NULL);
        }
        for (int i = 0; i < idx; ++i){
            if(FD_ISSET(netfd[i],&tempset)){
                bzero(buf, sizeof(buf));
                ssize_t sret = recv(netfd[i], buf, sizeof(buf), 0);
                if (sret == 0){
                    close(netfd[i]);
                    --idx;
                    printf("A client has left, clients = %d\n", idx);
                    FD_CLR(netfd[i], &rdset);
                    active_time[i] = 0;
                    for (int j = i; j < idx; ++j) {
                        netfd[j] = netfd[j+1];
                    }
                } else {
                    active_time[i] = time(NULL);
                    for (int j = 0; j < idx; ++j){
                        if (netfd[j] != netfd[i]) {
                            send(netfd[j], buf, sret, 0);
                        }
                    }
                }
            }
        }
    }
    return 0;
}

