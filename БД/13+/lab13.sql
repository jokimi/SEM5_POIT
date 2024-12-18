alter session set nls_date_format='dd-mm-yyyy hh24:mi:ss';
grant create tablespace, drop tablespace to KEM;
alter user KEM quota unlimited on ts1;
alter user KEM quota unlimited on ts2;
alter user KEM quota unlimited on ts3;
alter user KEM quota unlimited on ts4;

create tablespace ts1
    datafile '/opt/oracle/ts1-kem.dbf'
    size 7 m
    autoextend on
    maxsize unlimited
    extent management local;
create tablespace ts2
    datafile '/opt/oracle/ts2-kem.dbf'
    size 7 m
    autoextend on
    maxsize unlimited
    extent management local;
create tablespace ts3
    datafile '/opt/oracle/ts3-kem.dbf'
    size 7 m
    autoextend on
    maxsize unlimited
    extent management local;
create tablespace ts4
    datafile '/opt/oracle/ts4-kem.dbf'
    size 7 m
    autoextend on
    maxsize unlimited
    extent management local;

drop tablespace ts1 including contents and datafiles;
drop tablespace ts2 including contents and datafiles;
drop tablespace ts3 including contents and datafiles;
drop tablespace ts4 including contents and datafiles;

-- 1, 5

drop table T_RANGE;
create table T_RANGE (
    id number,
    time_id date
) partition by range (id) (
    partition p0 values less than (100) tablespace ts1,
    partition p1 values less than (200) tablespace ts2,
    partition p2 values less than (300) tablespace ts3,
    partition PMAX values less than (maxvalue) tablespace ts4
);

begin
    for i in 1..400
    loop
        insert into T_RANGE(id, time_id) values (i, sysdate);
    end loop;
end;

select * from T_RANGE partition(p0);
select * from T_RANGE partition(p1);
select * from T_RANGE partition(p2);
select * from T_RANGE partition(pmax);

select TABLE_NAME, PARTITION_NAME, HIGH_VALUE, TABLESPACE_NAME from USER_TAB_PARTITIONS
    where table_name = 'T_RANGE';

-- 2, 5

drop table T_INTERVAL;
create table T_INTERVAL (
    id number,
    time_id date
) partition by range (time_id)
interval (numtoyminterval(1, 'month')) (
    partition p0 values less than (to_date('01-10-2005', 'dd-mm-yyyy')) tablespace ts1,
    partition p1 values less than (to_date('01-10-2010', 'dd-mm-yyyy')) tablespace ts2,
    partition p2 values less than (to_date('01-10-2015', 'dd-mm-yyyy')) tablespace ts3
);

insert into T_INTERVAL(id, time_id) values (50, '01-01-1996');
insert into T_INTERVAL(id, time_id) values (105, '01-01-2000');
insert into T_INTERVAL(id, time_id) values (105, '01-01-2004');
insert into T_INTERVAL(id, time_id) values (205, '01-01-2008');
insert into T_INTERVAL(id, time_id) values (305, '01-01-2012');
insert into T_INTERVAL(id, time_id) values (405, '01-01-2016');
insert into T_INTERVAL(id, time_id) values (505, '01-01-2020');
commit;

select * from T_INTERVAL partition (p0);
select * from T_INTERVAL partition (p1);
select * from T_INTERVAL partition (p2);

select TABLE_NAME, PARTITION_NAME, HIGH_VALUE, TABLESPACE_NAME
    from USER_TAB_PARTITIONS
    where table_name = 'T_INTERVAL';

-- 3, 5

drop table T_HASH;
create table T_HASH (
    str varchar2(50),
    id  number
) partition by hash (str) (
    partition k0 tablespace ts1,
    partition k1 tablespace ts2,
    partition k2 tablespace ts3,
    partition k3 tablespace ts4
);

insert into T_HASH (str, id) values ('so', 1);
insert into T_HASH (str, id) values ('some', 2);
insert into T_HASH (str, id) values ('some st', 3);
insert into T_HASH (str, id) values ('some stri', 4);
insert into T_HASH (str, id) values ('some string', 7);
insert into T_HASH (str, id) values ('ssss', 14);
insert into T_HASH (str, id) values ('gggggg', 32);
commit;

select * from T_HASH partition (k0);
select * from T_HASH partition (k1);
select * from T_HASH partition (k2);
select * from T_HASH partition (k3);

-- 4, 5

drop table T_LIST;
create table T_LIST (
    obj char(3)
) partition by list(obj) (
    partition l0 values ('a') tablespace ts1,
    partition l1 values ('b') tablespace ts2,
    partition l2 values ('c') tablespace ts3,
    partition l3 values (default) tablespace ts4
);

insert into T_list(obj) values('a');
insert into T_list(obj) values('b');
insert into T_list(obj) values('c');
insert into T_list(obj) values('d');
insert into T_list(obj) values('e');
commit;

select * from T_list partition (l0);
select * from T_list partition (l1);
select * from T_list partition (l2);
select * from T_list partition (l3);

-- 6

alter table T_RANGE enable row movement;
select * from T_RANGE partition (PMAX);
update T_RANGE set id = 2 where id = 300;
select * from T_RANGE partition(p0) order by id;

alter table T_INTERVAL enable row movement;
select * from T_INTERVAL partition (p0);
update T_INTERVAL set time_id = to_date('01-02-1990') where id = 50;
select * from T_INTERVAL partition(p2);

alter table T_HASH enable row movement;
select * from T_HASH partition (k2);
update T_HASH set str = 'string' where id = 3;
select * from T_HASH partition(k3);

alter table T_LIST enable row movement;
select * from T_LIST partition(l0);
update T_LIST set obj = 'b' where obj = 'a';
select * from T_LIST partition(l1);

-- 7

alter table T_RANGE merge partitions p1, p2 into partition p5 tablespace ts4;
select * from T_RANGE partition(p5);

-- 8

alter table T_RANGE split partition p5 at (200)
    into (partition p1 tablespace ts1, partition p2 tablespace ts2);
select * from T_RANGE partition(p5);
select * from T_RANGE partition(p1);
select * from T_RANGE partition(p2);

-- 9

drop table T_RANGE1;
create table T_RANGE1 (
    id number,
    TIME_ID date
);
alter table T_RANGE exchange partition p0 with table T_RANGE1 without validation;
select * from T_RANGE partition (p0);
select * from T_RANGE1;