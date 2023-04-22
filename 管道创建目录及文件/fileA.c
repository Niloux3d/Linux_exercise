#include <func.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 3);
    int fdr = open(argv[1], O_RDONLY);
    ERROR_CHECK(fdr, -1, "open fdr");
    int fdw = open(argv[2], O_WRONLY);
    ERROR_CHECK(fdw, -1, "open fdw");

    int length = strlen(argv[1]);
    write(fdw, &length, sizeof(int));
    write(fdw, argv[1], length);

    char buf[4096] = {0};
    ssize_t sret = read(fdr, buf, sizeof(buf));
    length = sret;
    write(fdw, &length, sizeof(int));
    write(fdw, buf, length);
    close(fdw);
    close(fdr);
    
    return 0;
}
