alter session set container = ORCLPDB1;

-- 1

select * from DBA_USERS;
grant create any view to KEM;
grant create any table to KEM;
grant create sequence to KEM;
grant unlimited tablespace to KEM;
grant restricted session to KEM;
grant create cluster to KEM;
grant create synonym to KEM;
grant create public synonym to KEM;
grant create materialized view to KEM;

-- 2

create sequence S1
    start with 1000
    increment by 10
    nominvalue
    nomaxvalue
    nocycle
    nocache
    noorder;
select S1.nextval from DUAL;
select S1.nextval from DUAL;
select S1.nextval from DUAL;
select S1.currval from DUAL;
drop sequence S1;

-- 3-4

create sequence S2
    start with 10
    increment by 10
    maxvalue 100
    nocycle;
select S2.nextval from DUAL;
drop sequence S2;

-- 5

create sequence S3
    start with 10
    increment by -10
    minvalue -100
    maxvalue 10
    nocycle
    order;
select S3.nextval from DUAL;
drop sequence S3;

-- 6

create sequence S4
    start with 10
    increment by 1
    minvalue 10
    maxvalue 20
    cycle
    cache 5
    noorder;
select S4.nextval from DUAL;
drop sequence S4;

-- 7

select * FROM dba_sequences;

-- 8

create table T1 (
    N1 number(20),
    N2 number(20),
    N3 number(20),
    N4 number(20)
) cache storage ( buffer_pool keep );
begin
  for i in 1..7 loop
    insert into T1 values (S1.nextval, S2.nextval, S3.nextval, S4.nextval);
  end loop;
end;
select * from T1;
drop table T1;

-- 9

create cluster ABC (
    X number(10),
    V varchar2(12)
) size 200 hashkeys 200;
drop cluster ABC;

-- 10

create table A (
    XA number(10),
    VA varchar2(12),
    Y number(10)
) cluster ABC (XA, VA);
drop table A;

-- 11

create table B (
    XB number(10),
    VB varchar2(12),
    Z number(10)
) cluster ABC (XB, VB);
drop table B;

-- 12

create table C (
    XC number(10),
    VC varchar2(12),
    W number(10)
) cluster ABC (XC, VC);
insert into C values (1, '1', 1);
select * from C;
drop table C;

-- 13

select table_name from user_tables
    where table_name = 'A'
    or table_name = 'B'
    or table_name = 'C';
select cluster_name from user_clusters;

-- 14

select * from all_objects where object_name = 'C';
create synonym SC for SYS.C;
select * from SC;
drop synonym SC;

-- 15

create public synonym SB for SYS.B;
select * from SB;
drop public synonym SB;

-- 16

create table A16 (
    XA number(10),
    VA varchar2(12),
    Y number(10),
    constraint PK_A16 primary key (XA)
);
create table B16 (
  XB number(10),
  VB varchar2(12),
  Z number(10),
  constraint FK_B16 foreign key (XB) references A16(XA)
);
drop table B16;
drop table A16;
insert into A16 values (1, 'jokimi', 2);
insert into B16 values (1, 'keikenny', 2);
create view V1 as
    select * from A16
    inner join B16 on A16.XA = B16.XB;
select * from V1;
drop view V1;

-- 17

create materialized view MV
    refresh complete on demand
    start with SYSDATE
    next SYSDATE + numtodsinterval(2, 'MINUTE')
    as
    select * from A16
    inner join B16 on A16.XA = B16.XB;
drop materialized view MV;
select * from MV;
insert into A16 values (3, 'kimims', 2);
insert into B16 values (1, 'kknny', 2);
insert into A16 values (2, 'jokies', 3);
insert into B16 values (2, 'jokeiminny', 3);
commit;
select * from MV;