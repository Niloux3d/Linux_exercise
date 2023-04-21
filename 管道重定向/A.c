#include <func.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 2);
    printf("Helloworld\n");
    int fdw = open(argv[1], O_WRONLY);
    ERROR_CHECK(fdw, -1, "fdw open");
    // 重定向
    int newfd = 10;
    dup2(STDOUT_FILENO, newfd);
    dup2(fdw, STDOUT_FILENO);
    printf("Helloworld\n");
    dup2(newfd, STDOUT_FILENO);
    printf("Helloworld\n");
    close(fdw);
    return 0;
}

