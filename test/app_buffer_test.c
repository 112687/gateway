#include "app_buffer.h"
#include "log/log.h"

int main(int argc, char const *argv[])
{
    // 两条记录连同长度字段共占 (1 + 5) + (1 + 6) = 13 字节
    Buffer *buffer = add_Buffer_Init(13);

    // 写入数据1 2
    add_Buffer_Write(buffer, "abcde", 5);
    add_Buffer_Write(buffer, "fghijk", 6);

    // 读取数据1
    char read_buf[100];
    int data_len = add_Buffer_Read(buffer, read_buf, 100);
    log_debug("读数据1: %.*s, len: %d", data_len, read_buf, data_len);

    // 写入数据3
    add_Buffer_Write(buffer, "123456", 6);

    // 读取数据2
    char read_buf2[100];
    int data_len2 = add_Buffer_Read(buffer, read_buf2, 100);
    log_debug("读数据2: %.*s, len2: %d", data_len2, read_buf2, data_len2);

    // 读数据3
    char read_buf3[100];
    int data_len3 = add_Buffer_Read(buffer, read_buf3, 100);
    log_debug("读数据3: %.*s, len: %d", data_len3, read_buf3, data_len3);

    // 写数据 => 超出缓冲区大小， 失败
    add_Buffer_Write(buffer, "1234567890abcd", 14);
    return 0;
}
