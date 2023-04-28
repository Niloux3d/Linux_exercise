#include <func.h>

time_t realTime = 0;
time_t profTime = 0;

void sigFunc14(int signum){
    time_t now = time(NULL);
    printf("curtime = %s\n", ctime(&now));
    ++realTime;
}

void sigFunc2(int signum){
    printf("realTime use = %ld\n", realTime);
    printf("profTime use = %ld\n", profTime);
}

void sigFunc27(int signum){
    ++profTime;
}

int main(int argc, char* argv[])
{
    struct itimerval itimer1;
    itimer1.it_value.tv_sec = 1;
    itimer1.it_value.tv_usec = 0;
    itimer1.it_interval.tv_sec = 1;
    itimer1.it_interval.tv_usec = 0;
    signal(SIGALRM, sigFunc14);
    setitimer(ITIMER_REAL, &itimer1, NULL);

    struct itimerval itimer2;
    itimer2.it_value.tv_sec = 1;
    itimer2.it_value.tv_usec = 0;
    itimer2.it_interval.tv_sec = 1;
    itimer2.it_interval.tv_usec = 0;
    signal(SIGPROF, sigFunc27);
    setitimer(ITIMER_PROF, &itimer2, NULL);

    signal(SIGINT, sigFunc2);
    while (1);

    return 0;
}

