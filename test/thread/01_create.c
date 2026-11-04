#define _GNU_SOURCE
#include<pthread.h>

#include<stdio.h>
#include<unistd.h>
//first线程的执行函数
void *first_thread_func(void *arg)
{
    printf("first线程(%d)开始执行\n", gettid());
    return NULL;
    
}

void *second_thread_func(void *arg)
{
    printf("second线程(%d)开始执行\n", gettid());
    return NULL;
    
}
int main(int argc, char const *argv[])
{
    printf("主线程(%d)\n", gettid());
    //
    //创建分线程
    pthread_t pt1,pt2;

    pthread_create(&pt1,NULL,first_thread_func,NULL);
    pthread_create(&pt2,NULL,second_thread_func,NULL);


    //等待分线程结束运行
    pthread_join(pt1,NULL);
    pthread_join(pt2,NULL);



    // 主线程结束运行


    printf("主线程(%d)结束运行,pt1=%ld,pt2=%ld\n", gettid(),pt1,pt2);

    /* code */
    return 0;
}
