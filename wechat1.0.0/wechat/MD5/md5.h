#pragma once
#ifndef  _MD5_H_
#define  _MD5_H_
/* 对md5算法的改进：因为md5算法多用在用户密码的加密存储上，数据库中存放的是加密后的密文，当 */
/* 网站存在漏洞或者注入的时候仍然有可能使攻击者获得数据库的密文，在通过一些暴力破解工具获得用户密码造成用户信息泄漏，这里，提供 */
/* 改进方法---------对加密后的密文进行的改进： */
/* 1.利用一个截取函数截取密文start-1长度的左侧命名为A */
/* 2.利用截取函数截取密文start+number-1长度的的右侧命名为B */
/* 3.利用一个随机填充number位数的函数随机生成number个字符串命名为C */
/* 4.得到的密文就是A&C&B */

/* 用户登录的时候验证密码只要进行前两步，分别产生A1，B1, 如果A = A1, B = B1那麽验证成功，这样数据库里存放的并不是真正的密文， */
/* 即使被攻击者得到也不会轻易的获得用户的密码。 */
typedef  struct 
{
    unsigned int count[2];
    unsigned int stat[4];
    unsigned char buf[64];
}MD5_CTX;

#define F(x, y, z) ((x & y) | (~x & z))
#define G(x, y, z) ((x & z) | (~y & z))

#define H(x, y, z) (x^y^z)
#define I(x, y, z) (y ^ (x | ~z))
#define ROTATE_LEFT(x, n) ((x << n) | (x >> (32 - n)))

    /* 1.对a,b,c,d中的三个数进行非线性运算，然后加上第四个数，+文本中的一个子分组和一个常熟 */
    /* 2.再将结果右移一个随机的数 */
    /* 3.最后加上abcd中任意一个数 */
#define FF(a, b, c, d, x, s, ac)\
{\
    a += F(b, c, d) + x + ac;\
    a = ROTATE_LEFT(a, s);\
    a += b;\
}

#define GG(a, b, c, d, x, s, ac)\
{\
    a += G(b, c, d) + x + ac;\
    a = ROTATE_LEFT(a, s);\
    a += b;\
}

#define II(a, b, c, d, x, s, ac)\
{\
    a += I(b, c, d) + x + ac;\
    a = ROTATE_LEFT(a, s);\
    a += b;\
}

#define HH(a, b, c, d, x, s, ac)\
{\
    a += H(b, c, d) + x + ac;\
    a = ROTATE_LEFT(a, s);\
    a += b;\
}

void MD5Init(MD5_CTX *context);
void MD5Update(MD5_CTX *context, unsigned char *input, unsigned int inputlen);
void MD5Final(MD5_CTX *context, unsigned char digest[16]);
void MD5Transform(unsigned int state[4], unsigned char block[64]);
void MD5Encode(unsigned char *output, unsigned int *input, unsigned int len);
void MD5Decode(unsigned int *output, unsigned char *input, unsigned int len);

#endif
