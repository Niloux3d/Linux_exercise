#include <func.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 2);
    char filename[256] = {0};
    char filecontent[4096] = {0};
    int filelen;

    int fdr = open(argv[1], O_RDONLY);
    ERROR_CHECK(fdr, -1, "open fdr");

    read(fdr, &filelen, sizeof(filelen));
    read(fdr, filename, sizeof(filename));
    read(fdr, filecontent, sizeof(filecontent));

    mkdir("dir", 0777);
    chdir("dir");
    int fdw = open(filename, O_WRONLY | O_CREAT, 0666);
    ERROR_CHECK(fdw, -1, "open fdw");
    write(fdw, filecontent, strlen(filecontent));
    close(fdw);
    close(fdr);

    
    return 0;
}

