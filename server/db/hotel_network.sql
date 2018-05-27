DROP TABLE IF EXISTS HotelType CASCADE;
DROP TABLE IF EXISTS EmployeePosition CASCADE;
DROP TABLE IF EXISTS RoomType CASCADE;
DROP TABLE IF EXISTS Hotel CASCADE;
DROP TABLE IF EXISTS Employee CASCADE;
DROP TABLE IF EXISTS Room CASCADE;
DROP TABLE IF EXISTS Guest CASCADE;
DROP TABLE IF EXISTS RoomOrder CASCADE;

CREATE TABLE HotelType(
	Id SERIAL PRIMARY KEY,
	Name VARCHAR(100) NOT NULL
);

CREATE TABLE Hotel(
	Id SERIAL PRIMARY KEY,
	Name VARCHAR(100) NOT NULL,
	City VARCHAR(100) NOT NULL,
	Street VARCHAR(100) NOT NULL,
	PhoneNumber VARCHAR(20) NOT NULL,
	Email VARCHAR(100),
	Stars INTEGER NOT NULL DEFAULT(3),
	IdType INTEGER REFERENCES HotelType(Id) ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE TABLE EmployeePosition(
	Id SERIAL PRIMARY KEY,
	Name VARCHAR(100) NOT NULL
);

CREATE TABLE Employee(
	Id SERIAL PRIMARY KEY,
	FirstName VARCHAR(100) NOT NULL,
	SecondName VARCHAR(100) NOT NULL,
	LastName VARCHAR(100),
	PhoneNumber VARCHAR(20),
	Salary INTEGER NOT NULL,
	IdPosition INTEGER REFERENCES EmployeePosition(Id) ON DELETE CASCADE ON UPDATE CASCADE,
	IdHotel INTEGER REFERENCES Hotel(Id) ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE TABLE RoomType(
	Id SERIAL PRIMARY KEY,
	Name VARCHAR(100) NOT NULL
);

CREATE TABLE Room(
	Id SERIAL PRIMARY KEY,
	Places INTEGER NOT NULL,
	Price INTEGER NOT NULL,
	Rating INTEGER NOT NULL,
	Status bool NULL DEFAULT false,
	Floor INTEGER NOT NULL,
	IdType INTEGER REFERENCES RoomType(Id) ON DELETE CASCADE ON UPDATE CASCADE,
	IdHotel INTEGER REFERENCES Hotel(Id) ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE TABLE Guest(
	Id SERIAL PRIMARY KEY,
	FirstName VARCHAR(100) NOT NULL,
	SecondName VARCHAR(100) NOT NULL,
	LastName VARCHAR(100) NOT NULL,
	PhoneNumber VARCHAR(20),
	PassportNumber VARCHAR(20) NOT NULL
);

CREATE TABLE RoomOrder(
	Id SERIAL PRIMARY KEY,
	StartDate TIMESTAMP NOT NULL,
	EndDate TIMESTAMP NOT NULL,
	IdRoom INTEGER REFERENCES Room(Id) ON DELETE CASCADE ON UPDATE CASCADE,
	idEmployee INTEGER REFERENCES Employee(Id) ON DELETE CASCADE ON UPDATE CASCADE,
	IdGuest INTEGER REFERENCES Guest(Id) ON DELETE CASCADE ON UPDATE CASCADE

);
INSERT INTO HotelType(Name) VALUES('Luxury');
INSERT INTO HotelType(Name) VALUES('Boutique and Lifestyle');
INSERT INTO HotelType(Name) VALUES('Full-Service');
INSERT INTO HotelType(Name) VALUES('Focused or select service');
INSERT INTO HotelType(Name) VALUES('Economy and limited service');
INSERT INTO HotelType(Name) VALUES('Extended stay');
INSERT INTO HotelType(Name) VALUES('Timeshare and destination clubs');
INSERT INTO HotelType(Name) VALUES('Motel');
INSERT INTO HotelType(Name) VALUES('Microstay');

INSERT INTO Hotel(Name,City,Street,PhoneNumber,Email,Stars,IdType) VALUES('Pasazh','Odessa','Deribasivska','+380939853628','pasag@gmail.com',5,3);
INSERT INTO Hotel(Name,City,Street,PhoneNumber,Email,Stars,IdType) VALUES('Chernoye more','Odessa','Malaya Arnautskaya','+38048123456','blacksea@gmail.com',3,4);
INSERT INTO Hotel(Name,City,Street,PhoneNumber,Email,Stars,IdType) VALUES('Nemo','Odessa','Langironovskaya','+380501434321','nemo@gmail.com',4,4);
INSERT INTO Hotel(Name,City,Street,PhoneNumber,Email,Stars,IdType) VALUES('Something','Odessa','Dovjenka','+380971434321','myhotel@gmail.com',3,8);
INSERT INTO Hotel(Name,City,Street,PhoneNumber,Email,Stars,IdType) VALUES('Dnipro','Kiev','Hreshatic','+380934534321','dnipro@gmail.com',3,5);
INSERT INTO Hotel(Name,City,Street,PhoneNumber,Email,Stars,IdType) VALUES('GoldenKolos','Moscow','Yaroslavskiy','+380951994321','kolosgolden@gmail.com',4,4);
INSERT INTO Hotel(Name,City,Street,PhoneNumber,Email,Stars,IdType) VALUES('RegentWarsawHotel','Warsaw','Belwederska','+380599434321','regentwarsawhotel@gmail.com',5,1);

INSERT INTO EmployeePosition(Name) VALUES('Director');
INSERT INTO EmployeePosition(Name) VALUES('Manager');
INSERT INTO EmployeePosition(Name) VALUES('Receptionist');

/*
director
*/
INSERT INTO Employee(FirstName,SecondName,LastName,PhoneNumber,Salary,IdPosition) VALUES('Bogdan','Dmitrievich','Sidorov','+380939853628',25000,1);

/*
Hotel managers
*/
INSERT INTO Employee(FirstName,SecondName,LastName,PhoneNumber,Salary,IdPosition,  IdHotel) VALUES('Evgeniy','Vasilyevich','Emelyanov','+380939853628',15000,2,1);
INSERT INTO Employee(FirstName,SecondName,LastName,PhoneNumber,Salary,IdPosition,  IdHotel) VALUES('Vladimir','Nikolayevich','Dolgorukov','+380509333628',10000,2,2);
INSERT INTO Employee(FirstName,SecondName,LastName,PhoneNumber,Salary,IdPosition,  IdHotel) VALUES('Valeriy','Olegovich','Levickiy','+380739234664',15000,2,3);
INSERT INTO Employee(FirstName,SecondName,LastName,PhoneNumber,Salary,IdPosition,  IdHotel) VALUES('Dmitriy','Bogdanovich','Lemikov','+380739232343',15000,2,4);
INSERT INTO Employee(FirstName,SecondName,LastName,PhoneNumber,Salary,IdPosition,  IdHotel) VALUES('Vasiliy','Olegovich','Sokolovskit','+380732345664',15000,2,5);
/*
receptionists
*/
/*
for first hotel
*/
INSERT INTO Employee(FirstName,SecondName,LastName,PhoneNumber,Salary,IdPosition,  IdHotel) VALUES('Mariya','Alekandrovna','Stoyanova','+380669131333',7000,3,1);
INSERT INTO Employee(FirstName,SecondName,LastName,PhoneNumber,Salary,IdPosition,  IdHotel) VALUES('Oleg','Mironovich','Amelin','+380732297252',7000,3,1);
/*
for second hotel
*/
INSERT INTO Employee(FirstName,SecondName,LastName,PhoneNumber,Salary,IdPosition,  IdHotel) VALUES('Miron','Yanovich','Oksfordskiy','+380506695326',5000,3,2);
INSERT INTO Employee(FirstName,SecondName,LastName,PhoneNumber,Salary,IdPosition,  IdHotel) VALUES('Vasiliy','Mikhalovich','Taranenko','+380676468007',5000,3,2);
/*
for third hotel
*/
INSERT INTO Employee(FirstName,SecondName,LastName,PhoneNumber,Salary,IdPosition,  IdHotel) VALUES('Viktor','Yurievich','Dubovoy','+380951051032',6000,3,3);
INSERT INTO Employee(FirstName,SecondName,LastName,PhoneNumber,Salary,IdPosition,  IdHotel) VALUES('Nikolay','Sergeevich','Loik','+380398221156',6000,3,3);
/*
for five hotel
*/
INSERT INTO Employee(FirstName,SecondName,LastName,PhoneNumber,Salary,IdPosition,  IdHotel) VALUES('Leha','Yanovich','Kalima','+380956695326',5000,3,5);



INSERT INTO Guest(FirstName,SecondName,LastName,PhoneNumber,PassportNumber)VALUES('Pavel','Olegovich','Tarasov','+380939853628','КМ34542');
INSERT INTO Guest(FirstName,SecondName,LastName,PhoneNumber,PassportNumber)VALUES('Yaroslav','Petrovich','Karastayanov','+38050985628','КМ63254');
INSERT INTO Guest(FirstName,SecondName,LastName,PhoneNumber,PassportNumber)VALUES('Bogdan','Ivanovich','Kolesnikov','+38093985334322','КМ45987');
INSERT INTO Guest(FirstName,SecondName,LastName,PhoneNumber,PassportNumber)VALUES('Nikolay','Stepanovich','Gorkiy','+380736598458','КМ56562');
INSERT INTO Guest(FirstName,SecondName,LastName,PhoneNumber,PassportNumber)VALUES('Sergey','Aleksandrovich','Nesterenko','+380737212284','КМ12457');
INSERT INTO Guest(FirstName,SecondName,LastName,PhoneNumber,PassportNumber)VALUES('Oleg','Romanovich','Fedorov','+380937825444','КМ78652');
INSERT INTO Guest(FirstName,SecondName,LastName,PhoneNumber,PassportNumber)VALUES('Nazar','Egorovich','Telpis','+380631258477','КМ45287');--7
INSERT INTO Guest(FirstName,SecondName,LastName,PhoneNumber,PassportNumber)VALUES('Maksim','Olegovich','Samiycherniy','+380936857228','КМ12784');
INSERT INTO Guest(FirstName,SecondName,LastName,PhoneNumber,PassportNumber)VALUES('Ostap','Petrovich','Bender','+380931488228','КМ13998');--9
INSERT INTO Guest(FirstName,SecondName,LastName,PhoneNumber,PassportNumber)VALUES('Tony','','Montana','+380937845826','КМ14822');
INSERT INTO Guest(FirstName,SecondName,LastName,PhoneNumber,PassportNumber)VALUES('Stanislav','Nikolayevich','Shulga','+380504578452','КМ22842');
INSERT INTO Guest(FirstName,SecondName,LastName,PhoneNumber,PassportNumber)VALUES('Viktoriya','Sergeevna','Kulish','+380939853628','КМ42042');
INSERT INTO Guest(FirstName,SecondName,LastName,PhoneNumber,PassportNumber)VALUES('Tatyana','Stepanovna','Kondrasova','+380939853628','КМ42334');
INSERT INTO Guest(FirstName,SecondName,LastName,PhoneNumber,PassportNumber)VALUES('Valeria','Olegovna','Slastina','+380939853628','КМ13213');
INSERT INTO Guest(FirstName,SecondName,LastName,PhoneNumber,PassportNumber)VALUES('Andrey','Andreevich','Malakhoff','+380939853628','КМ46576');
INSERT INTO Guest(FirstName,SecondName,LastName,PhoneNumber,PassportNumber)VALUES('Stepan','Nikifoorovich','Partak','+380939853628','КМ76878');
INSERT INTO Guest(FirstName,SecondName,LastName,PhoneNumber,PassportNumber)VALUES('Ivan','Ivanovich','Leniviy','+380939853628','КМ12986');
INSERT INTO Guest(FirstName,SecondName,LastName,PhoneNumber,PassportNumber)VALUES('Sergey','Davidovich','Shnurov','+380939853628','КМ12753');
INSERT INTO Guest(FirstName,SecondName,LastName,PhoneNumber,PassportNumber)VALUES('Nikolay','Olegovich','Tolstiy','+380939853628','КМ12542');
INSERT INTO Guest(FirstName,SecondName,LastName,PhoneNumber,PassportNumber)VALUES('Vasiliy','Vaganovich','Kalishuk','+380953453628','КМ16742');--19
INSERT INTO Guest(FirstName,SecondName,LastName,PhoneNumber,PassportNumber)VALUES('Nikolay','Bogdanovich','Tolstiy','+380959673628','КМ19842');

INSERT INTO RoomType(Name) VALUES('Luxury');
INSERT INTO RoomType(Name) VALUES('VIP');
INSERT INTO RoomType(Name) VALUES('Family');
INSERT INTO RoomType(Name) VALUES('ForOne');

-- ROOMS

INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(10,10000,10,1,1,true,1);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(10,9000,9,2,1,true,1);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(6,9000,9,2,1,false,1);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(6,9000,9,2,1,true,1);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(3,5000,8,3,1,true,1);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(3,5000,8,3,1,true,1);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(3,3000,5,3,1,false,1);--7
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(3,4000,8,3,1,false,1);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(3,5000,8,3,1,false,1);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(3,5000,8,3,1,false,1);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(3,5000,6,3,1,true,2);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(3,5000,7,3,1,true,2);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(3,5000,7,3,1,true,2);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(4,3500,7,3,1,false,2);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(2,4000,7,3,1,false,2);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(2,4000,9,3,1,true,2);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(1,3000,6,4,1,true,2);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(1,3000,6,4,1,false,2);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(1,3020,6,4,1,true,2);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(1,2500,6,4,1,false,2);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(1,3000,5,4,1,true,3);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(1,3000,5,4,1,false,3);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(1,2800,5,4,1,true,3);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(1,3000,5,4,1,false,3);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(1,3000,10,4,1,true,3);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(1,3000,7,4,1,false,3);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(1,3000,7,4,1,false,3);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(1,3000,8,4,1,false,3);

INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(5,8000,9,2,2,true,1);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(6,8000,9,2,2,false,1);
--30
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(3,4500,8,3,2,true,1);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(3,4500,7,3,2,true,1);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(3,4500,8,3,2,true,1);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(3,4500,8,3,2,false,2);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(3,4500,6,3,2,false,2);--35
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(3,4500,6,3,2,false,2);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(3,4500,7,3,2,true,2);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(3,4500,7,3,2,false,2);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(3,4500,9,3,2,false,2);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(4,4500,9,3,2,true,2);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(2,4000,9,3,2,true,3);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(2,4000,4,3,2,true,3);

INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(1,2500,7,4,2,true,3);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(1,2500,7,4,2,true,3);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(1,2500,7,4,2,true,3);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(1,2500,6,4,2,false,3);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(1,2500,6,4,2,false,3);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(1,2500,6,4,2,false,3);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(1,2500,6,4,2,false,4);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(1,2500,9,4,2,false,4);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(1,2500,9,4,2,true,4);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(1,2500,9,4,2,true,4);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(1,2500,8,4,2,true,4);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(1,2500,8,4,2,true,4);

INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(7,8000,5,2,3,true,1);

INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(3,4500,4,3,3,true,1);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(3,4500,4,3,3,false,1);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(2,3500,5,3,3,false,2);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(3,4500,6,3,3,false,2);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(2,3500,2,3,3,true,2);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(2,3500,8,3,3,true,2);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(2,3500,8,3,3,false,2);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(3,4500,8,3,3,false,2);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(3,4500,1,3,3,true,2);

INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(1,2700,7,4,3,true,3);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(1,2700,3,4,3,true,3);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(1,2700,6,4,3,false,3);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(1,2700,5,4,3,false,3);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(1,2700,2,4,3,false,3);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(1,2700,9,4,3,false,3);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(1,2700,10,4,3,false,3);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(1,2700,8,4,3,true,3);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(1,2700,8,4,3,true,3);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(1,2700,1,4,3,true,4);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(1,2700,4,4,3,true,4);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(1,2700,8,4,3,true,4);

INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(2,2700,8,3,5,true,1);
INSERT INTO Room(Places, Price, Rating, IdType,IdHotel, Status, Floor) VALUES(1,2700,8,4,5,true,1);

--here
--RoomOrders
INSERT INTO RoomOrder(IdRoom,StartDate,EndDate,idEmployee,IdGuest) VALUES(35,'2017-12-10','2017-12-20',1,4);
INSERT INTO RoomOrder(IdRoom,StartDate,EndDate,idEmployee,IdGuest) VALUES(36,'2017-12-18','2017-12-19',2,10);
INSERT INTO RoomOrder(IdRoom,StartDate,EndDate,idEmployee,IdGuest) VALUES(37,'2017-12-01','2017-12-08',3,11);
INSERT INTO RoomOrder(IdRoom,StartDate,EndDate,idEmployee,IdGuest) VALUES(4,'2017-11-20','2017-12-10',5,4);
INSERT INTO RoomOrder(IdRoom,StartDate,EndDate,idEmployee,IdGuest) VALUES(5,'2017-11-21','2017-12-09',5,5);
INSERT INTO RoomOrder(IdRoom,StartDate,EndDate,idEmployee,IdGuest) VALUES(6,'2017-11-22','2017-12-08',6,6);
INSERT INTO RoomOrder(IdRoom,StartDate,EndDate,idEmployee,IdGuest) VALUES(7,'2017-11-23','2017-12-07',6,7);
INSERT INTO RoomOrder(IdRoom,StartDate,EndDate,idEmployee,IdGuest) VALUES(8,'2017-11-24','2017-12-06',6,8);
INSERT INTO RoomOrder(IdRoom,StartDate,EndDate,idEmployee,IdGuest) VALUES(1,'2017-11-29','2017-12-07',5,3);--10
INSERT INTO RoomOrder(IdRoom,StartDate,EndDate,idEmployee,IdGuest) VALUES(2,'2017-11-30','2017-12-18',6,4);
INSERT INTO RoomOrder(IdRoom,StartDate,EndDate,idEmployee,IdGuest) VALUES(3,'2017-11-27','2017-12-20',11,5);
INSERT INTO RoomOrder(IdRoom,StartDate,EndDate,idEmployee,IdGuest) VALUES(4,'2017-11-29','2017-12-03',12,6);
INSERT INTO RoomOrder(IdRoom,StartDate,EndDate,idEmployee,IdGuest ) VALUES(32,'2017-11-01','2017-12-02',3,12);
INSERT INTO RoomOrder(IdRoom,StartDate,EndDate,idEmployee,IdGuest ) VALUES(33,'2017-11-02','2017-12-02',4,13);
INSERT INTO RoomOrder(IdRoom,StartDate,EndDate,idEmployee,IdGuest ) VALUES(34,'2017-11-03','2017-12-03',9,14);
INSERT INTO RoomOrder(IdRoom,StartDate,EndDate,idEmployee,IdGuest ) VALUES(12,'2017-11-19','2017-12-01',5,15);
INSERT INTO RoomOrder(IdRoom,StartDate,EndDate,idEmployee,IdGuest ) VALUES(15,'2017-11-20','2017-12-10',5,16);
INSERT INTO RoomOrder(IdRoom,StartDate,EndDate,idEmployee,IdGuest ) VALUES(16,'2017-11-22','2017-12-11',5,17);
INSERT INTO RoomOrder(IdRoom,StartDate,EndDate,idEmployee,IdGuest ) VALUES(22,'2017-11-22','2017-12-12',7,18);--20
INSERT INTO RoomOrder(IdRoom,StartDate,EndDate,idEmployee,IdGuest ) VALUES(23,'2017-11-22','2017-12-10',7,19);
INSERT INTO RoomOrder(IdRoom,StartDate,EndDate,idEmployee,IdGuest ) VALUES(24,'2017-11-24','2017-12-06',7,5);
INSERT INTO RoomOrder(IdRoom,StartDate,EndDate,idEmployee,IdGuest ) VALUES(25,'2017-11-24','2017-12-05',7,6);
INSERT INTO RoomOrder(IdRoom,StartDate,EndDate,idEmployee,IdGuest ) VALUES(26,'2017-11-25','2017-12-10',7,7);
INSERT INTO RoomOrder(IdRoom,StartDate,EndDate,idEmployee,IdGuest ) VALUES(27,'2017-11-26','2017-12-04',7,8);
INSERT INTO RoomOrder(IdRoom,StartDate,EndDate,idEmployee,IdGuest ) VALUES(28,'2017-11-26','2017-12-05',7,9);
INSERT INTO RoomOrder(IdRoom,StartDate,EndDate,idEmployee,IdGuest ) VALUES(29,'2017-11-26','2017-12-03',7,10);
INSERT INTO RoomOrder(IdRoom,StartDate,EndDate,idEmployee,IdGuest ) VALUES(30,'2017-11-26','2017-12-05',7,11);
INSERT INTO RoomOrder(IdRoom,StartDate,EndDate,idEmployee,IdGuest ) VALUES(35,'2017-12-11','2017-12-12',9,12);
INSERT INTO RoomOrder(IdRoom,StartDate,EndDate,idEmployee,IdGuest ) VALUES(36,'2017-12-08','2017-12-10',9,6);
INSERT INTO RoomOrder(IdRoom,StartDate,EndDate,idEmployee,IdGuest ) VALUES(37,'2017-11-29','2017-12-10',9,20);
INSERT INTO RoomOrder(IdRoom,StartDate,EndDate,idEmployee,IdGuest ) VALUES(38,'2017-11-29','2017-12-10',9,21);
INSERT INTO RoomOrder(IdRoom,StartDate,EndDate,idEmployee,IdGuest ) VALUES(38,'2017-12-10','2017-12-20',9,21);
INSERT INTO RoomOrder(IdRoom,StartDate,EndDate,idEmployee,IdGuest ) VALUES(39,'2017-11-30','2017-12-19',10,4);
INSERT INTO RoomOrder(IdRoom,StartDate,EndDate,idEmployee,IdGuest ) VALUES(40,'2017-11-21','2017-12-01',10,2);
INSERT INTO RoomOrder(IdRoom,StartDate,EndDate,idEmployee,IdGuest ) VALUES(77,'2017-11-21','2017-12-01',13,4);

INSERT INTO RoomOrder(IdRoom, StartDate,EndDate,idEmployee,IdGuest) VALUES(25,'2017-12-15','2017-12-20',12,2);
INSERT INTO RoomOrder(IdRoom, StartDate,EndDate,idEmployee,IdGuest) VALUES(14,'2017-12-07','2017-12-10',12,6);
INSERT INTO RoomOrder(IdRoom, StartDate,EndDate,idEmployee,IdGuest) VALUES(10,'2017-12-16','2017-12-25',12,3);
INSERT INTO RoomOrder(IdRoom, StartDate,EndDate,idEmployee,IdGuest) VALUES(29,'2017-12-19','2017-12-25',12,7);
INSERT INTO RoomOrder(IdRoom, StartDate,EndDate,idEmployee,IdGuest) VALUES(25,'2017-12-21','2017-12-23',6,2);




GRANT ALL PRIVILEGES ON DATABASE hotelnet_database TO admin;
GRANT SELECT, UPDATE, INSERT, DELETE ON employee TO manager;
GRANT SELECT ON hotel TO manager;
GRANT SELECT, UPDATE, INSERT, DELETE ON room TO manager;
GRANT SELECT, UPDATE, INSERT, DELETE ON roomorder TO manager;
GRANT SELECT, UPDATE, INSERT, DELETE ON guest TO manager;
GRANT SELECT ON employeeposition TO manager;
GRANT SELECT ON hoteltype TO manager;
GRANT SELECT ON roomtype TO manager;

GRANT SELECT ON employee TO receptionist;
GRANT SELECT ON hotel TO receptionist;
GRANT SELECT ON room TO receptionist;
GRANT SELECT, UPDATE, INSERT, DELETE ON roomorder TO receptionist;
GRANT SELECT, UPDATE, INSERT, DELETE ON guest TO receptionist;
GRANT SELECT ON employeeposition TO receptionist;
GRANT SELECT ON hoteltype TO receptionist;
GRANT SELECT ON roomtype TO receptionist;

GRANT SELECT ON hotel TO guest;
GRANT SELECT ON room TO guest;
GRANT SELECT, UPDATE, INSERT, DELETE ON roomorder TO guest;
GRANT SELECT, UPDATE, INSERT, DELETE ON guest TO guest;
GRANT SELECT ON hoteltype TO guest;
GRANT SELECT ON roomtype TO guest;



