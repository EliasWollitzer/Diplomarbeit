create table Persons(
Pid INT NOT NULL auto_increment,
Did INT NOT NULL ,
primary key (Pid,Did), 
firstName VARCHAR(20),
lastName VARCHAR(20)
);

create table Borrowed(
Entid INT NOT NULL auto_increment,
Pid INT NOT NULL,
Rid INT NOT NULL,
primary key (Entid,Pid,Rid),  
description VARCHAR(30),
datefrom DATETIME,
dateto DATETIME
);
 create table Resources(
Rid INT NOT NULL auto_increment,
primary key (Rid),
resource VARCHAR(30)
);

create table Department(
Did INT NOT NULL auto_increment,
primary key (Did),
section VARCHAR(30)
);

insert into Persons
 (Did,firstName,lastName)
 values
 ('1','Elias','Wollitzer'),
 ('1','Johannes','Aigner'),
 ('2','Franz','Mustermann'),
 ('2','Frida','Musterfrau');

 
  insert into Borrowed
 (Pid,Rid,description,datefrom,dateto)
 values
 ('3','1','Besuch','2018-04-12 13:45:00','2018-04-12 15:00:00'),
 ('3','3','Besprechung','2018-04-14 11:45:00','2018-04-14 13:45:00'),
('1','2','Kundenbesuch','2018-04-22 10:45:00','2018-04-24 16:45:00');

insert into Resources
 (resource)
 values
 ('Raum 1'),
 ('Firmenauto 1'),
 ('Raum 2');

 insert into Department
 (section)
 values
 ('Hardware'),
 ('Software'),
 ('Marketing');

