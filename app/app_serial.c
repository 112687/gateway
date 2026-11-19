#define _GNU_SOURCE
#include "app_serial.h"
#include "log/log.h"

/**
 * 初始化
 */
int app_serial_init(Device *device){
    // 初始化串口设备
    app_serial_setBraudRate(device, BR_9600);
    app_serial_setParity(device, Parity_NONE);
    app_serial_setStopBits(device, SB_1);
    // app_serial_setBlock(device,0);
    app_serial_setRaw(device);


    //刷新生效
    int result = tcflush(device->fd, TCIFLUSH);
    if(result < 0){
        log_error("串口初始化失败");
        return -1;
    }
    
    log_info("串口初始化成功");
    return 0;
}

/**
 * 设置波特率
 */
int app_serial_setBraudRate(Device *device, BraudRate rate){

    struct termios attr;
    //修改串口属性
    tcgetattr(device->fd, &attr);

    //修改波特率
    cfsetispeed(&attr, rate);

    //设置串口属性(当前不生效,都设置好的flush时才生效)
    int result = tcsetattr(device->fd, TCSAFLUSH, &attr);
    if (result==-1)
    {
        log_error("设置波特率失败");
        return -1;

    }
    log_info("设置波特率成功");
    return 0;

}

/**
 * 设置校验位
 */
int app_serial_setParity(Device *device, Parity parity){
    struct termios attr;
    //修改串口属性
    tcgetattr(device->fd, &attr);

    //修改校验位
    attr.c_cflag &= ~(PARENB|PARODD); 
    attr.c_cflag |= parity;


    

    //设置串口属性(当前不生效,都设置好的flush时才生效)
    int result = tcsetattr(device->fd, TCSAFLUSH, &attr);
    if (result==-1)
    {
        log_error("设置校验位失败");
        return -1;

    }
    log_info("设置校验位成功");
    return 0;
}

/**
 * 设置停止位
 */
int app_serial_setStopBits(Device *device, StopBits stopBits){
    struct termios attr;
    //修改串口属性
    tcgetattr(device->fd, &attr);

    //修改停止位
    attr.c_cflag &= ~CSTOPB;
    attr.c_cflag |= stopBits;


    //设置串口属性(当前不生效,都设置好的flush时才生效)
    int result = tcsetattr(device->fd, TCSAFLUSH, &attr);
    if (result==-1)
    {
        log_error("设置停止位失败");
        return -1;

    }
    log_info("设置停止位成功");
    return 0;
}

/**
 * 设置是否阻塞模式
 */
int app_serial_setBlock(Device *device, int is_block){
    
struct termios attr;
    //修改串口属性
    tcgetattr(device->fd, &attr);

    //修改阻塞模式
    if (is_block)
    {
        /* 阻塞模式 */
        attr.c_cc[VTIME] = 0;
        attr.c_cc[VMIN] = 1;
    }
    else
    {
        /* 非阻塞模式 */
        attr.c_cc[VTIME] = 2;
        attr.c_cc[VMIN] = 0;
    }
    

    //设置串口属性(当前不生效,都设置好的flush时才生效)
    int result = tcsetattr(device->fd, TCSAFLUSH, &attr);
    if (result==-1)
    {
        log_error("设置阻塞模式失败");
        return -1;

    }
    log_info("设置阻塞模式成功");
    return 0;
}
int app_serial_setRaw(Device *device){
    struct termios attr;
    //修改串口属性
    tcgetattr(device->fd, &attr);

    //修改原始模式
    cfmakeraw(&attr);

    

    //设置串口属性(当前不生效,都设置好的flush时才生效)
    int result = tcsetattr(device->fd, TCSAFLUSH, &attr);
    if (result==-1)
    {
        log_error("设置原始模式失败");
        return -1;

    }
    log_info("设置原始模式成功");
    return 0;
}


/**
 * 刷新让设置属性生效
 */
int app_serial_flush(Device *device){
    return tcflush(device->fd, TCIOFLUSH);
}