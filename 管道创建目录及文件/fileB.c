#include <func.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 2);
    int fdr = open(argv[1], O_RDONLY);
    ERROR_CHECK(fdr, -1, "open fdr");
    mkdir("./Storage", 0777);
    chdir("./Storage");
    char buf[4096] = {0};
    int length;
    read(fdr, &length, sizeof(int));
    read(fdr, buf, length);

    int fdw = open(buf, O_WRONLY | O_CREAT, 0666);
    ERROR_CHECK(fdw, -1, "open fdw");
    memset(buf, 0, sizeof(buf));
    read(fdr, &length, sizeof(int));
    ssize_t sret = read(fdr, buf, length);
    printf("sret = %ld\n", sret);
    write(fdw, buf, sret);
    close(fdw);
    close(fdr);
    return 0;
}

