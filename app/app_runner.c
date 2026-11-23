#include "app_device.h"
#include "unistd.h"
#include "stdlib.h"
#include "app_bt.h"
#include "signal.h"
#include "app_runner.h"


int is_running = 1;
void runner_exit(int sign){
    is_running=0;
    // app_device_close();
}
int app_runner_run(){
    // 注册信号处理函数前实现结束前释放资源的操作
    signal(SIGINT,runner_exit);
    signal(SIGALRM,runner_exit);


    // 初始设备
    Device *device = app_device_init(DEVICE_FILE);

    // 初始化蓝牙
    app_bt_init(device);

    // 启动设备
    app_device_start(device);



    // 不断的运行
    while(is_running==1)
    {
        sleep(1);

    }
    // 释放资源
    app_device_close();
    return 0;

    

}