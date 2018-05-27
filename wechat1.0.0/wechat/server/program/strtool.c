#include"../bin/strtool.h"
#include"../bin/common.h"

//去除\r\n
void str_trim_crlf(char *str)
{
    char *p = &str[strlen(str)-1];
    while(*p == '\r' || *p == '\n'){
        *p-- = '\0';
    }
}
//字符串分割
void str_split(const char *str, char *left, char *right, char c)
{
    char *p = strchr(str, c);
    if(p == NULL)
        strcpy(left, str);
    else
    {
        strncpy(left, str, p-str);
        strcpy(right, p+1);
    }
}
//判断是否全是空白字符
int str_all_space(const char *str)
{
    while(*str)
    {
        if(!isspace(*str))
            return 0;
        str++;
    }
    return 1;
}
//字符串转化为大写格式
void str_upper(char *str)
{
    while(*str)
    {
        *str = toupper(*str);
        str ++;
    }
}
//将字符串转化为long long 
long long str_to_longlong(const char *str)
{
    //return atoll(str); 

    long long result;
    long long mult = 1;
    unsigned int len = strlen(str);
    unsigned int i;
    if(len > 15)
        return 0;

    for(i = 0; i < len; ++i)
    {
        char ch = str[len-(i+1)];
        long long val;
        if(ch < '0' || ch > '9')
            return 0;
        val = ch - '0';
        val *= mult;
        result += val;
        mult *= 10;
    }
    /*
    long long result;
    long long mult = 1;
    unsigned int len = strlen(str);
    int i;//这里不能用unsigned int 因为for循环i--会减到-1 超出范围

    if(len > 15)
        return 0;

    for(i = len - 1; i >= 0; i--)
    {
        char ch = str[i];
        long long val;
        if(ch < '0' || ch > '9')
            return 0;
        val = ch - '0';
        val *= mult;
        result += val;
        mult *= 10;
    }
    */
    return result;
}
//将字符串（八进制）转化为无符号整型
unsigned int str_octal_to_uint(const char *str)
{
    unsigned int result = 0;
    int seen_non_zero_digit = 0;

    while(*str)
    {
        int digit = *str;
        if(!isdigit(digit) || digit >'7')
            break;
        if(digit != '0')
            seen_non_zero_digit = 1;
        if(seen_non_zero_digit)
        {
            result <<= 3;
            result += (digit - '0');
        }
        str ++;
    }
    return result;
}
