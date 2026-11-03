#include "app_mqtt.h"
#include "log/log.h"
#include <MQTTClient.h>
#include <string.h>

static MQTTClient_message pubmsg = MQTTClient_message_initializer;

static MQTTClient client;
static MQTTClient_connectOptions conn_opts =MQTTClient_connectOptions_initializer;
static int (*recv_callback)(char *json) = NULL;

// 发送信息回调
static void delivered(void *context, MQTTClient_deliveryToken dt)
{
    log_debug("Message send successfully");
}
//  收到远程消息的回调
//返回1表示处理成功，返回0表示处理失败
static int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    int result = 0;
    if(recv_callback){
        result = recv_callback(message->payload)==0?1:0;
    }

    //释放资源
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return result;
}
//  链接断开的回调
static void connlost(void *context, char *cause)
{

    log_error("Connection lost cause: %s", cause);
}

/**
 * 初始化客户端
 */
int app_mqtt_init(){
    //初始化MQTT客户端
    int is_client =MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    if(is_client == MQTTCLIENT_SUCCESS){
        log_info("MQTTClient_create success");
    }else{
        log_error("MQTTClient_create error");
        return -1;
    }

    //设置回调函数
    int is_setCallbacks = MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);
    if(is_setCallbacks == MQTTCLIENT_SUCCESS){
        log_info("MQTTClient_setCallbacks success");
    }else{
        log_error("MQTTClient_setCallbacks error");
        MQTTClient_destroy(&client);
        return -1;
    }

    //连接到mqtt服务器
    int is_connect = MQTTClient_connect(client, &conn_opts);


    if(is_connect == MQTTCLIENT_SUCCESS){
        log_info("MQTTClient_connect success");
    }else{
        log_error("MQTTClient_connect error");
        MQTTClient_destroy(&client);
        return -1;
    }




    //订阅主题
    int is_subscribe = MQTTClient_subscribe(client, TOPIC_PULL, QOS);
    if(is_subscribe == MQTTCLIENT_SUCCESS){
        log_info("MQTTClient_subscribe success");
    }else{
        log_error("MQTTClient_subscribe error");
        MQTTClient_disconnect(client, TIMEOUT);
        MQTTClient_destroy(&client);
        return -1;
    }
    return 0;



}


/**
 * 关闭mqtt客户端
 */

 void app_mqtt_close(){
    MQTTClient_disconnect(client, TIMEOUT);
    MQTTClient_destroy(&client);
 }

 /**
  * 发送消息给远程服务器
  */
 int app_mqtt_send(char *json){
    pubmsg.payload = json;
    pubmsg.payloadlen = strlen(json)+1;
    pubmsg.qos = QOS;
    int is_message = MQTTClient_publishMessage(client, TOPIC_PUSH, &pubmsg, NULL);
    if(is_message == MQTTCLIENT_SUCCESS){
        log_debug("MQTTClient_publishMessage success");
    }else{
        log_error("MQTTClient_publishMessage error");
        return -1;
    }
    return 0;
 }


 /**
  * 注册接收处理远程消息的函数
  */
 void app_mqtt_registerRecvcallback(int (*callback)(char *json)){
    recv_callback = callback;

 }
