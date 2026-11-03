#if !defined(__APP_MQTT_H)
#define __APP_MQTT_H


#define ADDRESS     "tcp://127.0.0.1:1883"
#define CLIENTID    "b253ba38-daf6-4b37-984f-5d8fdc6a1cfa" 
#define TOPIC_PULL       "REMOTE_TO_GATEWAY"
#define TOPIC_PUSH       "GATEWAY_TO_REMOTE"
#define PAYLOAD     "Hello World!"
#define QOS         1
#define TIMEOUT     10000L
/**
 * 初始化客户端
 */
int app_mqtt_init();


/**
 * 关闭mqtt客户端
 */

 void app_mqtt_close();

 /**
  * 发送消息给远程服务器
  */
 int app_mqtt_send(char *json);


 /**
  * 注册接收处理远程消息的函数
  */
 void app_mqtt_registerRecvcallback(int (*callback)(char *json));





#endif // __APP_MQTT_H
