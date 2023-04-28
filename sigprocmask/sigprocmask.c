#include <func.h>

void sigFunc(int signum){
    printf("signum = %d\n", signum);
}

int main()
{
    sigset_t set, oldset, pending;
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    sigprocmask(SIG_BLOCK, &set, &oldset);
    printf("block SIGINT\n");

    sleep(5);
    signal(SIGINT, sigFunc);
    sigpending(&pending);
    if (sigismember(&pending, SIGINT)){
        printf("sianal SIGINT is pending\n");
    } else {
        printf("No signal is pending\n");
    }

    sigprocmask(SIG_SETMASK, &oldset, NULL);
    printf("unblock SIGINT\n");
    return 0;
}

