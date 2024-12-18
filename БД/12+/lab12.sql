alter session set container = ORCLPDB1;
grant create trigger, drop any trigger to KEM;
alter session set nls_date_format = 'dd-mm-yyyy hh24:mi:ss';

-- 1

create table KEM.task1 (
    a int primary key,
    b varchar(30)
);
drop table KEM.task1;

-- 2

begin
    for i in 1..10
        loop
            insert into KEM.task1 values (i, 'a');
        end loop;
end;
select * from KEM.task1;

-- 3

create or replace trigger InsertTRBeforeStatement
    before insert on KEM.task1
begin
    dbms_output.put_line('InsertTRBeforeStatement');
end;
create or replace trigger UpdateTRBeforeStatement
    before update on KEM.task1
begin
    dbms_output.put_line('UpdateTRBeforeStatement');
end;
create or replace trigger DeleteTRBeforeStatement
    before delete on KEM.task1
begin
    dbms_output.put_line('DeleteTRBeforeStatement');
end;

drop trigger InsertTRBeforeStatement;
drop trigger UpdateTRBeforeStatement;
drop trigger DeleteTRBeforeStatement;

-- 4

insert into KEM.task1 values (55, 'xx');
update KEM.task1 set b = 'yy' where a = 55;
delete KEM.task1 where a = 55;
select * from KEM.task1;

-- 5

create or replace trigger InsertTRBeforeRow
    before insert on KEM.task1 for each row
begin
    dbms_output.put_line('InsertTRBeforeRow');
end;
create or replace trigger UpdateTRBeforeRow
    before update on KEM.task1 for each row
begin
    dbms_output.put_line('UpdateTRBeforeRow');
end;
create or replace trigger DeleteTRBeforeRow
    before delete on KEM.task1 for each row
begin
    dbms_output.put_line('DeleteTRBeforeRow');
end;

drop trigger InsertTRBeforeRow;
drop trigger UpdateTRBeforeRow;
drop trigger DeleteTRBeforeRow;

insert into KEM.task1 values (55, 'xx');
insert into KEM.task1 values (56, 'aa');
update KEM.task1 set b = 'yy' where a = 55 or a = 56;
delete KEM.task1 where a = 55 or a = 56;
select * from KEM.task1;

-- 6

create or replace trigger TriggerDML
    before insert or update or delete on KEM.task1 for each row
begin
    if INSERTING then
        dbms_output.put_line('TriggerDMLBeforeInsert');
    elsif UPDATING then
        dbms_output.put_line('TriggerDMLBeforeUpdate');
    elsif DELETING then
        dbms_output.put_line('TriggerDMLBeforeDelete');
    end if;
end;

drop trigger TriggerDML;

insert into KEM.task1 values (55, 'xx');
update KEM.task1 set b = 'yy' where a = 55;
delete KEM.task1 where a = 55;
select * from KEM.task1;

-- 7

create or replace trigger InsertTRAfterStatement
    after insert on KEM.task1
begin
    dbms_output.put_line('InsertTRAfterStatement');
end;
create or replace trigger UpdateTRAfterStatement
    after update on KEM.task1
begin
    dbms_output.put_line('UpdateTRAfterStatement');
end;
create or replace trigger DeleteTRAfterStatement
    after delete on KEM.task1
begin
    dbms_output.put_line('DeleteTRAfterStatement');
end;

drop trigger InsertTRAfterStatement;
drop trigger UpdateTRAfterStatement;
drop trigger DeleteTRAfterStatement;

insert into KEM.task1 values (55, 'xx');
update KEM.task1 set b = 'yy' where a = 55;
delete KEM.task1 where a = 55;
select * from KEM.task1;

-- 8

create or replace trigger InsertTRAfterRow
    after insert on KEM.task1 for each row
begin
    dbms_output.put_line('InsertTRAfterRow');
end;
create or replace trigger UpdateTRAfterRow
    after update on KEM.task1 for each row
begin
    dbms_output.put_line('UpdateTRAfterRow');
end;
create or replace trigger DeleteTRAfterRow
    after delete on KEM.task1 for each row
begin
    dbms_output.put_line('DeleteTRAfterRow');
end;

drop trigger InsertTRAfterRow;
drop trigger UpdateTRAfterRow;
drop trigger DeleteTRAfterRow;

insert into KEM.task1 values (55, 'xx');
insert into KEM.task1 values (56, 'aa');
update KEM.task1 set b = 'yy' where a = 55 or a = 56;
delete KEM.task1 where a = 55 or a = 56;
select * from KEM.task1;

-- 9

create table AUDITS (
    OperationDate date,
    OperationType varchar2(50),
    TriggerName varchar2(50),
    Data varchar2(40)
);
drop table AUDITS;

-- 10

create or replace trigger TriggerBeforeAUDIT
    before insert or update or delete on KEM.task1 for each row
begin
    if inserting then
        dbms_output.put_line('TriggerBeforeAUDIT - INSERT');
        insert into AUDITS values (sysdate, 'insert', 'TriggerBeforeAUDIT', :new.a || ' ' || :new.b);
    elsif updating then
        dbms_output.put_line('TriggerBeforeAUDIT - UPDATE');
        insert into AUDITS values (sysdate, 'update', 'TriggerBeforeAUDIT', :old.a || ' ' || :old.b || ' -> ' || :new.a || ' ' || :new.b);
    elsif deleting then
        dbms_output.put_line('TriggerBeforeAUDIT - DELETE');
        insert into AUDITS values (sysdate, 'delete', 'TriggerDeforeAUDIT', :old.a || ' ' || :old.b);
    end if;
end;

create or replace trigger TriggerAfterAUDIT
    after insert or update or delete on KEM.task1 for each row
begin
    if inserting then
        dbms_output.put_line('TriggerAfterAUDIT - INSERT');
        insert into AUDITS values (sysdate, 'insert', 'TriggerAfterAUDIT', :new.a || ' ' || :new.b);
    elsif updating then
        dbms_output.put_line('TriggerAfterAUDIT - UPDATE');
        insert into AUDITS values (sysdate, 'update', 'TriggerAfterAUDIT', :old.a || ' ' || :old.b || ' -> ' || :new.a || ' ' || :new.b);
    elsif deleting then
        dbms_output.put_line('TriggerAfterAUDIT - DELETE' );
        insert into AUDITS values (sysdate, 'delete', 'TriggerAfterAUDIT', :old.a || ' ' || :old.b);
    end if;
end;

drop trigger TriggerBeforeAUDIT;
drop trigger TriggerAfterAUDIT;

insert into KEM.task1 values (55, 'xx');
insert into KEM.task1 values (56, 'aa');
update KEM.task1 set b = 'yy' where a = 55 or a = 56;
delete KEM.task1 where a = 55 or a = 56;
select * from AUDITS;
truncate table AUDITS;

-- 11

insert into KEM.task1 values (1, 'v');

-- 12

create table KEM.task1 (
    a int primary key,
    b varchar(30)
);
drop table KEM.task1;

create or replace trigger TriggerPreventTableDrop
    before drop on database
declare
    v_object_name varchar2(50);
    v_object_type varchar2(50);
begin
    v_object_name := ORA_DICT_OBJ_NAME;
    select object_type into v_object_type from all_objects
        where OBJECT_NAME = upper(v_object_name) and OWNER = 'KEM';
    if v_object_type = 'TABLE' and v_object_name = 'TASK1' then
        raise_application_error(-20001, 'You cannot delete table TASK1');
    end if;
end;

drop trigger TriggerPreventTableDrop;

-- 13

drop table AUDITS;

-- 14-15

drop view KEM.taskTableView;
create view KEM.taskTableView as select * from KEM.task1;
create or replace trigger TriggerInsteadOfInsert
    instead of insert on KEM.taskTableView
begin
    if INSERTING then
        DBMS_OUTPUT.PUT_LINE('TriggerInsteadOfInsert');
        insert into KEM.task1 values (100, 'www');
    end if;
end TriggerInsteadOfInsert;

select * from KEM.taskTableView;
insert into KEM.taskTableView values (111, 'eee');