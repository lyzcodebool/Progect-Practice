/*************************************************************************
	> File Name: mysql.h
	> Author: 
	> Mail: 
	> Created Time: Wed 21 Feb 2018 04:16:47 AM PST
 ************************************************************************/

#ifndef _MYSQL_H
#define _MYSQL_H
#include<mysql.h>
MYSQL           mysql;
MYSQL_RES       *res = NULL;
MYSQL_ROW       row;

/*连接数据库*/
int mysql_connect();
int mysql_data_add();
int mysql_data_dete();
int mysql_data_update();
int mysql_result();
#endif
