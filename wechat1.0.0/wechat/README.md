# 编写过程中常用到的错误码：

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
ETIMEDOUT：连接超时。 会尝试多次发送syn包  默认64s或者75秒
EHOSTUNREACH和ENETUNREACH路由器不可达会返回icmp校验包标记路由不可达

read/write
------------------------------------------


linux常用命令：

1.ls cd rm-rf rmdir mkdir(-p) touch chmod chown(改变拥有者) 
cat(-n) tac(逆序显示)  tail less(支持搜索) more head  
cp mv scp df du dd tree pstree(进程关系树)
ls--->list  -l|-a|-i
ls  文件类型（目录d+普通文件—+字符文件c+socket文件s）+ 当前用户+当前的组，其他组 
每命令都有可执行文件/bin/命令或者/sbin/命令    

#########开机启动密码输入错误返回慢
文件操作：
ls
lsof查看正在使用的文件
cd
mkdir
rmdir
touch
rm -r-i-f
cp
mv
du显示当前目录所占的磁盘空间 -h
df显示目前磁盘剩余空间 -h
cat
tail -f 显示另一个进程日志文件的增长（实时）
more
less带搜索功能
pwd
clear
grep查找文件中符合字符串的那行
find
chmod修改文件权限
free
fdisk-l显示磁盘信息

进程操作：
ps -aux|-ef
pstree
top
htop
iotop监控磁盘io
nice通过给定的优先值启动一个进程
renice -n -p pid设置进程的优先级
kill -9
killall -9
pkill pid
ulimit
fg，bg后台运行，调到前台jobs列出所有当前进程
ipcs进程通信设施状态
strace
pstack
top -H -p
pmap

网络操作：
---------网络参数配置
ifconfig
route
ip
--------网络侦错与观察
ping
traceroute
netstat
host
IPTraf实时ip局域网监控
-------远程联机
telnet
ftp
-------网页浏览
wget
-------封包截取
tcpdump
nc模拟客户端服务器

性能诊断：
uptime
dmesg|tail输出系统日志倒叙
vmstat虚拟内存统计
iostat
sar -n DEV 1查看网络设备的吞吐率
sar -n TCP,ETCP 1查看tcp连接状态



netstat -nt|-u|-a (协议，recv-Q, send-Q, host:port, foreign:port 状态) 
tcpdump -i 网卡 port 端口
telnet ip 端口 (登录，对方端口要打开) 
ping 
host  
arp 
route 
kill PID
ifconfig
ping .... telnet ....（远程主机的cpu跑满） 


kill
killall
ps -ef
strace 
pkill(杀父进程)
pstree
nice
rnice
top
pstack
pmap









 


