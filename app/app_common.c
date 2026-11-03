#include <stdio.h>
#include <sys/time.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

long app_common_getCurrentTime()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;


}
/**
 * [abcd]=> "61626364"
 */
char *app_common_chars2Hexstr(char *chars,int len){

    //申请16进制字符串空间
    char *hexstr = (char*)malloc(len*2+1);
    // 逐字节转换16进制
    for(int i=0;i<len;i++){
        sprintf(hexstr+i*2,"%02x",chars[i]);  // 


    }
    // 添加结束字符
    hexstr[len*2] = '\0';

    // 返回hexstr
    return hexstr;
}
/**
 * "61626364"=> [abcd]
 */

char *app_common_hexstr2Chars(char *hexstr,int *len){
    

    // 获取hexstr长度
    int hexstr_len = strlen(hexstr);
    // 获取字符串长度
    *len =strlen(hexstr)/2;

    //申请字符空间
    char *chars = (char*)malloc(*len);

    //每两个16进制字符换成1个字符
    for(int i=0;i<hexstr_len;i+=2){
        // 逐字节转换
        sscanf(hexstr+i,"%02x",(unsigned int *)(chars+i/2)); // 16进制转换成10进制
    }

    return chars;

}