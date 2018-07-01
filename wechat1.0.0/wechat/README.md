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


