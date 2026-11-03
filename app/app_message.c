#include "json/cJSON.h"
#include "log/log.h"
#include <string.h>
#include <stdlib.h>
#include "app_common.h"
#include "app_message.h"

/**
 * chars : 1 2 4 XX abcd
 * json : {"conn_type": 1,"id": "5858","msg":"61626364" } 
 */
char *app_message_chars2Json(char *chars,int len){
    // 从chars中解析出conn_type,id_len,msg_len,id,msg
    int conn_type = chars[0];
    int id_len = chars[1];
    int msg_len = chars[2];

    // 检查chars的合法性len是否足够
    if(len < 3+id_len+msg_len){
        log_error("chars is not enough");
        return NULL;
    }

    char id[id_len];

    memcpy(id,chars+3,id_len);

    char msg[msg_len];

    memcpy(msg,chars+3+id_len,msg_len);


    // 生成16进制的id和msg的字符串  chars2Hexstr
    char *id_hex = app_common_chars2Hexstr(id,id_len);
    char *msg_hex = app_common_chars2Hexstr(msg,msg_len);



    // 生成json字符串
    cJSON *root_obj = cJSON_CreateObject();
    cJSON_AddNumberToObject(root_obj,"conn_type",conn_type);
    cJSON_AddStringToObject(root_obj,"id",id_hex);
    cJSON_AddStringToObject(root_obj,"msg",msg_hex);

    char *json_str = cJSON_PrintUnformatted(root_obj);


    
    // 释放内存
    cJSON_Delete(root_obj);
    free(id_hex);
    free(msg_hex);


    // 返回json
    return json_str;

}

 int app_message_json2Chars(char *json,char *chars_buf,int buf_size){
/**
 * json : {"conn_type": 1,"id": "5858","msg":"61626364" } 
 * chars : 1 2 4 XX abcd
 * 
 */

//  解析json,得到 conn_type,id_hex,msg_hex,
    cJSON *root_obj = cJSON_Parse(json);
    cJSON *conn_type_json = cJSON_GetObjectItem(root_obj, "conn_type");
    cJSON *id_hex_json = cJSON_GetObjectItem(root_obj, "id");
    cJSON *msg_hex_json= cJSON_GetObjectItem(root_obj, "msg");


// 将16进制的id_hex,msg_hex 转换成chars类型的id,msg
    char conn_type = conn_type_json->valueint;
    char *id_hex = id_hex_json->valuestring;
    char *msg_hex = msg_hex_json->valuestring;

    int id_len= -1;
    int msg_len= -1;
    char *id = app_common_hexstr2Chars(id_hex,&id_len);
    char *msg = app_common_hexstr2Chars(msg_hex, &msg_len);

// 检查buf_size 是否足够
if(buf_size < id_len + msg_len + 3)
{
    log_error("buf_size is not enough");
    return -1;
}
// 向chars_buf 中写入字符数组消息
chars_buf[0] = conn_type;
chars_buf[1] = id_len;
chars_buf[2] = msg_len;
memcpy(chars_buf+3,id,id_len);
memcpy(chars_buf+3+id_len,msg,msg_len);

// 释放内存
cJSON_Delete(root_obj);
free(id);
free(msg);

// 返回拼接的字符数组长度
return 3+id_len+msg_len;

 }