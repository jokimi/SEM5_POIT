create table AUDITORIUM_TYPE (
    AUDITORIUM_TYPE char(20) constraint AUDITORIUM_TYPE_PK primary key,
    AUDITORIUM_TYPENAME varchar2(60) constraint AUDITORIUM_TYPENAME_NOT_NULL not null
);
insert into AUDITORIUM_TYPE (AUDITORIUM_TYPE, AUDITORIUM_TYPENAME)
    values ('LEC', 'Lecture Hall');
insert into AUDITORIUM_TYPE (AUDITORIUM_TYPE, AUDITORIUM_TYPENAME)
    values ('LAB-C', 'Computer Class');
insert into AUDITORIUM_TYPE (AUDITORIUM_TYPE, AUDITORIUM_TYPENAME)
    values ('LEC-C', 'Lecture Hall with PCs');
insert into AUDITORIUM_TYPE (AUDITORIUM_TYPE, AUDITORIUM_TYPENAME)
    values ('LAB-CH', 'Chemical Laboratory');
insert into AUDITORIUM_TYPE (AUDITORIUM_TYPE, AUDITORIUM_TYPENAME)
    values ('LAB-SC', 'Special Computer Class');
select * from AUDITORIUM_TYPE;

create table AUDITORIUM (
    AUDITORIUM char(20) primary key,
    AUDITORIUM_NAME varchar2(200),
    AUDITORIUM_CAPACITY number(4),
    AUDITORIUM_TYPE char(20) not null
    references AUDITORIUM_TYPE(AUDITORIUM_TYPE)
);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
    values ('206-1', '206-1', 'LAB-C', 15);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY)
    values ('301-1', '301-1', 'LAB-C', 15);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
    values ('236-1', '236-1', 'LEC', 60);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
    values ('313-1', '313-1', 'LEC', 60);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
    values ('324-1', '324-1', 'LEC', 50);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
    values ('413-1', '413-1', 'LAB-C', 15);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
    values ('423-1', '423-1', 'LAB-C', 90);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
    values ('408-2', '408-2', 'LEC', 90);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
    values ('103-4', '103-4', 'LEC', 90);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
    values ('105-4', '105-4', 'LEC', 90);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
    values ('107-4', '107-4', 'LEC', 90);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
    values ('110-4', '110-4', 'LEC', 30);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
    values ('111-4', '111-4', 'LEC', 30);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
    values ('114-4', '114-4', 'LEC-C', 90 );
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
    values ('132-4', '132-4', 'LEC', 90);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
    values ('137-4', '137-4', 'LEC', 90);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
    values ('229-4', '229-4', 'LEC', 90);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
    values ('304-4', '304-4','LAB-C', 25);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
    values ('314-4', '314-4', 'LEC', 90);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
    values ('320-4', '320-4', 'LEC', 90);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
    values ('429-4', '429-4', 'LEC', 90);
select * from AUDITORIUM;

create table FACULTY (
    FACULTY char(20) not null,
    FACULTY_NAME varchar2(200),
    constraint PK_FACULTY primary key(FACULTY)
);
insert into FACULTY (FACULTY, FACULTY_NAME )
    values ('FPTMC', 'Faculty of Print Technology and Media Communications');
insert into FACULTY (FACULTY, FACULTY_NAME )
    values ('CTEF', 'Chemical Technology and Engineering Faculty');
insert into FACULTY (FACULTY, FACULTY_NAME )
    values ('FF', 'Forestry Faculty');
insert into FACULTY (FACULTY, FACULTY_NAME )
    values ('EEF', 'Economic Engineering Faculty');
insert into FACULTY (FACULTY, FACULTY_NAME )
    values ('FFEMSD', 'Faculty of Forest Engineering, Materials Science and Design');
insert into FACULTY (FACULTY, FACULTY_NAME )
    values ('OSTF', 'Organic Substances Technology Faculty');
select * from FACULTY;

create table PULPIT (
    PULPIT char(20) not null,
    PULPIT_NAME varchar2(200),
    FACULTY char(20) not null,
    constraint FK_PULPIT_FACULTY foreign key(FACULTY) references FACULTY(FACULTY),
    constraint PK_PULPIT primary key(PULPIT)
);
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
    values ('IST', 'Information Systems and Technologies', 'FPTMC');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
    values ('PEDPS', 'Printing Equipment and Data Processing Systems', 'FPTMC');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
    values ('SC', 'Silviculture', 'FF');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
    values ('TNMH', 'Tourism, Nature Management and Hunting', 'FF');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
    values ('FI', 'Forest Inventory', 'FF');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
    values ('FPWS', 'Forest Protection and Wood Science', 'FF');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
    values ('LDA', 'Landscape Design and Architecture', 'FF');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
    values ('LMFRTPT', 'Logging Machinery, Forest Roads and Timber Production Technology', 'FFEMSD');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
    values ('WP', 'Woodworking Production', 'FFEMSD');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
    values ('OC', 'Organic Chemistry', 'OSTF');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
    values ('OGPP', 'Oil and Gas Processing and Petrochemistry', 'OSTF');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
    values ('IMTGCT', 'Inorganic Materials Technology and General Chemical Technology', 'CTEF');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
    values ('CTEPEEM', 'Chemistry, Technology of Electrochemical Production and Electronic Engineering Materials', 'CTEF');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
    values ('EM', 'Economics and Marketing', 'EEF');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
    values ('MBTSD', 'Management, Business Technology and Sustainable Development', 'EEF');
select * from PULPIT;

create table TEACHER (
    TEACHER char(20) not null,
    TEACHER_NAME varchar2(200),
    PULPIT char(20) not null,
    constraint PK_TEACHER primary key(TEACHER),
    constraint FK_TEACHER_PULPIT foreign key(PULPIT) references PULPIT(PULPIT)
);
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
    values ('SMLV', 'Smelov Vladimir Vladislavovich', 'IST');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
    values ('AKNVCH', 'Akunovich Stanislav Ivanovich', 'IST');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
    values ('KLSNKV', 'Kolesnikov Leonid Valerievich', 'IST');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
    values ('GRMN', 'German Oleg Vitoldovich', 'IST');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
    values ('LSHCHNK', 'Lashchenko Anatoliy Pavlovich', 'IST');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
    values ('BRKVCH', 'Brakovich Andrey Igorievich', 'IST');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
    values ('DTK', 'Dyatko Aleksandr Arkadievich', 'IST');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
    values ('KBL', 'Kobaylo Aleksandr Serafimovich', 'IST');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
    values ('URBNVCH', 'Urbanovich Pavel Pavlovich', 'IST');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
    values ('PMNNK', 'Romanenko Dmitriy Mikhailovich', 'IST');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
    values ('PSTVLV', 'Pustovalova Nataliya Nikolayevna', 'IST');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
    values ('?', 'Unknown', 'IST');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
    values ('GRN', 'Gurin Nikolay Ivanovich', 'IST');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
    values ('ZHLK', 'Zhilyak Nadezhda Aleksandrovna', 'IST');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
    values ('BRTSHVCH', 'Bartashevich Svyatoslav Aleksandrovich', 'PEDPS');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
    values ('YUDNKV', 'Yudenkov Viktor Stepanovich', 'PEDPS');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
    values ('BRNVSK', 'Baranovskiy Stanislav Ivanovich', 'EM');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
    values ('NVRV', 'Neverov Aleksandr Vasilievich', 'MBTSD');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
    values ('RVKCH', 'Rovkach Andrey Ivanovich', 'TNMH');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
    values ('DMDNK', 'Demidenko Marina Nikolayevna', 'LDA');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
    values ('MSHKVSK', 'Mashkovskiy Vladimir Petrovich', 'FI');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
    values ('LBKH', 'Labokha Konstantin Valentinovich', 'SC');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
    values ('ZVGNTSV', 'Zvyagintsev Vyacheslav Borisovich', 'FPWS');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
    values ('BZBRDV', 'Bezborodov Vladimir Stepanovich', 'OC');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
    values ('PRKPCHK', 'Prokopchuk Nikolay Romanovich', 'OGPP');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
    values ('NSKVTS', 'Naskovets Mikhail Trofimovich', 'LMFRTPT');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
    values ('MKHV', 'Mokhov Sergey Petrovich', 'WP');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
    values ('YSHCHNK', 'Yeshchenko Lyudmila Semyonovna', 'IMTGCT');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
    values ('ZHRCK', 'Zharskiy Ivan Mikhailovich', 'CTEPEEM');
select * from TEACHER;

create table SUBJECT (
    SUBJECT char(20) not null,
    SUBJECT_NAME varchar2(200) not null,
    PULPIT char(20) not null,
    constraint PK_SUBJECT primary key(SUBJECT),
    constraint FK_SUBJECT_PULPIT foreign key(PULPIT) references PULPIT(PULPIT)
);
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
    values ('DMS', 'Database management systems', 'IST');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
    values ('DB', 'Databases', 'IST');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
    values ('IT', 'Information technologies', 'IST');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
    values ('FAP', 'Fundamentals of Algorithmization and Programming', 'IST');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
    values ('KRCS', 'Knowledge representation in computer systems', 'IST');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
    values ('NAP', 'Network Application Programming', 'IST');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
    values ('MIPS', 'Modeling of information processing systems', 'IST');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
    values ('DIS', 'Design of information systems', 'IST');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
    values ('CG', 'Computer geometry', 'IST');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
    values ('PMAMFL', 'Printing machines, automatic machines and flow lines', 'PEDPS');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
    values ('CMS', 'Computer multimedia systems', 'IST');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
    values ('OPP', 'Organization of printing production', 'PEDPS');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
    values ('DM', 'Discrete Mathematics', 'IST');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
    values ('MP', 'Mathematical programming', 'IST');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
    values ('LFC', 'Logical foundations of computers', 'IST');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
    values ('OOP', 'Object-oriented programming', 'IST');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
    values ('ENM', 'Economics of nature management', 'MBTSD');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
    values ('ET', 'Economic theory', 'EM');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
    values ('BFABBH', 'Biology of forest animals and birds with the basics of hunting', 'TNMH');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
    values ('FGPMFPM', 'Fundamentals of garden and park management and forest park management', 'LDA');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
    values ('EG', 'Engineering geodesy', 'SC');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
    values ('FR', 'Forestry', 'FPWS');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
    values ('OC', 'Organic Chemistry', 'OC');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
    values ('TRP', 'Technology of rubber products', 'OGPP');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
    values ('WTT', 'Water transport of timber', 'LMFRTPT');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
    values ('TEL', 'Technology and equipment for logging', 'WP');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
    values ('MET', 'Mineral enrichment technology', 'IMTGCT');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT )
    values ('AE', 'Applied Electrochemistry', 'CTEPEEM');
select * from SUBJECT;

select table_name from dba_tables where owner = 'SYS';

drop table AUDITORIUM_TYPE;
drop table AUDITORIUM;
drop table FACULTY;
drop table PULPIT;
drop table TEACHER;
drop table SUBJECT;