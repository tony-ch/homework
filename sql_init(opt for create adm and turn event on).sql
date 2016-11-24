/*
delete from `user`;
delete from `orde`;
delete from `worker`;
delete from `company_account`;
delete from `bike`;
delete from `user_account`;
delete from `admin`;
delete from `personal_bike`;
delete from `maintain`;
delete from `purchase`;
*/
TRUNCATE TABLE `user_account`;
TRUNCATE TABLE `company_account`;
TRUNCATE TABLE `maintain`;
TRUNCATE TABLE `personal_bike`;
TRUNCATE TABLE `purchase`;
TRUNCATE TABLE `orde`;
/* foreign key constraint
TRUNCATE TABLE `bike`;
TRUNCATE TABLE `admin`;
TRUNCATE TABLE `user`;
TRUNCATE TABLE `worker`;
*/
DELETE FROM `admin`;
ALTER TABLE `admin` AUTO_INCREMENT = 1;
delete from `user`;
ALTER TABLE `user` AUTO_INCREMENT = 1;
delete from `worker`;
ALTER TABLE `worker` AUTO_INCREMENT = 1;
delete from `bike`;
ALTER TABLE `bike` AUTO_INCREMENT = 1;

show variables like 'event_scheduler';
set global event_scheduler='on';

drop event if exists DIL_SALARY;
create event DIL_SALARY
on schedule 
every 1 month
STARTS '2016-11-01 00:00:00'
on completion preserve
disable
do call DILIVER_SALARY();

alter event DIL_SALARY on completion preserve enable;
select * from  mysql.event;

select * from `user`;
select * from `orde`;
select * from `purchase`;
select * from `bike`;
select * from `user_account`;
select * from `admin`;
select * from `worker`;
select * from `company_account`;
select * from `maintain`;

insert into `admin` values('1','admin','admin','admin','12345678901');