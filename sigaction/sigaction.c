#include <func.h>

void sigFunc(int signum, siginfo_t *info, void* p){
    printf("before sigum = %d\n", signum);
    sleep(5);
    sigset_t pending;
    sigpending(&pending);
    if (sigismember(&pending, SIGQUIT)){
        printf("SIGQUIT is pending\n");
    }
    printf("received signal from PID: %d\n", info->si_pid);
}

int main()
{
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_sigaction = sigFunc;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGQUIT);
    act.sa_flags = 0;
    sigaction(SIGINT, &act, NULL);
    while (1);
    return 0;
}

