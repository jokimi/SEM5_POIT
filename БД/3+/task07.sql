create table KEM_table (
    id int,
    message varchar(20)
);
INSERT INTO KEM_table VALUES (1, 'msg1');
INSERT INTO KEM_table VALUES (2, 'msg2');
INSERT INTO KEM_table VALUES (3, 'msg3');
select * from KEM_table;
drop table KEM_table;