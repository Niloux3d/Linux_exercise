#include <func.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 2);
    int fdr = open(argv[1], O_RDONLY);
    ERROR_CHECK(fdr, -1, "fdr open");
    char buf[1024] = {0};
    read(fdr, buf, sizeof(buf));
    printf("%s", buf);
    close(fdr);
    return 0;
}

