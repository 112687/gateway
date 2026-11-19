
#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    /* code */
    printf("other程序进程(%d)开始执行\n,argv[1]=%s\n",getpid(),argv[1]);
    return 0;
}
