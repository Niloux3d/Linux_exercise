#include <func.h>

int main()
{
    pid_t pid;
    pid = fork();

    if (pid == 0){
        printf("child process!\n");
    } else {
        printf("parent process!\n");
    }
    
    if (pid > 0){
        sleep(20);
        int status;
        waitpid(-1, &status, 0);
    }
    return 0;
}

