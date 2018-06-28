mysql存储引擎：
查看mysql存储引擎种类的命令：show engines;
查看mysql的默认存储引擎：show variables like '% storge_engine';
查看某个数据库中具体的表使用的存储引擎：show table status from databases while name = "tablename";
着重区别myISAM和InnoDB:
1.InnoDB最重要的三个特性是（1）支持事务、回滚、系统崩溃修复能力（2）支持子增长列和外键（3）支持mvcc行级锁
2.myISAM的特性：（1）不支持事务（2）只支持并发插入的表锁，主要用于高负载的select
两种存储引擎的比较： myISAM适合于多读少写，InnoDB适合于多写少读，两者都采用B+tree索引，但是实现不同。

