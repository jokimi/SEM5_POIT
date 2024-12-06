alter session set container = ORCLPDB1;

-- 1

declare
    faculty_res FACULTY%rowtype;
begin
    select * into faculty_res from FACULTY where FACULTY = 'OSTF';
    dbms_output.put_line( faculty_res.FACULTY || faculty_res.FACULTY_NAME);
end;

-- 2

declare
    faculty_res FACULTY%rowtype;
begin
    select * into faculty_res from FACULTY;
    dbms_output.put_line(faculty_res.FACULTY || faculty_res.FACULTY_NAME);
exception
    when others then
        dbms_output.put_line('Error: ' || sqlerrm || ', code: ' || sqlcode);
end;

-- 3

declare
    pulpit_res PULPIT%rowtype;
begin
    select * into pulpit_res from PULPIT;
    dbms_output.put_line(pulpit_res.PULPIT_NAME);
exception
    when TOO_MANY_ROWS then
        dbms_output.put_line('Error: ' || sqlerrm || ', code: ' || sqlcode);
end;

-- 4

declare
    b1 boolean;
    b2 boolean;
    b3 boolean;
    n integer;
    subject_res SUBJECT%rowtype;
begin
    select * into subject_res from SUBJECT where SUBJECT = 'DMS';
    b1 := sql%found;
    b2 := sql%isopen;
    b3 := sql%notfound;
    n := sql%rowcount;
    if b1 then
        dbms_output.put_line('b1 = true');
    else
        dbms_output.put_line('b1 = false');
    end if;
    if b2 then
        dbms_output.put_line('b2 = true');
    else
        dbms_output.put_line('b2 = false');
    end if;
    if b3 then
        dbms_output.put_line('b3 = true');
    else
        dbms_output.put_line('b3 = false');
    end if;
    dbms_output.put_line(n);
    dbms_output.put_line(subject_res.SUBJECT);
exception
    when NO_DATA_FOUND then
        dbms_output.put_line('Error: ' || sqlerrm || ', code: ' || sqlcode);
end;

-- 5

declare
    v_capacity AUDITORIUM.AUDITORIUM_CAPACITY%type;
begin
    update AUDITORIUM
        set AUDITORIUM_NAME = 'Auditorium 105-4' where AUDITORIUM = '105-4'
        returning AUDITORIUM_CAPACITY into v_capacity;
    if v_capacity <> 100 then
        dbms_output.put_line('Table updated.');
        commit;
    else
        dbms_output.put_line('Table not updated.');
        rollback;
    end if;
end;

-- 6

declare
begin
    update AUDITORIUM
        set AUDITORIUM = 'Too long auditorium name (maximum length - 20 symbols)'
        where AUDITORIUM = '105-4';
exception
    when others then
        dbms_output.put_line('Error: ' || sqlerrm || ', code: ' || sqlcode);
    rollback;
end;

-- 7

declare
    v_aud AUDITORIUM.AUDITORIUM%type;
    v_aud_name AUDITORIUM.AUDITORIUM_NAME%type;
    v_aud_cap AUDITORIUM.AUDITORIUM_CAPACITY%type;
begin
    insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_CAPACITY, AUDITORIUM_TYPE)
        values ('401-4', '401-4', 100, 'LEC')
        returning AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_CAPACITY into v_aud, v_aud_name, v_aud_cap;
    dbms_output.put_line('AUDITORIUM: ' || v_aud || ', AUDITORIUM_NAME: ' || v_aud_name || ', AUDITORIUM_CAPACITY: ' || v_aud_cap);
    commit;
    insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_CAPACITY, AUDITORIUM_TYPE)
        values ('301-4', '301-4', 100, 'LEC')
        returning AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_CAPACITY into v_aud, v_aud_name, v_aud_cap;
    dbms_output.put_line('AUDITORIUM: ' || v_aud || ', AUDITORIUM_NAME: ' || v_aud_name || ', AUDITORIUM_CAPACITY: ' || v_aud_cap);
    rollback;
exception
    when others then
        dbms_output.put_line('Error: ' || sqlerrm || ', code: ' || sqlcode);
    rollback;
end;
select * from AUDITORIUM;

-- 8

declare
begin
    insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_CAPACITY, AUDITORIUM_TYPE)
        values ('408-2', '408-2', 90, 'LEC');
exception
    when others then
        dbms_output.put_line('Error: ' || sqlerrm || ', code: ' || sqlcode);
    rollback;
end;

-- 9

declare
    v_aud AUDITORIUM.AUDITORIUM%type;
    v_aud_name AUDITORIUM.AUDITORIUM_NAME%type;
    v_aud_cap AUDITORIUM.AUDITORIUM_CAPACITY%type;
begin
    delete from AUDITORIUM where AUDITORIUM = '401-4'
        returning AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_CAPACITY into v_aud, v_aud_name, v_aud_cap;
    dbms_output.put_line('AUDITORIUM: ' || v_aud || ', AUDITORIUM_NAME: ' || v_aud_name || ', AUDITORIUM_CAPACITY: ' || v_aud_cap);
    commit;
    delete from AUDITORIUM where AUDITORIUM = '429-4'
        returning AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_CAPACITY into v_aud, v_aud_name, v_aud_cap;
    dbms_output.put_line('AUDITORIUM: ' || v_aud || ', AUDITORIUM_NAME: ' || v_aud_name || ', AUDITORIUM_CAPACITY: ' || v_aud_cap);
    rollback;
end;
select * from AUDITORIUM;

-- 10

insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_CAPACITY, AUDITORIUM_TYPE)
    values ('102-1', '102-1', 15, 'LAB-C');
declare
begin
    delete from AUDITORIUM_TYPE where AUDITORIUM_TYPE = 'LAB-C';
exception
    when others then
        dbms_output.put_line('Error: ' || sqlerrm || ', code: ' || sqlcode);
    rollback;
end;
select * from AUDITORIUM;

-- 11

declare
    v_teacher TEACHER.TEACHER%type;
    v_teacher_name TEACHER.TEACHER_NAME%type;
    v_pulpit TEACHER.PULPIT%type;
    cursor c_TEACHER is
        select TEACHER, TEACHER_NAME, PULPIT from TEACHER;
begin
    for i in c_teacher
        LOOP
            v_teacher := i.TEACHER;
            v_teacher_name := i.TEACHER_NAME;
            v_pulpit := i.PULPIT;
            dbms_output.put_line('TEACHER: ' || v_teacher || ', TEACHER_NAME: ' || v_teacher_name || ', PULPIT: ' || v_pulpit);
        end loop;
end;

-- 12

declare
    v_subj SUBJECT%rowtype;
    cursor cur is
        select * from SUBJECT;
begin
    open cur;
    while true
        loop
            fetch cur into v_subj;
            exit when cur%notfound;
            dbms_output.put_line('SUBJECT: ' || v_subj.SUBJECT || ', SUBJECT_NAME: ' || v_subj.SUBJECT_NAME || ', PULPIT: ' || v_subj.PULPIT);
        end loop;
end;

-- 13

declare
    v_pulpit PULPIT.PULPIT%type;
    v_pulpit_name PULPIT.PULPIT_NAME%type;
    v_faculty PULPIT.FACULTY%type;
    v_teacher TEACHER.TEACHER%type;
    v_teacher_name TEACHER.TEACHER_NAME%type;
    v_pulpit_teacher PULPIT.PULPIT%type;
    cursor c_PULPIT_TEACHER is
        select PULPIT.PULPIT, PULPIT.PULPIT_NAME, PULPIT.FACULTY, TEACHER.TEACHER, TEACHER.TEACHER_NAME
            from PULPIT join TEACHER on PULPIT.PULPIT = TEACHER.PULPIT;
begin
    for i in c_pulpit_teacher
        loop
            v_pulpit := i.PULPIT;
            v_pulpit_name := i.PULPIT_NAME;
            v_faculty := i.FACULTY;
            v_teacher := i.TEACHER;
            v_teacher_name := i.TEACHER_NAME;
            dbms_output.put_line('PULPIT: ' || v_pulpit || ', PULPIT_NAME: ' || v_pulpit_name || ', FACULTY: ' || v_faculty ||
                ', TEACHER: ' || v_teacher || ', TEACHER_NAME: ' || v_teacher_name);
        end loop;
end;

-- 14

declare
    cursor aud_cur (min_cap AUDITORIUM.AUDITORIUM_CAPACITY%type, max_cap AUDITORIUM.AUDITORIUM_CAPACITY%type) is
        select AUDITORIUM_NAME, AUDITORIUM_CAPACITY from AUDITORIUM where AUDITORIUM_CAPACITY >= min_cap AND AUDITORIUM_CAPACITY <= max_cap;
    record aud_cur%rowtype;
    n int;
begin
    open aud_cur (0, 20);
    loop
        fetch aud_cur into record;
        exit when aud_cur%notfound;
        dbms_output.put_line(' ' || record.AUDITORIUM_NAME || ' ' || record.AUDITORIUM_CAPACITY);
    end loop;
    close aud_cur;
    dbms_output.put_line('------------------------------------------------------------------------');
    open aud_cur (21, 30);
    fetch aud_cur into record;
    while aud_cur%found
        loop
            dbms_output.put_line(' ' || record.AUDITORIUM_NAME || ' ' || record.AUDITORIUM_CAPACITY);
            fetch aud_cur into record;
        end loop;
    close aud_cur;
    dbms_output.put_line('------------------------------------------------------------------------');
    open aud_cur (31, 60);
    fetch aud_cur into record;
    while aud_cur%found
        loop
            dbms_output.put_line(' ' || record.AUDITORIUM_NAME || ' ' || record.AUDITORIUM_CAPACITY);
            fetch aud_cur into record;
        end loop;
    close aud_cur;
    dbms_output.put_line('------------------------------------------------------------------------');
    open aud_cur (61, 80);
    fetch aud_cur into record;
    while aud_cur%found
        loop
            dbms_output.put_line(' ' || record.AUDITORIUM_NAME || ' ' || record.AUDITORIUM_CAPACITY);
            fetch aud_cur into record;
        end loop;
    close aud_cur;
    dbms_output.put_line('------------------------------------------------------------------------');
    select max(AUDITORIUM_CAPACITY) into n from AUDITORIUM;
        for record in aud_cur (81, n)
            loop
                if record.AUDITORIUM_NAME like '%??%' then
                    continue;
                end if;
                dbms_output.put_line(' ' || record.AUDITORIUM_NAME || ' ' || record.AUDITORIUM_CAPACITY);
            end loop;
exception
    when others then
        dbms_output.put_line('Error: ' || sqlerrm || ', code: ' || sqlcode);
end;

-- 15

declare
    v_aud AUDITORIUM.AUDITORIUM%type;
    v_aud_name AUDITORIUM.AUDITORIUM_NAME%type;
    v_aud_cap AUDITORIUM.AUDITORIUM_CAPACITY%type;
    c_aud sys_refcursor;
begin
    open c_aud for
        select AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_CAPACITY from AUDITORIUM
            where AUDITORIUM_TYPE = 'LEC';
    loop
        fetch c_aud into v_aud, v_aud_name, v_aud_cap;
        exit when c_aud%notfound;
        dbms_output.put_line('AUDITORIUM: ' || v_aud || ', AUDITORIUM_NAME: ' || v_aud_name || ', AUDITORIUM_CAPACITY: ' || v_aud_cap);
    end loop;
    close c_aud;
end;

-- 16

declare
    cursor cur_in_cur (n integer, capacity AUDITORIUM.AUDITORIUM_CAPACITY%type) is
        select AUDITORIUM_NAME, AUDITORIUM_CAPACITY from (
            select * from AUDITORIUM where AUDITORIUM_CAPACITY > capacity order by AUDITORIUM_CAPACITY desc
        ) where rownum < n;
        result cur_in_cur%rowtype;
begin
    open cur_in_cur (10, 60);
        fetch cur_in_cur into result;
            while cur_in_cur%found
                loop
                    dbms_output.put_line(' ' || result.AUDITORIUM_NAME || ' ' || result.AUDITORIUM_CAPACITY);
                    fetch cur_in_cur into result;
                end loop;
    close cur_in_cur;
exception
    when others then
        dbms_output.put_line('Error: ' || sqlerrm || ', code: ' || sqlcode);
end;

-- 17

select * from auditorium order by auditorium;

DECLARE
    CURSOR c_aud (min_cap AUDITORIUM.AUDITORIUM_CAPACITY%TYPE, max_cap AUDITORIUM.AUDITORIUM_CAPACITY%TYPE) IS
        SELECT * FROM AUDITORIUM WHERE AUDITORIUM_CAPACITY >= min_cap AND AUDITORIUM_CAPACITY <= max_cap FOR UPDATE;
BEGIN
    FOR i IN c_aud(40, 80) LOOP
        UPDATE AUDITORIUM 
        SET AUDITORIUM_CAPACITY = AUDITORIUM_CAPACITY * 0.9 
        WHERE CURRENT OF c_aud; 
    END LOOP;
    COMMIT;
EXCEPTION
    WHEN OTHERS THEN
        ROLLBACK;
        DBMS_OUTPUT.PUT_LINE('Error: ' || SQLERRM || ', code: ' || SQLCODE);
END;

select * from auditorium order by auditorium;
begin
    rollback;
end;

-- 18

declare
    v_aud AUDITORIUM.AUDITORIUM%type;
    cursor c_aud (count int) is
        select AUDITORIUM from AUDITORIUM
            where AUDITORIUM_CAPACITY >= 0 and AUDITORIUM_CAPACITY <= count for update;
begin
    open c_aud (20);
    loop
        fetch c_aud into v_aud;
        exit when c_aud%notfound;
        delete from AUDITORIUM where current of c_aud;
    end loop;
    close c_aud;
end;
select * from AUDITORIUM;
rollback;

-- 19

declare
    cursor c_aud is
        select rowid from AUDITORIUM
            where AUDITORIUM_CAPACITY >= 0 and AUDITORIUM_CAPACITY <= 20 for update;
begin
    for i in c_aud
        loop
            update AUDITORIUM set AUDITORIUM_CAPACITY = AUDITORIUM_CAPACITY * 0.9 where rowid = i.rowid;
            delete from AUDITORIUM where rowid = i.rowid;
        end loop;
end;
select * from AUDITORIUM;
rollback;

-- 20

declare
    v_teacher TEACHER.TEACHER%type;
    v_teacher_name TEACHER.TEACHER_NAME%type;
    v_pulpit PULPIT.PULPIT%type;
    cursor c_teacher is
        select TEACHER, TEACHER_NAME, PULPIT from TEACHER;
begin
    open c_teacher;
    loop
        fetch c_teacher into v_teacher, v_teacher_name, v_pulpit;
        exit when c_teacher%notfound;
        dbms_output.put_line('PULPIT: ' || v_pulpit || ', TEACHER: ' || v_teacher || ', TEACHER_NAME: ' || v_teacher_name);
        if mod(c_teacher%rowcount, 3) = 0 then
            dbms_output.put_line('------------------------------------------------------------------------');
        end if;
    end loop;
    close c_teacher;
end;