#include "app_mqtt.h"
#include "log/log.h"
#include <unistd.h>

int app_mqtt_recv(char *json){
    log_debug("收到的远程消息数据: %s", json);
    return 0;
}
int main(int argc, char const *argv[])
{

    // 注册接收的回调函数
    app_mqtt_registerRecvcallback(app_mqtt_recv);
    // 初始化
    if (app_mqtt_init() != 0) {
        log_error("MQTT初始化失败，退出测试");
        return 1;
    }
    // 发送数据
    if (app_mqtt_send("{\"conn_type\":1,\"id\":\"5858\",\"msg\":\"61626364\"}") != 0) {
        app_mqtt_close();
        return 1;
    }
    
    // 休眠50秒，让当前运行的程序不立即结束 =》 为了能接受消息
    sleep(50);

    // 关闭
    app_mqtt_close();
    return 0;
}
