#include <func.h>

int main()
{
    pid_t pid;
    pid = fork();
    if (pid == 0){
        char* const args[] = {"./add_test", "3", "4", NULL};
        execv("add_test", args);
    } else {
        int wstatus;
        wait(&wstatus);
        if (WIFEXITED(wstatus)){
            printf("exit normally, exit code = %d\n", WEXITSTATUS(wstatus));
        } else if (WIFSIGNALED(wstatus)){
            printf("exit abnormally. exit code = %d\n", WTERMSIG(wstatus));
        }
    }
    return 0;
}

