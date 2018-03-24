#include"ftpproto.h"
#include"sysutil.h"
#include"strtool.h"
#include"tunable.h"

typedef struct ftpcmd
{
    const char *cmd;
    void (*cmd_handler)(session_t *sess);
}ftpcmd_t;

void ftp_replay(session_t *sess, int status, const char *text);
void ftp_lreplay(session_t *sess, int status, const char *text);
int list_common(session_t *sess);
int get_transfer_fd(session_t *sess);

static void do_user(session_t *sess);
static void do_pass(session_t *sess);
static void do_cmd(session_t *sess);
static void do_cdup(session_t *sess);
static void do_quit(session_t *sess);
static void do_port(session_t *sess);
static void do_pasv(session_t *sess);
static void do_type(session_t *sess);
static void do_stru(session_t *sess);
static void do_mode(session_t *sess);
static void do_retr(session_t *sess);
static void do_stor(session_t *sess);
static void do_appe(session_t *sess);
static void do_list(session_t *sess);
static void do_nlst(session_t *sess);
static void do_rest(session_t *sess);
static void do_abor(session_t *sess);
static void do_pwd(session_t *sess);
static void do_xpwd(session_t *sess);
static void do_mkd(session_t *sess);
static void do_xmkd(session_t *sess);
static void do_rmd(session_t *sess);
static void do_xrmd(session_t *sess);
static void do_dele(session_t *sess);
static void do_rnfr(session_t *sess);
static void do_rnto(session_t *sess);
static void do_site(session_t *sess);
static void do_syst(session_t *sess);
static void do_feat(session_t *sess);
static void do_size(session_t *sess);
static void do_stat(session_t *sess);
static void do_noop(session_t *sess);
static void do_help(session_t *sess);
static void do_allo(session_t *sess);
/*通过建立映射表防止程序中过多的调用if else */
ftpcmd_t ctrl_cmds[]= 
{
    /*访问控制命令*/
    {"USER",    do_user},
    {"PASS",    do_pass},
    {"CWD",    do_cmd},
    {"XCWD",    NULL},
    {"CDUP",    do_cdup},
    {"XCUP",    NULL},
    {"QUIT",    do_quit},
    {"ACCT",    NULL},
    {"SMNT",    NULL},
    {"REIN",    NULL},
    /*传递参数命令*/
    {"PORT",    do_port},
    {"PASV",    do_pasv},
    {"TYPE",    do_type},
    {"STRU",    do_stru},
    {"MODE",    do_mode},
    /*服务命令*/
    {"RETR",    do_retr},
    {"STOR",    do_stor},
    {"APPE",    do_appe},
    {"LIST",    do_list},
    {"NLST",    do_nlst},
    {"REST",    do_rest},
    {"ABOR",    do_abor},
    {"\377\364\377\362ABOR",    do_abor},
    {"PWD",    do_pwd},
    {"XPWD",    do_xpwd},
    {"MKD",    do_mkd},
    {"XMKD",    do_xmkd},
    {"RMD",    do_rmd},
    {"XRMD",    do_xrmd},
    {"DELE",    do_dele},
    {"RNFR",    do_rnfr},
    {"RNTO",    do_rnto},
    {"SITE",    do_site},
    {"SYST",    do_syst},
    {"FEAT",    do_feat},
    {"SIZE",    do_size},
    {"STAT",    do_stat},
    {"NOOP",    do_noop},
    {"HELP",    do_help},
    {"ALLO",    do_allo},
    {"CLNT",    NULL},
    {NULL, NULL}
};

void handle_child(session_t *sess)
{
    ftp_replay(sess, 220, "(miniftp 0.1)");
    int ret;
   while(1)
   {
        memset(sess->cmdline, 0, sizeof(sess->cmdline));
        memset(sess->cmd, 0, sizeof(sess->cmd));
        memset(sess->arg, 0, sizeof(sess->arg));
        ret = readline(sess->ctrl_fd, sess->cmdline, MAX_COMMAND_LINE);
        if(ret == -1)
            ERR_EXIT("readline");
        else if(ret == 0)
            exit(EXIT_FAILURE);

        //去除\r\n
        str_trim_crlf(sess->cmdline);
        printf("cmdline = [%s]\n", sess->cmdline);
        //得到一行命令
        //解析FTP命令and参数
        str_split(sess->cmdline, sess->cmd, sess->arg, ' ');
        printf("cmd = [%s] arg = [%s]\n", sess->cmd, sess->arg);

        //将命令转化为大写
        str_upper(sess->cmd);
        //处理FTP各种命令
       /* if(strcmp("USER", sess->cmd) == 0)
        {
            do_user(sess);
        }
        else if(strcmp("PASS", sess->cmd) == 0)
        {
            do_pass(sess);
        }*/
        int i;
        //得到数组长度
        int size = sizeof(ctrl_cmds)/sizeof(ctrl_cmds[0]);
        for(i = 0; i < size; ++i)
        {
            if(strcmp(ctrl_cmds[i].cmd, sess->cmd) == 0){
                if(ctrl_cmds[i].cmd_handler != NULL){
                    ctrl_cmds[i].cmd_handler(sess);
                }else{
                ftp_replay(sess, FTP_COMMANDNOTIMPL, "unimplement command");
                }
                break;
            }    
            if(i == size){
                ftp_replay(sess, FTP_BADCMD, "unknow command");
            }  
        }
   }
}
void ftp_replay(session_t *sess, int status, const char *text)
{
    char buf[1024] = {0};
    sprintf(buf, "%d %s\r\n", status, text);
    writen(sess->ctrl_fd, buf, strlen(buf));
}

void ftp_lreplay(session_t *sess, int status, const char *text)
{
    char buf[1024] = {0};
    sprintf(buf, "%d-%s\r\n", status, text);
    writen(sess->ctrl_fd, buf, strlen(buf));
}

int list_common(session_t *sess)
{
    DIR *dir = opendir(".");
    if(dir == NULL){
        return 0;
    }
    struct dirent *dt;
    struct stat sbuf;
    while((dt = readdir(dir)) != NULL){
        if(lstat(dt->d_name, &sbuf) < 0){
            continue;
        }
        if(dt->d_name[0] == '.')
            continue;
        char perms[] = "-------------";
        perms[0] = '?';
        //获取文件类型
        mode_t mode = sbuf.st_mode;
        switch(mode & S_IFMT)
        {
            case S_IFREG:
                perms[0] = '-';
                break;
            case S_IFDIR:
                //用户文件
                perms[0] = 'd';
                break;
            case S_IFLNK:
                //符号连接文件
                perms[0] = 'l';
                break;
            case S_IFIFO:
                //管道文件
                perms[0] = 'p';
                break;
            case S_IFSOCK:
                //套接字文件
                perms[0] = 's';
                break;
            case S_IFCHR:
                //字符设备文件
                perms[0] = 'c';
                break;
            case S_IFBLK:
                //块设备文件
                perms[0] = 'b';
                break;
        }
        //用户自身的权限
        if(mode & S_IRUSR){
            perms[1] = 'r';
        }
        if(mode & S_IWUSR){
            perms[2] = 'w';
        }
        if(mode & S_IXUSR){
            perms[3] = 'x';
        }
        //组用户的权限
        if(mode & S_IRGRP){
            perms[4] = 'r';
        }
        if(mode & S_IWGRP){
            perms[5] = 'r';
        }
        if(mode & S_IXGRP){
            perms[6] = 'x';
        }
        //其他用户的权限
        if(mode & S_IROTH){
            perms[7] = 'r';
        }
        if(mode & S_IWOTH){
            perms[8] = 'r';
        }
        if(mode & S_IXOTH){
            perms[9] = 'x';
        }
        //处理特殊权限位
        if(mode & S_ISUID){
            perms[3] = (perms[3] == 'x') ? 's':'S';
        }
        if(mode & S_ISGID){
            perms[6] = (perms[6] == 'x') ? 's':'S';
        }
        if(mode & S_ISVTX){
            perms[9] = (perms[9] == 'x') ? 't':'T';
        }

        //获取连接数
        char buf[1024] = {0};
        int off = 0;
        off += sprintf(buf, "%s", perms);
        off += sprintf(buf+off, "%3d  %-8d %-8d", (int)sbuf.st_nlink, sbuf.st_uid, sbuf.st_gid);
        //输出文件大小
        off += sprintf(buf+off, "%8lu", (unsigned long)sbuf.st_size);

        //输出文件的创建时间，两种格式
        const char *p_date_format = "%b %e %H:%M";
        struct timeval ty;
        gettimeofday(&ty, NULL);
        long local_time = ty.tv_sec;
        if(sbuf.st_mtime > local_time || (local_time - sbuf.st_mtime) > 60*60*24*182){
            //man strftime
            //超过半年时间的输出格式
            p_date_format = "%b %e %Y";
        }

        char datebuf[64] = {0};
        struct tm *p_tm = localtime(&local_time);
        //这里要用localtime将秒转化成struct tm结构体
        strftime(datebuf, sizeof(datebuf), p_date_format, p_tm);
        off += sprintf(buf+off, "%s", datebuf);

        //必须考虑到符号连接文件的输出格式
        //readlink
        if(S_ISLNK(sbuf.st_mode)){
            char temp[1024] = {0};
            readlink(dt->d_name, temp, sizeof(temp));
            off += sprintf(buf+off, "  %s -> %s\r\n", dt->d_name, temp);
        }else{
        sprintf(buf+off, "  %s\r\n", dt->d_name);
        }      
        writen(sess->data_fd, buf, strlen(buf));
    }
    closedir(dir);
    return 0;
}

int port_active(session_t *sess)
{
    if(sess->port_addr){
        if(pasv_active(sess)){
            fprintf(stderr, "botn port an pasv are active!\n");
            exit(EXIT_FAILURE);
        }
        return 1;
    }
    return 0;
}

int pasv_active(session_t *sess)
{
    return 0;
}

int get_transfer_fd(session_t *sess)
{
    //检测是否收到post或者posv命令
    if(!port_active(sess) && !posv_active(sess)){
        ftp_replay(sess, "Use port or pasv first.");
        return 0;
    }

    if(port_active(sess)){
        //这里普通用户没办法绑定20端口
        int fd = tcp_client(0);
        if(connect_timeout(fd, sess->port_addr, tunable_connect_timeout) < 0){
            close(fd);
            return 0;
        }
        sess->data_fd = fd;
    }
    if(sess->port_addr){
        free(sess->port_addr);
        sess->port_addr = NULL;
    }
    return 1;
}

static void do_user(session_t *sess)
{
    struct passwd *pw = getpwnam(sess->arg);
    if(pw == NULL){
        //用户不存在
        ftp_replay(sess, FTP_UNLOGIN, "Login incorrect");
        return;
    }
    sess->uid = pw->pw_uid;
    ftp_replay(sess, FTP_PASS, "Please specify the password");
}
static void do_pass(session_t *sess)
{
    //pass 123456
    struct passwd *pw = getpwuid(sess->uid);
    if(pw == NULL){
        //用户不存在
        ftp_replay(sess, FTP_UNLOGIN, "Login incorrect");
        return;
    }
    struct spwd *sp = getspnam(pw->pw_name);
    if(sp == NULL){
        ftp_replay(sess, FTP_UNLOGIN, "Login incorrect");
        return; 
    }
    //将明文进行加密,得到一个加密的密码
    char *encrypted = crypt(sess->arg, sp->sp_pwdp);
    //验证密码
    if(strcmp(encrypted, sp->sp_pwdp))
    {
        ftp_replay(sess, FTP_UNLOGIN, "Login incorrect");
    }
    setegid(pw->pw_gid);
    seteuid(pw->pw_uid);
    chdir(pw->pw_dir);
    ftp_replay(sess, FTP_LOGINSUCCESS, "Login successful");
}




static void do_cmd(session_t *sess)
{}
static void do_cdup(session_t *sess)
{}
static void do_quit(session_t *sess)
{}
static void do_port(session_t *sess)
{
    unsigned int v[6];
    sscanf(sess->arg, "%u, %u, %u, %u, %u", &v[2], &v[3], &v[4], &v[0], &v[1]);
    sess->port_addr = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));
    memset(sess->port_addr, 0, sizeof(struct sockaddr_in));
    sess->port_addr->sin_family = AF_INET;
    unsigned char *p = (unsigned char *)&sess->port_addr->sin_port;
    p[0] = v[0];
    p[1] = v[1];

    p = (unsigned char *)&sess->port_addr->sin_addr;
    p[0] = v[2];
    p[1] = v[3];
    p[2] = v[4];
    p[3] = v[5];
    
    ftp_replay(sess, FTP_PORTOK, "Port commond successful. Consider using PASV.");
}
static void do_pasv(session_t *sess)
{}
static void do_type(session_t *sess)
{
   if(strcmp(sess->arg, "A") == 0){
       sess->is_ascii = 1;
       ftp_replay(sess, FTP_TYPEOK, "Switching to ASCII mode");
   }else if(strcmp(sess->arg, "I") == 0){
       sess->is_ascii = 0;
       ftp_replay(sess, FTP_TYPEOK, "Switching to Binary mode");
   }else{
       ftp_replay(sess, FTP_BADCMD, "Unrecognised TYPE command");
   }

}
static void do_stru(session_t *sess)
{}
static void do_mode(session_t *sess)
{}
static void do_retr(session_t *sess)
{}
static void do_stor(session_t *sess)
{}
static void do_appe(session_t *sess)
{}
static void do_list(session_t *sess)
{
   //创建数据连接
    if(get_transfer_fd(sess) == 0){
        return;
    }
   //150
   ftp_replay(sess, FTP_DATACONNOK, "Here comes the directory listing");
   //传输列表
   list_common(sess);
   //关闭数据套接字
   close(sess->data_fd);
   //226
   ftp_replay(sess, FTP_TRANSOK, "Directory send OK");

}
static void do_nlst(session_t *sess)
{}
static void do_rest(session_t *sess)
{}
static void do_abor(session_t *sess)
{}
static void do_pwd(session_t *sess)
{
    char text[1024] = {0};
    char dir[1024] = {0};
    //获取当前路经
    getcwd(dir, 1024);
    sprintf(text, "\"%s\"", dir);
    ftp_replay(sess, FTP_PWDOK, text);
}
static void do_xpwd(session_t *sess)
{}
static void do_mkd(session_t *sess)
{}
static void do_xmkd(session_t *sess)
{}
static void do_rmd(session_t *sess)
{}
static void do_xrmd(session_t *sess)
{}
static void do_dele(session_t *sess)
{}
static void do_rnfr(session_t *sess)
{}
static void do_rnto(session_t *sess)
{}
static void do_site(session_t *sess)
{}
static void do_syst(session_t *sess)
{
    ftp_replay(sess, FTP_SYSTOK, "unity type: l8");
    //也可以在配置文件中设置，查找以后输出文本
    //暂不实现
}

static void do_feat(session_t *sess)
{
    
    ftp_lreplay(sess, FTP_FEAT, "Features:");
    writen(sess->ctrl_fd, " EPRT\r\n", strlen(" EPRT\r\n"));
    writen(sess->ctrl_fd, " EPSV\r\n", strlen(" EPSV\r\n"));
    writen(sess->ctrl_fd, " MDTN\r\n", strlen(" MDTN\r\n"));
    writen(sess->ctrl_fd, " PASV\r\n", strlen(" PASV\r\n"));
    writen(sess->ctrl_fd, " EPRT\r\n", strlen(" EPRT\r\n"));
    writen(sess->ctrl_fd, " REST STREAM\r\n", strlen(" REST STREAM\r\n"));
    writen(sess->ctrl_fd, " EPRT\r\n", strlen(" EPRT\r\n"));
    writen(sess->ctrl_fd, " TVFS\r\n", strlen(" TVFS\r\n"));
    writen(sess->ctrl_fd, " UTF8\r\n", strlen(" UTF8\r\n"));
    ftp_replay(sess, FTP_FEAT, "end");
}
static void do_size(session_t *sess)
{}
static void do_stat(session_t *sess)
{}
static void do_noop(session_t *sess)
{}
static void do_help(session_t *sess)
{}
static void do_allo(session_t *sess)
{}
