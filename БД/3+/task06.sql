alter session set container = KEM_PDB;
show con_name;

-- tablespace

CREATE TABLESPACE TS_KEM_lab3
        DATAFILE 'D:\BSTU\5 sem\DB\labs\3\TS_KEM_lab3'
        SIZE 7M
        AUTOEXTEND ON NEXT 5M
        MAXSIZE 100 M;
DROP TABLESPACE TS_KEM_lab3 INCLUDING CONTENTS and DATAFILES;

-- profile

CREATE PROFILE PF_KEMCORE_lab3 LIMIT
    FAILED_LOGIN_ATTEMPTS 7
    SESSIONS_PER_USER 3
    PASSWORD_LIFE_TIME 60
    PASSWORD_REUSE_TIME 365
    PASSWORD_LOCK_TIME 1
    CONNECT_TIME 180
    IDLE_TIME 30;
drop profile PF_KEMCORE_lab3;

-- user

CREATE USER U1_KEM_PDB IDENTIFIED BY 1
    DEFAULT TABLESPACE TS_KEM_lab3 quota unlimited on TS_KEM_lab3
    PROFILE PF_KEMCORE_lab3
    ACCOUNT UNLOCK
    PASSWORD EXPIRE;
GRANT CREATE SESSION TO U1_KEM_PDB;
GRANT
    CREATE TABLE,
    CREATE VIEW
    TO U1_KEM_PDB;
GRANT
    INSERT any table
    to U1_KEM_PDB;
GRANT sysdba to U1_KEM_PDB;
DROP USER U1_KEM_PDB cascade;
select * from dba_users;

-- role

create role KEM_PDB_RL;
grant 
    connect, 
    create session,
    alter session,
    create any table, 
    create any view,
    create any procedure
to KEM_PDB_RL;
drop role KEM_PDB_RL;