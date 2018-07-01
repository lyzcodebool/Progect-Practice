/*mysql存储引擎：*/
查看mysql存储引擎种类的命令：show engines;
查看mysql的默认存储引擎：show variables like '% storge_engine';
查看某个数据库中具体的表使用的存储引擎：show table status from databases while name = "tablename";
着重区别myISAM和InnoDB:
1.InnoDB最重要的三个特性是（1）支持事务、回滚、系统崩溃修复能力（2）支持子增长列和外键（3）支持mvcc行级锁
2.myISAM的特性：（1）不支持事务（2）只支持并发插入的表锁，主要用于高负载的select
两种存储引擎的比较： myISAM适合于多读少写，InnoDB适合于多写少读，两者都采用B+tree索引，但是实现不同。


/*创建存储过程*/
SHOW procedure status
drop procedure 存储过程名字

delimiter $;
create procedure name(in | out| inout)
    begin 
        select * from tablename;
        /*批语句*/
    end$


/*创建视图*/
存储查询结果的虚拟表
Create view 视图名 as select语句
drop view 视图名
alter view 视图名 as select语句
为什么要视图：
1.简化查询
2.可以进行权限控制
3.大叔据分表的时候可用（id%表数+1、视图）

修改表视图改变
修改视图表不一定改变（因为表可能是众多行运算以后的结果，即一对多的情况，相反一对一的情况可以修改表但也要注意：对于insert视图必须包含没有默认值的列）

视图algorithm 
merge 就是一条语法规则，没有创建表，对视图的操作实际是和视图的语法规则合并形成一条select语句对原表的操作
temptable创建一张临时表，对视图的操作就是对临时表的操作
undefine由系统自己决定规则


/*触发器trigger*/
触发器的概念：见识某种情况并出发某种操作
例如：
一个电子商城商品表goods简称g：
主键   商品名    库存    
1      电脑      28
2      手表      120

订单表o：
订单主键    订单外键    购买数量
1           2           3
2           1           5
完成下单与减少库存逻辑：
Insert into o(商品外键， 购买数量) values （2, 3）;
Update g set 库存 = 库存-3 if id = 2;
这两个逻辑可以堪称一个整体，或者说insert肯定会引发update
这就可以用到触发器，我们可以监视某个表的变化，当变化发生，触发一个表的操作。


要注意：
触发器可以监视：增删改操作，没有查
触发器可以触发：增删改操作，没有查

实例：
//建立商品表
create table g(
               id int,
               name varchar(10),
               num int
              )charset utf8;
//建立订单表
create table o(
               oid int;
               gid int;
               much int;
              )charset utf8;

//添加商品表
insert into g values(1, '猪'， 22)， （2, ‘'羊'， 10）， (3, '狗', 13);
//买三只样
insert into o values(1, 2 3);
//减少商品表的库存（更新）
update g num = num -3 where id = 2;

触发器四要素：
监视地点：o表
监视操作：insert
触发操作：update
触发时间：after
因此上面的整个过程可以做成触发器（语法）：
delimiter $;
create tigger tiggerName
After/Before insert/update/detele on tableName
For each row //这句是固定的为行触发器，了解oracle数据库还有表触发器
begin
    insert/update/detele （sql语句); /*可以多句*/
end$
delimiter ;;

根据上面的实例一个可用的触发器：
//设计的关键是得到订单表的新行的数据，在触发器中引用
对于insert而言，新增的行用new表示,行中每一列的值用 new.列名 来表示

delimiter $;
create tigger tg1
after insert on o
for each row 
begin 
    update g set num = num - new.much where id = new.gid;
end$

这里增设一种情况：万一用户撤销了订单：
所以，修改订单的时候，库存也要改变
监视地点：o表
监视操作：detele
触发操作：update
触发时间：after
有了四要素，写tigger就很清楚了
对于detele而言，新增的行用old表示,行中每一列的值用 old.列名 来表示

create tigger tg2
after detele on o
for each row
befor
    update g set num = num + old.much where id = old.gid;
end$

再来看修改订单的数量时，库存相应改变：
对于update而言，被修改的行修改前用old表示,行中每一列的值用 old.列名 来表示 修改之后的
行用new表示,行中每一列的值用 new.列名 来表示
create tigger tg3
after detele on o
for each row
befor
    update g set num = num + old.much - new.much where id = old.gid;
end$

