#include <func.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 3);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);

    int ret = connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));
    ERROR_CHECK(ret, -1, "connect");

    fd_set rdset;
    char buf[4096];
    while (1){
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO, &rdset);
        FD_SET(sockfd, &rdset);
        select(sockfd+1, &rdset, NULL, NULL, NULL);
        if (FD_ISSET(STDIN_FILENO, &rdset)){
            bzero(buf, sizeof(buf));
            ssize_t sret = read(STDIN_FILENO, buf, sizeof(buf));
            if (sret == 0){
                send(sockfd, "break", 5, 0);
                break;
            }
            send(sockfd, buf, sret, 0);
        }
        if (FD_ISSET(sockfd, &rdset)){
            bzero(buf, sizeof(buf));
            ssize_t sret = recv(sockfd, buf, sizeof(buf), 0);
            if (sret == 0){
                break;
            }
            printf("buf = %s\n", buf);
        }
    }
    close(sockfd);
    return 0;
}

