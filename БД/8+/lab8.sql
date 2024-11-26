alter session set container = ORCLPDB1;

-- 1

begin
    NULL;
end;

-- 2

set serveroutput on;
begin dbms_output.put_line('Hello World!'); end;
set serveroutput off;

-- 3

declare
    v_num number;
begin
    v_num := 1/0;
exception
    when others then
        dbms_output.put_line('Error: ' || sqlerrm);
        dbms_output.put_line('Error code: ' || sqlcode);
end;

-- 4

declare
    v_num number;
begin
    declare
    begin
        v_num := 1/0;
    exception
        when others then
            dbms_output.put_line('Error: ' || sqlerrm);
            dbms_output.put_line('Error code: ' || sqlcode);
    end;
    dbms_output.put_line('Hello World!');
end;

-- 5

show parameter plsql_warnings;
select type, name, value from v$parameter where name = 'plsql_warnings';

-- 6

select keyword from v$reserved_words
    where length = 1;

-- 7

select keyword from v$reserved_words
    where length > 1
    order by keyword;

-- 8

show parameter plsql;
select name, value from v$parameter
    where name like '%plsql%';

-- 10 - 11

declare
    n1 number(10) := 21;
    n2 number(10) := 2;
    n3 number(10);
begin
    dbms_output.put_line('n1 = ' || n1);
    dbms_output.put_line('n2 = ' || n2);
    n3 := n1 + n2;
    dbms_output.put_line('n1 + n2 = ' || n3);
    n3 := n1 / n2;
    dbms_output.put_line('n1 / n2 = ' || n3);
end;

-- 12 - 13

declare
    n4 number(10, 5) := 123.12;
    n5 number(10, -2) := 154.99;
    n6 number(10, -2) := 123.52;
    n7 number(6, 2) := 1234.56 ;
begin
    dbms_output.put_line('n4 = ' || n4);
    dbms_output.put_line('n5 = ' || n5);
    dbms_output.put_line('n6 = ' || n6);
    dbms_output.put_line('n7 = ' || n7);
end;

-- 14 - 15

declare
    binary_float_number binary_float := 123456789.21414;
    binary_double_number binary_double := 123.12421541;
begin
    dbms_output.put_line('Binary float: ' || binary_float_number);
    dbms_output.put_line('Binary double: ' || binary_double_number);
end;

-- 16 - 17

declare
    number_E number(10, 3) := 2E+2;
    b1 boolean := true;
    b2 boolean := false;
begin
    dbms_output.put_line('E: ' || number_E);
    if b1 then
        dbms_output.put_line('b1: ' || 'true');
    end if;
    if not b1 then
        dbms_output.put_line('b1: ' || 'false');
    end if;
    if b2 then
        dbms_output.put_line('b2: ' || 'true');
    end if;
    if not b2 then
        dbms_output.put_line('b2: ' || 'false');
    end if;
end;

-- 18

declare
    const_number constant number(5) := 10;
    const_varchar constant varchar2(10) := 'kem';
    const_char constant char(10) := 'jokimi';
begin
    dbms_output.put_line('const_number: ' || const_number);
    dbms_output.put_line('const_number * 2: ' || const_number * 2);
    dbms_output.put_line('const_varchar: ' || const_varchar);
    dbms_output.put_line('const_char: ' || const_char);
end;

-- 19

declare
    VCHAR_CONST constant varchar2(20) := 'VCHAR_CONST';
    CHAR_CONST constant char(20) := 'CHAR_CONST';
    NUMBER_CONST constant number := 1;
    VCHAR_CONST2 VCHAR_CONST%type := 'VCHAR_CONST2';
    CHAR_CONST2 CHAR_CONST%type := 'CHAR_CONST2';
    NUMBER_CONST2 NUMBER_CONST%type := 2;
begin
    dbms_output.put_line(VCHAR_CONST2);
    dbms_output.put_line(CHAR_CONST2);
    dbms_output.put_line(NUMBER_CONST2);
end;

-- 20

declare
    AUDITORIUM_TYPE_ROW AUDITORIUM_TYPE%rowtype;
begin
    AUDITORIUM_TYPE_ROW.AUDITORIUM_TYPENAME := 'Lecture Hall';
    AUDITORIUM_TYPE_ROW.AUDITORIUM_TYPE := 'LEC';
    dbms_output.put_line(AUDITORIUM_TYPE_ROW.AUDITORIUM_TYPE);
    dbms_output.put_line(AUDITORIUM_TYPE_ROW.AUDITORIUM_TYPENAME);
end;

-- 21 - 22

declare
    v_num number := 1;
begin
    if v_num = 1 then
        dbms_output.put_line('v_num = 1');
    elsif v_num = 2 then
        dbms_output.put_line('v_num = 2');
    elsif v_num is null then
        dbms_output.put_line('v_num is null');
    else
        dbms_output.put_line('v_num = 3');
    end if;
end;

-- 23

declare
    v_num number := 1;
begin
    case v_num
        when 1 then
            dbms_output.put_line('v_num = 1');
        when 2 then
            dbms_output.put_line('v_num = 2');
        when 3 then
            dbms_output.put_line('v_num = 3');
        else
            dbms_output.put_line('v_num is null');
    end case;
end;

-- 24

declare
    v_num number := 1;
begin
    loop
        dbms_output.put_line(v_num);
        v_num := v_num + 1;
        exit when v_num > 10;
    end loop;
end;

-- 25

declare
    v_num number := 1;
begin
    while v_num <= 10
        loop
            dbms_output.put_line(v_num);
            v_num := v_num + 1;
        end loop;
end;

-- 26

declare
    v_num number := 1;
begin
    for i in 1..10 loop
        dbms_output.put_line(i);
    end loop;
end;