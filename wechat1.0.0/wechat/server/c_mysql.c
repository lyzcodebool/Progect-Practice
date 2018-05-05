/*************************************************************************
	> File Name: mysql.h
	> Author: 
	> Mail: 
	> Created Time: Wed 21 Feb 2018 04:16:47 AM PST
 ************************************************************************/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"c_mysql.h"

/*连接数据库*/
int mysql_connect(char *db_name, char *db_passwd)
{
    if(NULL == mysql_init(&mysql)){
        printf("mysql_init() : %s\n", mysql_error(&mysql));
        exit(1);
    }
    if(NULL == mysql_real_connect(&mysql, "localhost", "root", db_name, db_passwd, 0, NULL, 0)){
        printf("mysql_real_connect(): %s\n", mysql_errorr(&mysql));
        exit(1);
    }
    printf("connect Mysql database success!\n");
    mysql_close(&mysql);
    return 0;
}
int mysql_data_add()
{
    
}
/*删除数据库的数据*/
int mysql_data_dete(char *query_str)
{
    rc = mysql_real_query(&mysql, query_str, strlen(query_str));
    if(rc != 0){
        printf("mysql_real_query(): %s\n", mysql_errorr());
        exit(1);
    }
    return 0;
}
/**/
int mysql_data_update();
{

}
/*查询数据库数据*/
int mysql_query()
{
    char            *query_str = NULL;
    int             rc, i, fields;
    int             rows;
    rc = mysql_real_query(&mysql, query_str, strlen(query_str));
    if(rc != 0){
        printf("mysql_real_query(): %s\n", mysql_error(&mysql));
        exit(1);
    }
    res = mysql_store_result(&mysql);
    if(res == NULL){
        printf("mysql_store_result(): %s\n", mysql_error(&mysql));
        exit(1);
    }
    rows = mysql_num_rows(res);
    printf("The total rows is %d\n", rows);
    fields = mysql_num_fields(res);
    printf("The total fields is %d\n", fields);
    while((row = mysql_fetch_row(res))){
        for(i = 0; i < fields; ++i){
            printf("%s\t", row[i]);
        }
        putchar(10);
    }
    return 0;
}
