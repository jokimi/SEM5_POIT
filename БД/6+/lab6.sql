-- docker exec -it oraora21 bash

-- 1

-- cd /opt/oracle/homes/OraDB21Home1/network/admin
-- ls -l
-- cat sqlnet.ora
-- cat tnsnames.ora

-- 2

-- sqlplus system
-- password: system
-- show parameter;
-- select name, value from v$parameter;

-- 3

-- connect system/system@//localhost:1521/ORCLPDB1
-- show con_name;
-- select tablespace_name, file_name from dba_data_files;
-- select tablespace_name, file_name from dba_temp_files;
-- select role from dba_roles;
-- select username from dba_users;

-- 4

-- task04.jpg

-- 5

-- task05.jpg

-- 6

alter session set container = PodSproutPDB;
alter pluggable database PodSproutPDB open;
-- sqlplus sprout/sprout@//localhost:1521/PodSproutPDB

-- 7

-- create table myTable (a number, b number);
-- insert into myTable values (1, 2);
-- insert into myTable values (3, 4);
-- commit;
select * from myTable;

-- 8

-- help timing
-- set timing on;
-- select * from myTable;
-- set timing off;

-- 9

-- help describe;
-- describe myTable;

-- 10

select segment_name from user_segments;

-- 11

create view view_segments as select count(segment_name) segments_count, sum(extents) extents_count, sum(blocks) blocks_count, sum(bytes) memory_size from user_segments;
select * from view_segments;