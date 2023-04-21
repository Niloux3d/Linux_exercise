#include <func.h>
int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,3);
    int fdr = open(argv[1],O_RDONLY);
    ERROR_CHECK(fdr,-1,"open fdr");
    int fdw = open(argv[2],O_WRONLY);
    ERROR_CHECK(fdw,-1,"open fdw");
    printf("chat is established!\n");
    char buf[4096];
    fd_set rdset;
    while(1){
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO,&rdset);
        FD_SET(fdr,&rdset);
        select(fdr+1,&rdset,NULL,NULL,NULL);
        if(FD_ISSET(STDIN_FILENO,&rdset)){
            memset(buf,0,sizeof(buf));
            ssize_t sret = read(STDIN_FILENO,buf,sizeof(buf));
            if(sret == 0){
                break;
            }
            write(fdw,buf,strlen(buf));
        }
        if(FD_ISSET(fdr,&rdset)){
            memset(buf,0,sizeof(buf));
            ssize_t sret = read(fdr,buf,sizeof(buf));
            if(sret == 0){
                break;
            }
            printf("buf = %s\n", buf);
        }

    }
    close(fdw);
    close(fdr);
    return 0;
}

