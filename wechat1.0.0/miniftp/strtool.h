#ifndef _STR_TOOL_H_
#define _STR_TOOL_H_

//去除\r\n
void str_trim_crlf(char *str);
//字符串分割
void str_split(const char *str, char *left, char *right, char c);
//判断是否全是空白字符
int str_all_space(const char *str);
//字符串转化为大写格式
void str_upper(char *str);
//将字符串转化为long long 
long long str_to_longlong(const char *str);
//将字符串（八进制）转化为无符号整型
unsigned int str_octal_to_uint(const char *str);

#endif
