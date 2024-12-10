
-- 1

declare
    procedure GetTeachers(pcode TEACHER.PULPIT%type) is
        cursor cGetTeachers is
            select TEACHER, TEACHER_NAME, PULPIT from TEACHER where PULPIT = pcode;
        mteacher TEACHER.TEACHER%type;
        mteachername TEACHER.TEACHER_NAME%type;
        mpulpit TEACHER.PULPIT%type;
    begin
        open cGetTeachers; 
        loop
            fetch cGetTeachers into mteacher, mteachername, mpulpit;
            exit when cGetTeachers%notfound;
            DBMS_OUTPUT.PUT_LINE(mteacher || ' ' || mteachername || ' ' || mpulpit);
        end loop;
        close cGetTeachers;
    end GetTeachers;
begin
    GetTeachers('IST');
end;

-- 2

declare
    function GetNumTeachers(pcode TEACHER.PULPIT%type) return number is
        resultnum number;
    begin
        select count(TEACHER) into resultnum from TEACHER where PULPIT = pcode;
        return resultnum;
    end GetNumTeachers;
begin
    DBMS_OUTPUT.PUT_LINE('Number of teachers: ' || GetNumTeachers('IST'));
end;

-- 3

declare
    procedure GetTeachersByFaculty(fcode FACULTY.FACULTY%type) is
        cursor cGetTeachersByFaculty is
            select TEACHER, TEACHER_NAME, p.PULPIT
                from TEACHER inner join PULPIT p on p.PULPIT = TEACHER.PULPIT where FACULTY = fcode;
        mteacher TEACHER.TEACHER%type;
        mteachername TEACHER.TEACHER_NAME%type;
        mpulpit TEACHER.PULPIT%type;
    begin
        open cGetTeachersByFaculty;
        loop
            fetch cGetTeachersByFaculty into mteacher, mteachername, mpulpit;
            exit when cGetTeachersByFaculty%notfound;
            DBMS_OUTPUT.PUT_LINE(mteacher || ' ' || mteachername || ' ' || mpulpit);
        end loop;
        close cGetTeachersByFaculty;
    end GetTeachersByFaculty;
begin
    GetTeachersByFaculty('OSTF');
end;

-- 4

declare
    procedure GetSubjects(pcode SUBJECT.PULPIT%type) is
        cursor cGetSubjects is
            select * from SUBJECT where PULPIT = pcode;
        msubject SUBJECT.SUBJECT%type;
        msubjectname SUBJECT.SUBJECT_NAME%type;
        mpulpit SUBJECT.PULPIT%type;
    begin
        open cGetSubjects;
        loop
            fetch cGetSubjects into msubject, msubjectname, mpulpit;
            exit when cGetSubjects%notfound;
            DBMS_OUTPUT.PUT_LINE(msubject || ' ' || msubjectname || ' ' || mpulpit);
        end loop;
        close cGetSubjects;
    end GetSubjects;
begin
    GetSubjects('IST');
end;

-- 5

declare
    function FGetNumTeachers(fcode FACULTY.FACULTY%type) return number is
        resultnum number;
    begin
        select count(TEACHER) INTO resultnum from TEACHER
            where TEACHER.PULPIT in (select PULPIT from PULPIT where FACULTY = fcode);
        return resultnum;
    end FGetNumTeachers;
begin
    DBMS_OUTPUT.PUT_LINE('Number of teachers in faculty: ' || FGetNumTeachers('OSTF'));
end;

-- 6

declare
    function PGetNumSubjects(pcode SUBJECT.PULPIT%type) return number is
        resultnum number;
    begin
        select count(SUBJECT) into resultnum from SUBJECT where PULPIT = pcode;
        return resultnum;
    end PGetNumSubjects;
begin
    DBMS_OUTPUT.PUT_LINE('Number of subjects: ' || PGetNumSubjects('IST'));
end;

-- 7

create or replace package TEACHERS as
    fcode FACULTY.FACULTY%type;
    pcode SUBJECT.PULPIT%type;
    procedure PGetTeachers (fcode FACULTY.FACULTY%type);
    procedure PGetSubjects (pcode SUBJECT.PULPIT%type);
    function PGetNumTeachers (fcode FACULTY.FACULTY%type) return number;
    function PGetNumSubjects (pcode SUBJECT.PULPIT%type) return number;
end TEACHERS;
create or replace package body TEACHERS is
    procedure PGetTeachers (fcode FACULTY.FACULTY%type) is
        cursor GetTeachersByFaculty is
            select TEACHER, TEACHER_NAME, P.PULPIT
                from TEACHER inner join PULPIT P on P.PULPIT = TEACHER.PULPIT where FACULTY = fcode;
        mTeacher TEACHER.TEACHER%type;
        mTeacherName TEACHER.TEACHER_NAME%type;
        mPulpit TEACHER.PULPIT%type;
    begin
        open GetTeachersByFaculty;
        fetch GetTeachersByFaculty into mTeacher, mTeacherName, mPulpit;
        while (GetTeachersByFaculty%found)
            loop
                dbms_output.put_line(mTeacher || ' ' || mTeacherName || ' ' || mPulpit);
                fetch GetTeachersByFaculty into mTeacher, mTeacherName, mPulpit;
            end loop;
        close GetTeachersByFaculty;
    end PGetTeachers;
    procedure PGetSubjects (pcode SUBJECT.PULPIT%type) is
        cursor GetSubjects is
            select * from SUBJECT where PULPIT = pcode;
        mSubject SUBJECT.SUBJECT%type;
        mSubjectName SUBJECT.SUBJECT_NAME%type;
        mPulpit SUBJECT.PULPIT%type;
    begin
        open GetSubjects;
        fetch GetSubjects into mSubject, mSubjectName, mPulpit;
        while (GetSubjects%found)
            loop
                dbms_output.put_line(mSubject || ' ' || mSubjectName || ' ' || mPulpit);
                fetch GetSubjects into mSubject, mSubjectName, mPulpit;
            end loop;
        close GetSubjects;
    end PGetSubjects;
    function PGetNumTeachers (fcode FACULTY.FACULTY%type) return number is
        resultNum number;
    begin
        select count(TEACHER) into resultNum from TEACHER
            where TEACHER.PULPIT in (select PULPIT from PULPIT where FACULTY = fcode);
        return resultNum;
    end PGetNumTeachers;
    function PGetNumSubjects (pcode SUBJECT.PULPIT%type) return number is
        resultNum number;
    begin
        select count(SUBJECT) into resultNum from SUBJECT where PULPIT = pcode;
        return resultNum;
    end PGetNumSubjects;
begin
    null;
end TEACHERS;

-- 8

begin
    dbms_output.put_line('Number of teachers in the faculty: ' || TEACHERS.PGetNumTeachers('OSTF'));
    dbms_output.put_line('Number of subjects in the pulpit: ' || TEACHERS.PGetNumSubjects('IST'));
    dbms_output.put_line('-----------------------------------------');
    TEACHERS.PGetTeachers('OSTF');
    dbms_output.put_line('-----------------------------------------');
    TEACHERS.PGetSubjects('IST');
end;