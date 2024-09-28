-- 1

create tablespace TS_KEM
    datafile 'D:\BSTU\5 sem\DB\labs\2\datafiles\TS_KEM.dbf'
    size 7M
    autoextend on next 5M
    maxsize 20M
select TABLESPACE_NAME, STATUS, contents, logging from USER_TABLESPACES;
drop tablespace TS_KEM including contents and datafiles;

-- 2

create temporary tablespace TS_KEM_TEMP
    tempfile 'D:\BSTU\5 sem\DB\labs\2\datafiles\TS_KEM_TEMP.dbf'
    size 5M
    autoextend on next 3M
    maxsize 30M
select TABLESPACE_NAME, STATUS, contents, logging from USER_TABLESPACES;
drop tablespace TS_KEM_TEMP including contents and datafiles;

-- 3

select * from SYS.DBA_TABLESPACES;
SELECT TABLESPACE_NAME, FILE_NAME, BYTES / 1024 / 1024 AS SIZE_MB FROM DBA_DATA_FILES;
SELECT TABLESPACE_NAME, FILE_NAME, BYTES / 1024 / 1024 AS SIZE_MB FROM DBA_TEMP_FILES;

-- 4

create role RL_KEMCORE;
GRANT CONNECT TO RL_KEMCORE;
GRANT CREATE TABLE TO RL_KEMCORE;
GRANT CREATE VIEW TO RL_KEMCORE;
GRANT CREATE PROCEDURE TO RL_KEMCORE;
drop role RL_KEMCORE;

-- 5

select * from DBA_ROLES where ROLE = 'RL_KEMCORE';
select * from DBA_SYS_PRIVS where GRANTEE = 'RL_KEMCORE';

-- 6
create profile PF_KEMCORE limit
    failed_login_attempts 7
    sessions_per_user 3
    password_life_time 60
    password_reuse_time 365
    password_lock_time 1
    connect_time 180
    idle_time 30;
drop profile PF_KEMCORE;
    
-- 7

select PROFILE from DBA_PROFILES;
select * from DBA_PROFILES where profile = 'PF_KEMCORE';
select * from DBA_PROFILES where profile = 'DEFAULT';

-- 8

create user KEMCORE
    identified by 111
    default tablespace TS_KEM
    temporary tablespace TS_KEM_TEMP
    profile PF_KEMCORE
    account unlock
    password expire;
grant
    create session,
    create table,
    create view,
    drop any table,
    drop any view
to KEMCORE;
drop user KEMCORE;

-- 9

-- task9.jpg

-- 10

create table anyTable (
    id number
);

create view anyView as select * from anyTable;

drop view anyView;
drop table anyTable;

-- 11

create tablespace KEM_QDATA
    datafile 'D:\BSTU\5 sem\DB\labs\2\datafiles\KEM_QDATA.dbf'
    size 10M
    offline;
select TABLESPACE_NAME, STATUS, contents logging from USER_TABLESPACES;
alter tablespace KEM_QDATA online;
alter user KEMCORE quota 2M on KEM_QDATA;

drop tablespace KEM_QDATA including contents and datafiles;

create table tableTask11 (
    id number,
    name varchar(10)
) tablespace KEM_QDATA;

insert into tableTask11 values (1, 'one');
insert into tableTask11 values (2, 'two');
insert into tableTask11 values (3, 'three');

select * from tableTask11;
drop table tableTask11;