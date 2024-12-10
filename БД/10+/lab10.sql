
-- 1

alter table TEACHER add BIRTHDAY date;
alter table TEACHER add SALARY NUMBER(10, 3);
select * from TEACHER;
declare
    cursor AAA is select * from TEACHER for update;
    result TEACHER%rowtype;
begin
    open AAA;
    fetch AAA into result;
    while AAA%FOUND
        loop
            result.BIRTHDAY :=  to_date(
                trunc(dbms_random.value(to_number(to_char(date '1975-01-01', 'J')), to_number(to_char(date '2000-12-31', 'J')))), 'J'
            );
            update TEACHER set TEACHER.BIRTHDAY = result.BIRTHDAY, TEACHER.SALARY = dbms_random.value(999, 3000)
                where current of AAA;
            fetch AAA into result;
        end loop;
    close AAA;
end;

-- 2

select
    substr(TEACHER_NAME, 1, instr(TEACHER_NAME, ' '))  ||
    substr(substr(TEACHER_NAME, instr(TEACHER_NAME, ' ') + 1), 1, 1) || '. ' ||
    substr(substr(TEACHER_NAME, instr(TEACHER_NAME, ' ', 1, 2) + 1), 1, 1) || '.' as "Surname N. P."
from TEACHER;

-- 3

select TEACHER_NAME, BIRTHDAY from TEACHER where to_char(BIRTHDAY, 'DAY') = 'œŒÕ≈ƒ≈À‹Õ» ';

-- 4

create view BirthdayNextMonth as select TEACHER_NAME, BIRTHDAY from TEACHER
    where extract(MONTH from BIRTHDAY) = extract(MONTH from add_months(current_date, 1));
select * from BirthdayNextMonth;
drop view BirthdayNextMonth;

-- 5

create view TeacherBirthdaysCount as
    select extract(MONTH from BIRTHDAY) as BirthMonth, count(*) as TeachersCount
    from TEACHER
    group by extract(MONTH from BIRTHDAY) order by extract(MONTH from BIRTHDAY);
select * from TeacherBirthdaysCount;
drop view TeacherBirthdaysCount;

-- 6

declare
    cursor JUBILLE is select * from TEACHER;
    result TEACHER%rowtype;
    current_year NUMBER;
begin
    current_year := EXTRACT(YEAR FROM SYSDATE);
    for result in JUBILLE
    loop
        if mod(current_year - extract(YEAR from result.BIRTHDAY) + 1, 10) = 0 then
            dbms_output.put_line(result.TEACHER_NAME || ' ' || result.BIRTHDAY);
        end if;
    end loop;
end;

-- 7

select * from PULPIT;
select * from TEACHER;
declare
    cursor ACCOUNTANT IS
        select avg(SALARY) as AVG_SALARY_PULPIT, TEACHER.PULPIT, FACULTY
            from TEACHER join PULPIT P on P.PULPIT = TEACHER.PULPIT
            group by TEACHER.PULPIT, FACULTY order by FACULTY;
    result ACCOUNTANT%rowtype;
    fullSalaryAvg integer;
    countRow integer;
    countRowFaculty integer;
    avgSalaryByFaculty number(10,3);
    facultyType FACULTY.FACULTY%type;
begin
    open ACCOUNTANT;
    fullSalaryAvg := 0;
    countRow := 0;
    avgSalaryByFaculty := 0;
    countRowFaculty := 0;
        fetch ACCOUNTANT into result;
        facultyType := result.FACULTY;
        while ACCOUNTANT%found
            loop
                if facultyType = result.FACULTY then
                    countRowFaculty := countRowFaculty + 1;
                    avgSalaryByFaculty := avgSalaryByFaculty + result.AVG_SALARY_PULPIT;
                else
                    dbms_output.put_line('------------------------------');
                    dbms_output.put_line(facultyType ||' avg: ' || floor(avgSalaryByFaculty / countRowFaculty));
                    dbms_output.put_line('==============================');
                    avgSalaryByFaculty := result.AVG_SALARY_PULPIT;
                    countRowFaculty := 1;
                end if;
                    dbms_output.put_line(result.PULPIT ||'avg: ' || floor(result.AVG_SALARY_PULPIT));
                    fullSalaryAvg := fullSalaryAvg + floor(result.AVG_SALARY_PULPIT);
                    countRow := countRow + 1;
                facultyType := result.FACULTY;
                fetch ACCOUNTANT into result;
            end loop;
        dbms_output.put_line('fullAvg: ' || floor(fullSalaryAvg / countRow));
    close ACCOUNTANT;
exception
    when others then dbms_output.put_line(sqlerrm);
end;

-- 8

declare
    rec1 TEACHER%rowtype;
    type address is record (
        address1 varchar2(100),
        address2 varchar2(100),
        address3 varchar2(100)
    );
    type person is record (
        code TEACHER.TEACHER%type,
        name TEACHER.TEACHER_NAME%type,
        homeAddress address
    );
    rec2 person;
    rec3 person;
begin
    rec1.TEACHER := 'SMLV';
    rec1.TEACHER_NAME := 'Smelov Vladimir Vladislavovich';
    rec2.code := rec1.TEACHER;
    rec2.name := rec1.TEACHER_NAME;
    rec2.homeAddress.address1 := 'Belarus';
    rec2.homeAddress.address2 := 'Minsk';
    rec2.homeAddress.address3 := 'Belorusskaya 21';
    rec3 := rec2;
    dbms_output.put_line('Code: ' || rec3.code);
    dbms_output.put_line('Name: ' || rec3.name);
    dbms_output.put_line('Address 1: ' || rec3.homeAddress.address1);
    dbms_output.put_line('Address 2: ' || rec3.homeAddress.address2);
    dbms_output.put_line('Address 3: ' || rec3.homeAddress.address3);
exception
    when others then
        dbms_output.put_line('Error: ' || sqlcode || ' ' || sqlerrm);
end;
