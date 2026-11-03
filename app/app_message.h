#if !defined(__APP_MESSAGE_H)

#define __APP_MESSAGE_H

/*
@brief 字符数组转换成json消息 
@param chars 字符数组
@param len 字符数组长度
@return json字符串
 */

 char *app_message_chars2Json(char *chars,int len);

 

 /**
  * @brief json消息转换成字符数组
  * @param json json字符串
  * @param chars_buf 字符数组消息的缓存容器
  * @param buf_size 缓存容器大小
  * @return 实际写入的字符数组长度 ,-1 转换失败
  * 
  * 
  *  
  * */ 
 int app_message_json2Chars(char *json,char *chars_buf,int buf_size);


#endif // __APP_MESSAGE_H
