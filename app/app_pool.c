#include "app_pool.h"
#include"log/log.h"
#include <pthread.h>
#include <stdlib.h>
#include <mqueue.h>


static int thread_num; 
static pthread_t *threads_pool; 
static mqd_t mq_fd;    // 队列描述符
static char *mq_name = "/app_pool_mq";    // /代表: /dev/mqueue,  my_pool_mq是任务队列的名字



//线程函数
static void *thread_func(void *arg) {
    Task task;
    while (1) {
        //从消息队列中取出任务
        int len = mq_receive(mq_fd, (char *)&task, sizeof(Task), NULL);

        //执行任务
        if (len==sizeof(Task)) {
            task.task_func(task.arg);
        } else {
            log_error("接收的错误任务长度: %d,期望长度: %lu", len, sizeof(Task));
        }
    }
    return NULL;
}


/**
 * 初始化线程池
 * @param pool_size 线程池中线程的数量
 */
int app_pool_init(int pool_size){

//初始化线程队列
    // 创建队列
    struct mq_attr attr; // 队列属性结构体
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(Task);  // 任务结构体的大小


    // O_CREAT:如果队列不存在,则创建队列
    // O_RDWR:读写模式打开队列
    // 0644:队列的权限,用户可读写,组用户可读,其他用户可读
    mq_fd = mq_open(mq_name, O_CREAT | O_RDWR, 0644, &attr);
    
    if (mq_fd == -1) {
        log_error("Failed to create message queue");
        free(threads_pool);
        return -1;
    }

    //初始化线程池
    thread_num = pool_size;
    threads_pool = (pthread_t *)malloc(sizeof(pthread_t) * pool_size);

    if (!threads_pool) {
        log_error("Failed to allocate memory for threads");
        return -1;
    }
    for (int i = 0; i < pool_size; i++) {
        if (pthread_create(&threads_pool[i], NULL, thread_func, NULL) != 0) {
            log_error("Failed to create thread %d", i);
            
            return -1;
        }
        log_debug("线程%d创建成功", gettid());
    }

    log_debug("线程池初始化成功");
    return 0;

    

}


/**
 * 销毁线程池
 */

 void app_pool_destroy(){
    //关闭并删除队列
    mq_close(mq_fd);
    mq_unlink(mq_name);

    // 取消线程并等待它们结束
    for (int i = 0; i < thread_num; i++)
    {
        pthread_cancel(threads_pool[i]);
        pthread_join(threads_pool[i], NULL);
    }
    // 释放线程池内存
    free(threads_pool);
    log_debug("线程池销毁成功");
    


 }

 /**
  * 添加任务
  * @param task_func 任务函数指针
  * @param arg 任务函数参数
  */
 int  app_pool_addTask( int(*task_func)(void *arg), void *arg)
    {
        // 创建任务结构体
        Task task;
        task.task_func = task_func;
        task.arg = arg;
    
        // 将任务发送到任务队列
        return mq_send(mq_fd, (const char *)&task, sizeof(Task), 0) ;



    }
