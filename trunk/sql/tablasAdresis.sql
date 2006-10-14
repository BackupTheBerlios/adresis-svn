DROP TABLE AdUser CASCADE;
DROP TABLE AdSpace CASCADE;
DROP TABLE AdAudioVisual CASCADE;
DROP TABLE AdSpaceReserve CASCADE;
DROP TABLE AdAVReserve CASCADE;
DROP TABLE AdRols CASCADE;
-- DROP TABLE AdScheduleAV CASCADE;
-- DROP TABLE AdScheduleSpace CASCADE;


CREATE TABLE AdRols(
	rol integer,
	action varchar(20),
	permission integer, check(permission in (0,1))
);

INSERT INTO AdRols VaLUES(0,'gestionarUsuario',1);
INSERT INTO AdRols VaLUES(0,'consultarUsuario',1);
INSERT INTO AdRols VaLUES(0,'gestionarEspacio',1);
INSERT INTO AdRols VaLUES(0,'consultarEspacio',1);
INSERT INTO AdRols VaLUES(0,'gestionarAudiovisual',1);
INSERT INTO AdRols VaLUES(0,'consultarAudiovisual',1);
INSERT INTO AdRols VaLUES(0,'gestionarSemestral',1);
INSERT INTO AdRols VaLUES(0,'gestionarTemporal',1);
INSERT INTO AdRols VaLUES(0,'consultarReservas',1);
INSERT INTO AdRols VaLUES(0,'gestionarReportes',1);
INSERT INTO AdRols VaLUES(0,'consultarReportes',1);

INSERT INTO AdRols VaLUES(1,'gestionarUsuario',0);
INSERT INTO AdRols VaLUES(1,'consultarUsuario',0);
INSERT INTO AdRols VaLUES(1,'gestionarEspacio',0);
INSERT INTO AdRols VaLUES(1,'consultarEspacio',1);
INSERT INTO AdRols VaLUES(1,'gestionarAudiovisual',0);
INSERT INTO AdRols VaLUES(1,'consultarAudiovisual',1);
INSERT INTO AdRols VaLUES(1,'gestionarSemestral',0);
INSERT INTO AdRols VaLUES(1,'gestionarTemporal',1);
INSERT INTO AdRols VaLUES(1,'consultarReservas',1);
INSERT INTO AdRols VaLUES(1,'gestionarReportes',0);
INSERT INTO AdRols VaLUES(1,'consultarReportes',1);

INSERT INTO AdRols VaLUES(2,'gestionarUsuario',0);
INSERT INTO AdRols VaLUES(2,'consultarUsuario',0);
INSERT INTO AdRols VaLUES(2,'gestionarEspacio',0);
INSERT INTO AdRols VaLUES(2,'consultarEspacio',1);
INSERT INTO AdRols VaLUES(2,'gestionarAudiovisual',0);
INSERT INTO AdRols VaLUES(2,'consultarAudiovisual',1);
INSERT INTO AdRols VaLUES(2,'gestionarSemestral',0);
INSERT INTO AdRols VaLUES(2,'gestionarTemporal',0);
INSERT INTO AdRols VaLUES(2,'consultarReservas',1);
INSERT INTO AdRols VaLUES(2,'gestionarReportes',0);
INSERT INTO AdRols VaLUES(2,'consultarReportes',1);




CREATE TABLE AdUser(
	rol integer,
	nameUser varchar(30),
	codeUser varchar(20),
	loginUser varchar(20) PRIMARY KEY,
	passwdUser varchar(20)
);

INSERT INTO AdUser VALUES(0,'charly','0330911','charagmz','charagmz');
INSERT INTO AdUser VALUES(0,'juan','0330912','juank','juank');
INSERT INTO AdUser VALUES(0,'carlos','0330913','carlosbor','carlosbor');
INSERT INTO AdUser VALUES(0,'hector','0330914','hecfa','hecfa');
INSERT INTO AdUser VALUES(0,'jorge','0330915','jorgecua','jorgecua');
INSERT INTO AdUser VALUES(0,'humberto','0330916','humbertocu','humbertocu');
INSERT INTO AdUser VALUES(0,'sebastian','0330917','sebrand','sebrand');
INSERT INTO AdUser VALUES(0,'deisy','0330918','deisych','deisych');
INSERT INTO AdUser VALUES(0,'rafael','0330919','rafaca','rafaca');
INSERT INTO AdUser VALUES(0,'jhon','0330910','jhonmu','jhonmu');


CREATE TABLE AdSpace(
	codeSpace varchar(20) PRIMARY KEY,
	typespace varchar(20),
	coolAirSpace boolean,
	capacitySpace integer,
	nameSpace varchar(30)
);

INSERT INTO AdSpace VALUES('211','salon', false, 36, 'salon ingenieria 1');
INSERT INTO AdSpace VALUES('212','salon', false, 36, 'salon ingenieria 2');
INSERT INTO AdSpace VALUES('213','salon', false, 36, 'salon ingenieria 3');
INSERT INTO AdSpace VALUES('214','auditorio', true, 120, 'auditorio ingenieria 1');
INSERT INTO AdSpace VALUES('215','auditorio', true, 120, 'auditorio ingenieria 2');
INSERT INTO AdSpace VALUES('216','auditorio', true, 120, 'auditorio ingenieria 3');
INSERT INTO AdSpace VALUES('217','salon', true, 26, 'salon ingenieria 4');
INSERT INTO AdSpace VALUES('218','salon', true, 26, 'salon ingenieria 5');
INSERT INTO AdSpace VALUES('219','salon', true, 26, 'salon ingenieria 6');
INSERT INTO AdSpace VALUES('220','salon', true, 26, 'salon ingenieria 7');
INSERT INTO AdSpace VALUES('null','null');


CREATE TABLE AdAudioVisual(
	typeAV varchar(20),
	marksEquipmentAV varchar(20),
	estateAV varchar(50) check(estateAv in ('bueno', 'malo')),
	numberinventoryAV varchar(20) PRIMARY KEY,
	codeSpace varchar(20), FOREIGN KEY (codeSpace) REFERENCES AdSpace(codeSpace)
	--Si la ayuda no tiene espacio es Null
	--este codigo se utiliza para determinar que ayudas tiene un espacio
);

INSERT INTO AdAudioVisual VALUES('video beam','sony','bueno', '1122', '211');
INSERT INTO AdAudioVisual VALUES('proyector acetatos','epson','bueno', '1133', '212');
INSERT INTO AdAudioVisual VALUES('proyector acetatos','hp','bueno', '1144', '213');
INSERT INTO AdAudioVisual VALUES('proyector acetatos','epson','malo', '1155', '214');
INSERT INTO AdAudioVisual VALUES('cpu','dell','bueno', '1166', '211');
INSERT INTO AdAudioVisual VALUES('cpu','dell','bueno', '1177', '216');
INSERT INTO AdAudioVisual VALUES('reproductor VHS','sony','bueno', '1188', '217');
INSERT INTO AdAudioVisual VALUES('televisor','sony','bueno', '1199', '217');
INSERT INTO AdAudioVisual VALUES('computador portatil','dell','bueno', '1100', '218');
INSERT INTO AdAudioVisual VALUES('computador portatil','dell','malo', '1222', '219');



/*
CREATE TABLE AdScheduleAV(
	idAudioVisual varchar(20), FOREIGN KEY (idAudioVisual) REFERENCES AdAudioVisual(numberinventoryAV)ON DELETE CASCADE ON UPDATE CASCADE,
	idScheduleAV varchar(20) PRIMARY KEY,
	beginHour time ,
	endHour time, 
	beginDate date,
	endDate date,
	check(beginHour < endHour and beginDate <= endDate)
);


INSERT INTO AdScheduleAV VALUES('1122','111110','10:00:00.00','11:00:00.00','1/6/2006','1/6/2006');
INSERT INTO AdScheduleAV VALUES('1133','111111','11:00:00.00','12:00:00.00','2/6/2006','2/6/2006');
INSERT INTO AdScheduleAV VALUES('1144','111112','12:00:00.00','13:00:00.00','3/6/2006','3/6/2006');
INSERT INTO AdScheduleAV VALUES('1155','111113','13:00:00.00','14:00:00.00','4/6/2006','4/6/2006');
INSERT INTO AdScheduleAV VALUES('1166','111114','14:00:00.00','15:00:00.00','5/6/2006','5/6/2006');
INSERT INTO AdScheduleAV VALUES('1177','111115','15:00:00.00','16:00:00.00','6/6/2006','6/6/2006');
INSERT INTO AdScheduleAV VALUES('1188','111116','16:00:00.00','17:00:00.00','7/6/2006','7/6/2006');
INSERT INTO AdScheduleAV VALUES('1199','111117','17:00:00.00','18:00:00.00','8/6/2006','8/6/2006');
INSERT INTO AdScheduleAV VALUES('1100','111118','18:00:00.00','19:00:00.00','9/6/2006','9/6/2006');
INSERT INTO AdScheduleAV VALUES('1222','111119','19:00:00.00','20:00:00.00','10/6/2006','10/6/2006');

CREATE TABLE AdScheduleSpace(
	idSpace varchar(20), FOREIGN KEY (idSpace) REFERENCES AdSpace(codeSpace)ON UPDATE CASCADE,
	idScheduleSpace varchar(20) PRIMARY KEY,
	beginHour time,
	endHour time,
	beginDate date,
	endDate date,
	check(beginHour < endHour and beginDate <= endDate)
);

INSERT INTO AdScheduleSpace VALUES('211','11510','08:00:00.00','10:00:00.00','1/6/2006','1/6/2006');
INSERT INTO AdScheduleSpace VALUES('212','11511','10:00:00.00','11:00:00.00','1/6/2006','1/6/2006');
INSERT INTO AdScheduleSpace VALUES('213','11512','12:00:00.00','13:00:00.00','1/6/2006','1/6/2006');
INSERT INTO AdScheduleSpace VALUES('214','11513','14:00:00.00','16:00:00.00','1/6/2006','1/6/2006');
INSERT INTO AdScheduleSpace VALUES('215','11514','10:00:00.00','11:00:00.00','1/6/2006','1/6/2006');
INSERT INTO AdScheduleSpace VALUES('216','11515','07:00:00.00','10:00:00.00','1/6/2006','1/6/2006');
INSERT INTO AdScheduleSpace VALUES('217','11516','09:00:00.00','11:00:00.00','1/6/2006','1/6/2006');
INSERT INTO AdScheduleSpace VALUES('218','11517','16:00:00.00','18:00:00.00','1/6/2006','1/6/2006');
INSERT INTO AdScheduleSpace VALUES('219','11518','16:00:00.00','18:00:00.00','1/6/2006','1/6/2006');
INSERT INTO AdScheduleSpace VALUES('220','11519','16:00:00.00','18:00:00.00','1/6/2006','1/6/2006');


CREATE TABLE AdSpaceReserve(
	typeReserve varchar(20),
	--tipoRecurso varvarchar(20),
	idUser varchar(20), FOREIGN KEY (idUser) REFERENCES AdUser(loginUser)ON DELETE CASCADE ON UPDATE CASCADE,
	idSpace varchar(20), FOREIGN KEY (idSpace) REFERENCES AdSpace(codeSpace)ON UPDATE CASCADE,
	idScheduleSpace varchar(20), FOREIGN KEY (idScheduleSpace) REFERENCES AdScheduleSpace(idScheduleSpace)ON DELETE CASCADE ON UPDATE CASCADE, 
	destinationReserve varchar(50),
	PRIMARY KEY(idUser, idSpace, idScheduleSpace)
);

INSERT INTO AdSpaceReserve VALUES('semestral','charagmz','211','11510','Clase de Algoritmia y Programacion');
INSERT INTO AdSpaceReserve VALUES('temporal','juank','212','11511','Monitoria Matematicas Discretas I');
INSERT INTO AdSpaceReserve VALUES('semestral','carlosbor','213','11512','Clase de FADA');
INSERT INTO AdSpaceReserve VALUES('temporal','hecfa','214','11513','Monitoria de ITI');
INSERT INTO AdSpaceReserve VALUES('semestral','jorgecua','215','11514','Desarrollo proyecto ADRESIS');
INSERT INTO AdSpaceReserve VALUES('temporal','humbertocu','216','11515','Taller de Desarrollo de Software');
INSERT INTO AdSpaceReserve VALUES('semestral','sebrand','217','11516','Clase de IPOO');
INSERT INTO AdSpaceReserve VALUES('temporal','deisych','218','11517','Monitoria IPOO');
INSERT INTO AdSpaceReserve VALUES('semestral','rafaca','219','11518','Desarrollo proyecto CENESIS');
INSERT INTO AdSpaceReserve VALUES('temporal','jhonmu','220','11519','Monitoria FADA');


CREATE TABLE AdAVReserve(
	typeReserve varchar(20),
	--tipoRecurso varvarchar(20),
	idUser varchar(20), FOREIGN KEY (idUser) REFERENCES AdUser(loginUser) ON UPDATE CASCADE ON DELETE CASCADE,
	idAudioVisual varchar(20), FOREIGN KEY (idAudioVisual) REFERENCES AdAudioVisual(numberinventoryAV)ON UPDATE CASCADE ON DELETE CASCADE,
	idScheduleAV varchar(20), FOREIGN KEY (idScheduleAV) REFERENCES AdScheduleAV(idScheduleAV)ON UPDATE CASCADE ON DELETE CASCADE, 
	destinationReserve varchar(50),
	PRIMARY KEY(idUser, idAudioVisual, idScheduleAV)
);

INSERT INTO AdAVReserve VALUES('semestral','charagmz','1122', '111110', 'Clase de Algoritmia y Programacion');
INSERT INTO AdAVReserve VALUES('semestral','juank','1133', '111111', 'Clase de FADA');
INSERT INTO AdAVReserve VALUES('semestral','carlosbor','1144', '111112', 'Clase de IPOO');
INSERT INTO AdAVReserve VALUES('temporal','hecfa','1155', '111113', 'Monitoria de ITI');
INSERT INTO AdAVReserve VALUES('temporal','jorgecua','1166', '111114', 'Monitoria Matematicas Discretas I');
INSERT INTO AdAVReserve VALUES('temporal','humbertocu','1177', '111115', 'Desarrollo Proyecto ADRESIS');
INSERT INTO AdAVReserve VALUES('temporal','sebrand','1188', '111116', 'Desarrollo Proyecto CENESIS');
INSERT INTO AdAVReserve VALUES('temporal','humbertocu','1199', '111117', 'Desarrollo Proyecto ADRESIS');
INSERT INTO AdAVReserve VALUES('temporal','deisych','1100', '111118', 'Monitoria de FADA');*/


-- CREATE TABLE AdSpaceCancelation(
-- 	typeCancelation varchar(20),
-- 	tipoRecurso varchar(20),
-- 	idUser varchar(10), FOREIGN KEY (idUser) REFERENCES AdUser(loginUser),
-- 	idSpace varchar(10), FOREIGN KEY (idSpace) REFERENCES AdSpace(codeSpace),
-- 	idScheduleSpace varchar(10), FOREIGN KEY (idScheduleSpace) REFERENCES AdScheduleSpace(idScheduleSpace), 
-- 	cancelationDetails varchar(50),
-- -- 	PRIMARY KEY(idUser, idSpace, idScheduleSpace)
-- );
-- 
-- CREATE TABLE AdAVCancelation(
-- 	typeCancelation varchar(20),
-- 	tipoRecurso varchar(20),
-- 	idUser varchar(10), FOREIGN KEY (idUser) REFERENCES AdUser(loginUser),
-- 	idAudioVisual varchar(10), FOREIGN KEY (idAudioVisual) REFERENCES AdAudioVisual(numberinventoryAV),
-- 	idScheduleAV varchar(10), FOREIGN KEY (idScheduleAV) REFERENCES AdScheduleAV(idScheduleAV), 
-- 	cancelationDetails varchar(50),
-- -- 	PRIMARY KEY(idUser, idAudioVisual, idScheduleAV)
-- );


/*****************************************************
***********EJEMPLO DE COMO CREAR UNA SEQUENCIA********
******************************************************

CREATE SEQUENCE tablename_colname_seq;
CREATE TABLE tablename (
    colname integer DEFAULT nextval('tablename_colname_seq') NOT NULL
);*/

DROP SEQUENCE adSpaceReserve_idReserve_seq;
CREATE SEQUENCE adSpaceReserve_idReserve_seq;

CREATE TABLE AdSpaceReserve
(
	idReserve integer default nextval('adSpaceReserve_idReserve_seq') not null,
	typeReserve varchar(20),
	idUserReserve varchar(20), FOREIGN KEY (idUserReserve) REFERENCES AdUser(loginUser)ON DELETE CASCADE ON UPDATE CASCADE, --Este es el usuario que realiza la reserva 
	idUserResponsable varchar(20), FOREIGN KEY (idUserResponsable) REFERENCES AdUser(loginUser)ON DELETE CASCADE ON UPDATE CASCADE, -- Este es el usuario encargado de la reserva
	idResource varchar(20), FOREIGN KEY (idResource) REFERENCES AdSpace(codeSpace)ON UPDATE CASCADE,
	day varchar(9), --Este campo es para las reservas semestrales, para saber que dia es(lunes, martes....)
	beginHour time,
	endHour time,
	beginDate date,
	endDate date,
	isActive boolean, -- Este campo es para saber si la reserva esta activa o ha sido cancelada.
	destinationReserve varchar(80), check(beginHour < endHour and beginDate <= endDate)
);

DROP SEQUENCE adavreserve_idreserve_seq;
CREATE SEQUENCE adAvReserve_idReserve_seq;
CREATE TABLE AdAvReserve
(
	idReserve integer default nextval('adAvReserve_idReserve_seq') not null,
	typeReserve varchar(20),
	idUserReserve varchar(20), FOREIGN KEY (idUserReserve) REFERENCES AdUser(loginUser)ON DELETE CASCADE ON UPDATE CASCADE, --Este es el usuario que realiza la reserva 
	idUserResponsable varchar(20), FOREIGN KEY (idUserResponsable) REFERENCES AdUser(loginUser)ON DELETE CASCADE ON UPDATE CASCADE, -- Este es el usuario encargado de la reserva
	idResource varchar(20), FOREIGN KEY (idResource) REFERENCES AdAudioVisual(numberinventoryAV)ON UPDATE CASCADE ON DELETE CASCADE,
	day varchar(9), --Este campo es para las reservas semestrales, para saber que dia es(lunes, martes....
	beginHour time ,
	endHour time, 
	beginDate date,
	endDate date,
	isActive boolean, -- Este campo es para saber si la reserva esta activa o ha sido cancelada.
	destinationReserve varchar(80),
	check(beginHour < endHour and beginDate <= endDate)
);

-- INSERT INTO adspacereserve (typereserve,iduser,idspace,day,beginhour,endhour,begindate,enddate,isactive) VALUES ('Semestral', 'hecfa', '211', 'lunes', '8:00','10:00','01/15/2006','08/15/2006',true); 







