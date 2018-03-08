#ifndef _FTP_PROTO_H_
#define _FTP_PROTO_H_
#include"session.h"
void handle_child(session_t *sess);
int list_common(session_t *sess);

#define FTP_UNLOGIN 530
#define FTP_LOGINSUCCESS 230
#define FTP_COMMANDNOTIMPL 502
#define FTP_BADCMD 500
#define FTP_PASS 331
#define FTP_SYSTOK 215 
#define FTP_FEAT 211
#define FTP_PWDOK 257
#define FTP_TYPEOK 200
#define FTP_PORTOK 200
#define FTP_DATACONNOK 150
#define FTP_TRANSOK 226
#endif

