#if !defined(__APP_COMMON_H)

#define __APP_COMMON_H

/**
@brief 获取当前时间戳
@return 当前时间戳
*/  
long app_common_getCurrentTime();


/**
 *@brief 字符数组转成16进制字符串
 *@param chars 源字符数组
 *@param len 源字符数组长度 
    *@return 16进制字符串
 * 
 * 
 */

char *app_common_chars2Hexstr(char *chars,int len);  


/**
 * @brief 16进制字符串转成字符数组
 * @param hexstr 16进制字符串
 * @param len 16进制字符串长度长度的指针
 * @return 转换后的字符数组
 */
char *app_common_hexstr2Chars(char *hexstr,int *len);  

#endif // __APP_COMMON_H
