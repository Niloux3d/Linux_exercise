#include <func.h>

int main(int argc, char* argv[])
{
    const char query[] ="GET / HTTP/1.0\r\n"
                                "Host: www.baidu.com\r\n"
                                "\r\n";
    const char hostname[] = "www.baidu.com";
    struct hostent* phost = gethostbyname(hostname);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    addr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)phost->h_addr_list[0]));
    int ret = connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));
    ERROR_CHECK(ret, -1, "connect");

    char buf[4096];
    send(sockfd, query, sizeof(query), 0);
    sleep(5);
    recv(sockfd, buf, sizeof(buf), 0);
    printf("buf = %s\n", buf);

    return 0;
}

