编写过程中常用到的错误码：

bind：
-------------------------------------
EACCES：被绑定的地址受保护，超级用户才能访问
EADDRINUSE：绑定的地址正在使用中，可能是处于timewait状态的地址
listen
------------------------------------
ECONNREFUSED：请求链接的数量达到了监听队列的最大长度backlog（达到了半链接（SYN_RECV）和全链接（ESTABLISHED）的socket上限）
connect
------------------------------------
ECONNREFUSED:目标端口不存在，连接被拒绝。
ETIMEDOUT：连接超时。


linux常用命令：

1.ls cd rm-rf rmdir mkdir(-p) touch chmod chown(改变拥有者) 
cat(-n) tac(逆序显示)  tail less(支持搜索) more head  
cp mv scp df du dd tree pstree(进程关系树)
ls--->list  -l|-a|-i
ls  文件类型（目录d+普通文件—+字符文件c+socket文件s）+ 当前用户+当前的组，其他组 
每命令都有可执行文件/bin/命令或者/sbin/命令    

#########开机启动密码输入错误返回慢


netstat -nt|-u|-a (协议，recv-Q, send-Q, host:port, foreign:port 状态) 
tcpdump -i 网卡 port 端口
telnet ip 端口 (登录，对方端口要打开) 
ping 
host  
arp 
route 
killpid 
ifconfig


ping .... telnet ....（远程主机的cpu跑满） 


kill
killpid
kill all
ps -ef
strace 
pkill(杀父进程)
pstree
nice
rnice
top
pstack
pmap

修改只读的数据
访问受保护的地址，指针赋值0
访问不存在的内存，null
内存越界
整数按照字符串方式打印（实际也是访问不存在内存或者受保护的内存）



 


