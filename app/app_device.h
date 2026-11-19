#if !defined(__APP_DEVICE_H)
#define __APP_DEVICE_H


#include <pthread.h>
#include "app_buffer.h"


//设备的结构题
typedef struct{
    char *filename;  //串口文件名
    int  fd; //串口文件描述符
    Buffer *upbuffer;  //上行缓冲区
    Buffer *downbuffer;  //下行缓冲区
    int is_running;
    int (*post_read)(char *data, int len); //读取到串口数据后的后置处理(蓝牙数据->message)
    int (*pre_write)(char *data, int len);  //发送到串口数据后的前置处理(message->蓝牙数据)
    long last_write_time;
}Device;


/**
 * 初始化
 */
Device *app_device_init(char *filename);



/**
 * 启动
 */
int app_device_start();

 /**
  * 停止
  */
 int app_device_close();


#endif // __APP_DEVICE_H
