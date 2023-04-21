#include <func.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 2);
    char filename[256];
    char filecontent[4096];
    int filelen;

    printf("输入文件名:");
    scanf("%s", filename);
    printf("输入文件内容:");
    scanf("%s", filecontent);
    filelen = strlen(filecontent);
    
    int fdw = open(argv[1], O_WRONLY);
    ERROR_CHECK(fdw, -1, "open fdw");
    write(fdw, &filelen, sizeof(filelen));
    write(fdw, filename, sizeof(filename));
    write(fdw, filecontent, sizeof(filecontent));

    close(fdw);
    return 0;
}

