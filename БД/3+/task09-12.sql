
-- 9

GRANT CREATE SESSION TO C##KEM;
ALTER USER C##KEM DEFAULT TABLESPACE users;
ALTER USER C##KEM QUOTA UNLIMITED ON users;

-- 10

GRANT CREATE SESSION TO C##NOTKEM;
ALTER USER C##NOTKEM DEFAULT TABLESPACE users;
ALTER USER C##NOTKEM QUOTA UNLIMITED ON users;
SELECT * FROM V$SESSION WHERE USERNAME IS NOT NULL;

create user C##NOTKEM
    identified by notkem;
grant 
    connect, 
    create session, 
    alter session, 
    create any table,
    drop any table,
    SYSDBA
to C##NOTKEM container = all;
select * from dba_users where USERNAME like '%C##%';

SELECT * FROM V$SESSION WHERE USERNAME IS NOT NULL;

SELECT * FROM DBA_DATA_FILES;

DROP PLUGGABLE DATABASE KEM_PDB INCLUDING DATAFILES;
drop user C##NOTKEM;