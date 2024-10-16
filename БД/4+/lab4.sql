
-- 1

select * from DBA_TABLESPACES;
select TABLESPACE_NAME, FILE_NAME from SYS.DBA_DATA_FILES;
select TABLESPACE_NAME, FILE_NAME from SYS.DBA_TEMP_FILES;

-- 2

drop tablespace KEM_QDATA including contents and datafiles;
create tablespace KEM_QDATA
    datafile 'D:\BSTU\5 sem\DB\labs\4\KEM_QDATA'
    size 10m
    extent management local
    offline;
alter tablespace KEM_QDATA online;
ALTER SESSION SET "_ORACLE_SCRIPT" = TRUE;
-- drop user c##kem4 cascade;
-- create user C##KEM4 identified by kem4 DEFAULT TABLESPACE KEM_QDATA quota unlimited on KEM_QDATA ACCOUNT UNLOCK;
grant create session,create table, create view,
    create procedure,drop any table,drop any view,
    drop any procedure to C##KEM4;
alter user C##KEM4 quota 2m on KEM_QDATA;
--> KEM4 user
create table KEM_T1(
    x number(20) primary key,
    y number(20)
) tablespace KEM_QDATA;
insert into KEM_T1 values (1, 11);
insert into KEM_T1 values (2, 22);
insert into KEM_T1 values (3, 33);
commit;
select * from KEM_T1;

-- 3

SELECT SEGMENT_NAME, SEGMENT_TYPE, TABLESPACE_NAME, BYTES, BLOCKS, EXTENTS FROM USER_SEGMENTS WHERE TABLESPACE_NAME = 'KEM_QDATA';
select * from user_segments;

-- 4

drop table KEM_T1;
select * from user_segments where tablespace_name = 'KEM_QDATA';
select object_name, original_name, operation, type, ts_name, createtime, droptime from USER_RECYCLEBIN;

-- 5

flashback table KEM_T1 to before drop;
SELECT * FROM KEM_T1;

-- 6

declare
    i integer := 4;
begin
    while i < 10004 loop
        insert into KEM_T1(x, y) values (i, i);
        i := i + 1;
    end loop;
end;
select count(*) from KEM_T1;
SELECT * from KEM_T1;

-- 7

SELECT * FROM USER_SEGMENTS;
SELECT * FROM USER_EXTENTS;

-- 8

--> admin
drop tablespace KEM_QDATA including contents and datafiles;
select * from SYS.DBA_TABLESPACES;

-- 9

SELECT * FROM V$LOG ORDER BY GROUP#;
select GROUP# from v$log where STATUS = 'CURRENT';

-- 10

SELECT * FROM V$LOGFILE ORDER BY GROUP#;

-- 11

alter system switch logfile;
select GROUP# from v$log where STATUS = 'CURRENT';
select current_timestamp from SYS.DUAL;

-- 12

alter database add logfile group 4 '/opt/oracle/oradata/ORCLCDB/redo-044.log' size 50m blocksize 512;
alter database add logfile member '/opt/oracle/oradata/ORCLCDB/redo-04-14.log' to group 4;
alter database add logfile member '/opt/oracle/oradata/ORCLCDB/redo-04-24.log' to group 4;
SELECT * FROM V$LOG ORDER BY GROUP#;
select GROUP#, MEMBER, STATUS from v$logfile;
-- switching to 4th group
alter system switch logfile;
select GROUP#, STATUS from v$log;

-- 13

alter database drop logfile member '/opt/oracle/oradata/ORCLCDB/redo-04-1.log';
alter database drop logfile member '/opt/oracle/oradata/ORCLCDB/redo-04-2.log';
alter database clear unarchived logfile group 4;
alter database drop logfile group 4;
select * from v$logfile;

-- 14

select instance_name, archiver from v$instance;
SELECT DBID, NAME, LOG_MODE FROM V$DATABASE;

-- 15

select MAX(SEQUENCE#) from v$archived_log;

-- 16

select instance_name, archiver from v$instance;
select LOG_MODE from V$DATABASE;

-- 17

ALTER SYSTEM SET LOG_ARCHIVE_DEST_1 ='LOCATION=/opt/oracle/oradata/ORCLCDB/archive1.arc';
alter system switch logfile;
select * from v$archived_log;
SELECT * FROM V$LOG ORDER BY GROUP#;

-- 18

select instance_name, archiver from v$instance;
select LOG_MODE from V$DATABASE;

-- 19

select * from V$CONTROLFILE;

-- 20

show parameter control;
SELECT * FROM V$CONTROLFILE_RECORD_SECTION;

-- 21

select * from v$parameter where name = 'spfile';
show parameter spfile;

-- 22

create pfile = '/opt/oracle/dbs/KEM_PFILE.ora' from spfile;

-- 23

select * from v$pwfile_users;
SHOW PARAMETER REMOTE_LOGIN_PASSWORDFILE;

-- 24

select * from V$DIAG_INFO;
show parameter background_dump_dest;

-- 25

select distinct segment_type from dba_segments;
