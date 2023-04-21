#include <func.h>
int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,3);
    int fdw = open(argv[1],O_WRONLY);
    ERROR_CHECK(fdw,-1,"open fdw");
    int fdr = open(argv[2],O_RDONLY);
    ERROR_CHECK(fdr,-1,"open fdr");
    printf("chat is established!\n");
    char buf[4096];
    fd_set rdset;
    struct timeval timeout;
    time_t now1 = time(NULL);
    time_t now2 = time(NULL);

    while(1){
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO,&rdset);
        FD_SET(fdr,&rdset);
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        if (now1 - now2 > 10){
            break;
        }

        int ret = select(fdr+1,&rdset,NULL,NULL,&timeout);
        if(ret == 0){
            printf("超时\n");
            now1 = time(NULL);
            continue;
        } 
        if(FD_ISSET(STDIN_FILENO,&rdset)){
            memset(buf,0,sizeof(buf));
            ssize_t sret = read(STDIN_FILENO,buf,sizeof(buf));
            if(sret == 0){
                break;
            }
            write(fdw,buf,strlen(buf));
        }
        if(FD_ISSET(fdr,&rdset)){
            now2 = time(NULL);
            memset(buf,0,sizeof(buf));
            ssize_t sret = read(fdr,buf,sizeof(buf));
            if(sret == 0){
                break;
            }
            printf("buf = %s\n", buf);
        }
    }
    close(fdr);
    close(fdw);
    return 0;
}


