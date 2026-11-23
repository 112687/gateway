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
        char path[] = "/home/wfh/桌面/GateWay/test/process/other";
        char *argv[] = {path,"abc",NULL}; //参数数组最后必须为NULL
        execve(path,argv,NULL); 
       
    }
    else
    {
        printf("父进程(%d)开始执行,子进程id:%d\n",getpid(),pid);
        
    }

    printf("进程结束\n");
    return 0;
}
