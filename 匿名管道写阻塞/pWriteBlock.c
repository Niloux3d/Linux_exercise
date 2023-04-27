#include <func.h>

int main(){
    int fds[2];
    pipe(fds);
    if (fork() == 0){
        close(fds[1]);
        while (1){
            char buf[4096] = {0};
            ssize_t sret = read(fds[0], buf, sizeof(buf));
            ERROR_CHECK(sret, -1, "read");
            sleep(1);
            printf("chlid read %ld bytes\n", sret);
            if (sret == 0){
                printf("child read block, exit\n");
                exit(1);
            }
        }
        close(fds[0]);
        exit(0);
    } else {
        close(fds[0]);
        char buf [4096] = {0};
        ssize_t total = 0;
        while (1){
            ssize_t sret = write(fds[1], buf, sizeof(buf));
            ERROR_CHECK(sret, -1,"write");
            total += sret;
            printf("parent write %ld bytes, total = %ld\n", sret, total);
        }
        close(fds[1]);
        exit(0);
    }
    return 0;
}
