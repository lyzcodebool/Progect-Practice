#include"tunable.h"


//是否开启被动模式
int tunable_pasv_enable = 1;
//是否开启主动模式
int tunable_port_enable = 1;
//FTP服务器端口
unsigned int tunable_listen_port = 21;
//最大客户连接数
unsigned int tunable_max_clients = 2000;
//每ip最大连接数
unsigned int tunable_max_per_ip = 50;
unsigned int tunable_accept_timeout = 60;
unsigned int tunable_connect_timeout = 60;
//控制连接超时时间
unsigned int tunable_idle_session_timeout = 300;
//数据连接超时时间
unsigned int tunable_data_connection_timeout = 300;
//掩码
unsigned int tunable_local_umask = 077;
//最大上传速度
unsigned int tunable_upload_max_rate = 0;
//最大下载速度
unsigned int tunable_download_max_rate = 0;
//FTP服务器ip地址
const char *tunable_listen_address;
