#include "app_buffer.h"
#include "log/log.h"
#include <stdlib.h>
#include <string.h>

static SubBuffer *init_SubBuffer(int size)
{

    // 初始化子缓冲区
    // 申请subBuffer结构体内存
    SubBuffer *subBuffer = (SubBuffer *)malloc(sizeof(SubBuffer));

    if (subBuffer == NULL)
    {
        log_error("Failed to allocate memory for subBuffer");
        return NULL;
    }

    // 申请缓冲区内存
    subBuffer->pBuffer = (char *)malloc(size);
    subBuffer->total_size = size;
    subBuffer->len = 0;

    if (subBuffer->pBuffer == NULL)
    {
        log_error("Failed to allocate memory for subBuffer->pBuffer");
        free(subBuffer);
        return NULL;
    }

    return subBuffer;
}
/**'
 * 初始化缓冲区
 * @param size 缓冲区大小
 *
 *
 */
Buffer *add_Buffer_Init(int size)
{
    // 申请buffer结构体内存
    Buffer *buffer = (Buffer *)malloc(sizeof(Buffer));
    if (buffer == NULL)
    {
        log_error("Failed to allocate memory for buffer");
        return NULL;
    }
    // 初始化buffer
    buffer->Sub_Buffer[0] = init_SubBuffer(size);
    buffer->Sub_Buffer[1] = init_SubBuffer(size);

    if (buffer->Sub_Buffer[0] == NULL || buffer->Sub_Buffer[1] == NULL)
    {
        log_error("Failed to allocate memory for sub buffers");
        free(buffer);
        return NULL;
    }
    buffer->read_index = 0;
    buffer->write_index = 1;
    // 初始化 读 写锁
    pthread_mutex_init(&buffer->read_lock, NULL);
    pthread_mutex_init(&buffer->write_lock, NULL);

    return buffer;
}

/**
 * 销毁缓冲区
 * @param buffer 缓冲区指针
 *
 */
void Buffer_Destroy(Buffer *buffer)
{
    // 释放子缓冲区内存
    free(buffer->Sub_Buffer[0]->pBuffer);
    free(buffer->Sub_Buffer[1]->pBuffer);
    free(buffer->Sub_Buffer[0]);
    free(buffer->Sub_Buffer[1]);
    free(buffer);
}

/**
 * 写入数据
 * @param buffer 缓冲区指针
 * @param data 要写入的数据
 * @param data_len 数据长度
 *
 *
 */
int add_Buffer_Write(Buffer *buffer, char *data, int data_len)
{

    if (data_len > 255)
    {
        printf("数据长度不能大于255\n");
        return -1;
    }
    // 获取写锁
    log_debug("获取写锁");
    pthread_mutex_lock(&buffer->write_lock);

    // 获取写缓冲区
    SubBuffer *write_buffer = buffer->Sub_Buffer[buffer->write_index];

    // 检查写缓冲区是否有足够的空间
    if (write_buffer->len + data_len + 1 > write_buffer->total_size)
    { // 加1是因为要写入长度信息
        log_error("写缓冲区空间不足");
        pthread_mutex_unlock(&buffer->write_lock);
        return -1;
    }

    // 写入数据: 先写长度,再写数据
    write_buffer->pBuffer[write_buffer->len] = data_len;

    memcpy(write_buffer->pBuffer + write_buffer->len + 1, data, data_len);

    // 更新写缓冲区的长度len
    write_buffer->len += data_len + 1; // 加1是因为写入了长度信息

    // 释放写锁
    log_debug("释放写锁");
    pthread_mutex_unlock(&buffer->write_lock);

    return 0;
}

// 交换缓冲区索引
static void swap_buffers(Buffer *buffer)
{
    //正在写入的时候不能切换读写缓冲区
    log_debug("准备切换缓冲区索引前添加写锁");
    pthread_mutex_lock(&buffer->write_lock);

    // 交换读写缓冲区索引
    int temp = buffer->read_index;
    buffer->read_index = buffer->write_index;
    buffer->write_index = temp;

    // 重置新的写缓冲区的长度len为0

    buffer->Sub_Buffer[buffer->write_index]->len = 0;

    // 释放写锁
    log_debug("切换缓冲区索引后释放写锁");
    pthread_mutex_unlock(&buffer->write_lock);
}

/**
 * 读取数据
 * @param buffer 缓冲区指针
 * @param data 读取到的数据
 * @param buffer_size 数据长度
 * @return 读取到的数据长度
 *
 *
 */
int add_Buffer_Read(Buffer *buffer, char *data, int buffer_size)
{
    // 获取读锁
    log_debug("获取读锁");
    pthread_mutex_lock(&buffer->read_lock);
    // 得到读缓冲区
    SubBuffer *read_buffer = buffer->Sub_Buffer[buffer->read_index];

    // 检查读缓冲区是否有数据,没有数据就交换缓冲区
    if (read_buffer->len == 0)
    {
        swap_buffers(buffer);
        read_buffer = buffer->Sub_Buffer[buffer->read_index];
        if (read_buffer->len == 0)
        {
            log_debug("no data in read buffer");
            // 释放读锁
            pthread_mutex_unlock(&buffer->read_lock);
            
            return -1;
        }
    }

    // 读取数据: 先读长度,再读数据
    int data_len = read_buffer->pBuffer[0]; // 读取长度信息

    if (buffer_size < data_len)
    {
        log_error("data空间不足");
        // 释放读锁
        pthread_mutex_unlock(&buffer->read_lock);
        return -1;
    }

    memcpy(data, read_buffer->pBuffer + 1, data_len);

    // 讲未读取的移动到最左边
    memmove(read_buffer->pBuffer, read_buffer->pBuffer + data_len + 1, read_buffer->len - data_len - 1);

    // 更新读缓冲区的长度len
    read_buffer->len -= data_len + 1;

    // 释放读锁
    log_debug("释放读锁");
    pthread_mutex_unlock(&buffer->read_lock);

    return data_len;
}