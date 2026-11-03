#define _GNU_SOURCE
#include<pthread.h>

#include<stdio.h>
#include<unistd.h>


int main(int argc, char const *argv[])
{
    printf("主线程(%d)\n", gettid());
    //
    //创建分线程
    pthread_t pt1,pt2;
    pthread_create(&pt1);
    pthread_create(&pt2);

    /* code */
    return 0;
}
