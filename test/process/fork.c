#include <stdio.h>
#include <unistd.h>
#include<sys/types.h>

int main(int argc, char const *argv[])


{
    /* code */
    /**
     * 主进程
     * pid<0 创建失败
     * pid==0 子进程
     * pid>0 父进程(子进程的pid)
     */

    printf("主进程(%d)开始执行\n",getpid());


    //fork创建子进程
    int pid = fork();
    if(pid < 0)
    {
        printf("创建子进程失败\n");
        return -1;
    }
    else if(pid == 0)
    {
        printf("子进程(%d)开始执行,父进程id:%d\n",getpid(),getppid());
        printf("子进程(%d)结束执行,父进程id: %d\n",getpid(),getppid());
    }
    else
    {
        printf("父进程(%d)开始执行,子进程id:%d\n",getpid(),pid);
        printf("父进程(%d)结束执行,子进程id:%d\n",getpid(),pid);
    }

    printf("进程结束\n");
    return 0;
}
