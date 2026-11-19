#if !defined(_APP_POOL_H)
#define _APP_POOL_H
//任务结构体

typedef struct 
{
    int (*task_func)(void *arg); //任务函数指针
    void *arg;                     //任务函数参数
}Task;



/**
 * 初始化线程池
 * @param pool_size 线程池中线程的数量
 */
int app_pool_init(int pool_size);


/**
 * 销毁线程池
 */

 void app_pool_destroy();

 /**
  * 添加任务
  * @param task_func 任务函数指针
  * @param arg 任务函数参数
  */
 int app_pool_addTask( int(*task_func)(void *arg), void *arg);




#endif // _APP_POOL_H
