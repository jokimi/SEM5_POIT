alter session set nls_date_format='dd-mm-yyyy hh24:mi:ss';
alter system set JOB_QUEUE_PROCESSES = 5;
SELECT * FROM USER_SYS_PRIVS WHERE PRIVILEGE = 'CREATE JOB';
grant create job to KEM;
select count(*) from dba_objects_ae where Object_Type = 'PACKAGE';

-- 1

drop table LAB14;
drop table LAB14_TO;
drop table JOB_STATUS;

create table LAB14 (
    a number,
    b number
);
create table LAB14_TO (
    a number,
    b number
);
create table JOB_STATUS (
    status nvarchar2(50),
    error_message nvarchar2(500),
    datetime date default sysdate
);

insert into LAB14 values (1, 6);
insert into LAB14 values (8, 2);
insert into LAB14 values (9, 68);
insert into LAB14 values (41, 12);
insert into LAB14 values (3, 77);
insert into LAB14 values (122, 4);
insert into LAB14 values (661, 44);
insert into LAB14 values (341, 61);
insert into LAB14 values (1, 0);
commit;
select * from LAB14;

-- 2

drop procedure JobProc;
create or replace procedure JobProc is
    cursor jobcur is
    select * from LAB14;
    err_message varchar2(500);
begin
    for m in jobcur
        loop
            insert into LAB14_TO values (m.a, m.b);
            delete from LAB14 where a = m.a;
        end loop;
    insert into JOB_STATUS (status, datetime) values ('SUCCESS', sysdate);
    commit;
exception
    when others then
        err_message := sqlerrm;
        insert into JOB_STATUS (status, error_message) values ('FAILURE', err_message);
        commit;
end JobProc;

declare
    job_number user_jobs.job%type;
begin
    dbms_job.submit(job_number, 'BEGIN JobProc(); END;', to_date('17-12-2024 10:29:00', 'dd-mm-yyyy hh24:mi:ss'), 'sysdate + 7');
    commit;
    dbms_output.put_line(job_number);
end;

select * from LAB14;
select * from LAB14_TO;
select * from JOB_STATUS;

delete from LAB14_TO;
delete from JOB_STATUS;

-- 3

select job, what, last_date, last_sec, next_date, next_sec, broken from user_jobs;

-- 4

begin
  dbms_job.run(59);
end;

SELECT object_name, status
    FROM user_objects
    WHERE object_type = 'PROCEDURE'
    AND object_name = 'JOBPROC';

begin
  dbms_job.remove(50);
end;

select * from JOB_STATUS;

-- 5

begin
    dbms_scheduler.create_schedule (
        schedule_name => 'SCH_1',
        start_date => sysdate,
        repeat_interval => 'FREQ = WEEKLY',
        comments => 'SCH_1 WEEKLY starts now'
    );
end;

select * from user_scheduler_schedules;

begin
    dbms_scheduler.create_program(
        program_name => 'PROGRAM_1',
        program_type => 'STORED_PROCEDURE',
        program_action => 'JobProc',
        number_of_arguments => 0,
        enabled => true,
        comments => 'PROGRAM_1'
    );
end;

select * from user_scheduler_programs;

begin
    dbms_scheduler.create_job (
        job_name => 'JOB_1',
        program_name => 'PROGRAM_1',
        schedule_name => 'SCH_1',
        enabled => true
    );
end;

select * from user_scheduler_jobs;

begin
    DBMS_SCHEDULER.DISABLE('JOB_1');
end;

begin
    DBMS_SCHEDULER.RUN_JOB('JOB_1');
end;

begin
    DBMS_SCHEDULER.DROP_JOB( JOB_NAME => 'JOB_1');
end;