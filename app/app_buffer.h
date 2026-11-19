#if !defined(__APP_BUFFER_H)
#define __APP_BUFFER_H

#include <pthread.h>

//内部子缓冲区
typedef struct 
{
    unsigned char* pBuffer; //缓冲区指针
    int total_size;     //缓冲区大小
    int len;     //已使用大小
} SubBuffer;



//双缓冲区
typedef struct 
{
    SubBuffer* Sub_Buffer[2]; //双缓冲区指针 两个子缓冲区
    int read_index; //当前读取的缓冲区索引
    int write_index; //当前写入的缓冲区索引
    pthread_mutex_t read_lock; //读锁
    pthread_mutex_t write_lock; //写锁
    

} Buffer;


/**'
 * 初始化缓冲区
 * @param size 缓冲区大小
 * 
 * 
 */
Buffer* add_Buffer_Init(int size);

 /**
  * 销毁缓冲区
  * @param buffer 缓冲区指针
  * 
  */
 void Buffer_Destroy(Buffer* buffer);


 /**
  * 写入数据
  * @param buffer 缓冲区指针
  * @param data 要写入的数据
  * @param data_len 数据长度
  * 
  * 
  */
 int add_Buffer_Write(Buffer* buffer, char* data, int data_len);


 /**
  * 读取数据
  * @param buffer 缓冲区指针
  * @param data 读取到的数据
  * @param buffer_size 数据长度
  * @return 读取到的数据长度
  *
  * 
  */
int add_Buffer_Read(Buffer* buffer, char* data, int buffer_size);








#endif // __APP_BUFFER_H
