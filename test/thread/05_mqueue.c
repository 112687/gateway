#define _GNU_SOURCE
#include"mqueue.h"
#include<pthread.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>


static char *mq_name = "/my_mq";    // /代表: /dev/mqueue,  my_mq是队列的名字,可以自定义

static mqd_t mq_fd;    // 队列描述符


//first线程的执行函数,发送消息
void *first_thread_func(void *arg)
{
    printf("first线程(%d)发送信息...\n", gettid());

    mq_send(mq_fd, "hello world", strlen("hello world")+1, 0);
    return NULL;
    
}
//second线程的执行函数,接收消息
void *second_thread_func(void *arg)
{
    sleep(1);  // 确保first线程先执行
    printf("second线程(%d)接收信息...\n", gettid());


    char buffer[1024];
    ssize_t bytes_received = mq_receive(mq_fd, buffer, sizeof(buffer), NULL);
    if (bytes_received == -1)
    {
        printf("mq_receive failed\n");
        return NULL;
    }
    printf("接收到的消息: %s,长度是%ld\n", buffer, bytes_received   );
    return NULL;
    
}
int main(int argc, char const *argv[])
{

    // 创建队列
    struct mq_attr attr; // 队列属性结构体
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 1024;


    // O_CREAT:如果队列不存在,则创建队列
    // O_RDWR:读写模式打开队列
    // 0644:队列的权限,用户可读写,组用户可读,其他用户可读
    mq_fd = mq_open(mq_name, O_CREAT | O_RDWR, 0644, &attr);
    if (mq_fd == -1)
    {
        printf("创建队列 failed\n");
        return -1;
    }


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

    // 关闭并删除队列
    mq_close(mq_fd);
    mq_unlink(mq_name);

    /* code */
    return 0;
}
