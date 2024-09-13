-- 9

create table KEM_t (
    nnumber number(3),
    sstring varchar2(50),
    primary key (nnumber)
);

-- 11

insert into KEM_t (nnumber, sstring) values (100, 'One');
insert into KEM_t (nnumber, sstring) values (200, 'Two');
insert into KEM_t (nnumber, sstring) values (300, 'Three');
commit;

-- 12

update KEM_t
set sstring = 'Modified' where nnumber like 100;
commit;

-- 13

select COUNT (*) as countLines from KEM_t;
select sum (nnumber) as sumOfNumbers from KEM_t;
select POWER (nnumber, 2) as squareOfNumber from KEM_t;
select nnumber, sstring from KEM_t;

-- 14

delete from KEM_t where power (nnumber, 2) like 90000;
commit;

-- 15

create table KEM_t1 (
    nnumber number(3),
    sstring varchar2(50) default '---',
    foreign key (nnumber) references KEM_t(nnumber)
);
insert into KEM_t1 (nnumber, sstring) values (100, 'Four');
insert into KEM_t1 (nnumber) values (200);
commit;

-- 16

select * from KEM_t inner join KEM_t1 on KEM_t.nnumber = KEM_t1.nnumber;
select * from KEM_t left outer join KEM_t1 on KEM_t.nnumber = KEM_t1.nnumber;
select * from KEM_t right outer join KEM_t1 on KEM_t.nnumber = KEM_t1.nnumber;

-- 18

drop table KEM_t1;
drop table KEM_t;