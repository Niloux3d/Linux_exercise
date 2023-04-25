#include <func.h>

int main()
{
    printf("process begins\n");
    if (fork() != 0){
        printf("I am parent, pid = %d, ppid = %d\n", getpid(), getppid());
    } else {
        printf("I am child, pid = %d, ppid = %d\n", getpid(), getppid());
    }
    return 0;
}

