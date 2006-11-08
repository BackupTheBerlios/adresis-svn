DROP TABLE AdUser CASCADE;
DROP TABLE AdSpace CASCADE;
DROP TABLE AdAudioVisual CASCADE;
--DROP TABLE AdSpaceReserve CASCADE;
--DROP TABLE AdAVReserve CASCADE;
DROP TABLE AdReserve CASCADE;
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


--*****************************************************
--***********EJEMPLO DE COMO CREAR UNA SEQUENCIA********
--******************************************************

--CREATE SEQUENCE tablename_colname_seq;
--CREATE TABLE tablename (
    --colname integer DEFAULT nextval('tablename_colname_seq') NOT NULL
--);

--DROP SEQUENCE adSpaceReserve_idReserve_seq;
--CREATE SEQUENCE adSpaceReserve_idReserve_seq;

-- CREATE TABLE AdSpaceReserve
-- (
-- 	idReserve integer default nextval('adSpaceReserve_idReserve_seq') not null,
-- 	typeReserve varchar(20),
-- 	idUserReserve varchar(20), FOREIGN KEY (idUserReserve) REFERENCES AdUser(loginUser)ON DELETE CASCADE ON UPDATE CASCADE, --Este es el usuario que realiza la reserva 
-- 	idUserResponsable varchar(20), FOREIGN KEY (idUserResponsable) REFERENCES AdUser(loginUser)ON DELETE CASCADE ON UPDATE CASCADE, -- Este es el usuario encargado de la reserva
-- 	idResource varchar(20), FOREIGN KEY (idResource) REFERENCES AdSpace(codeSpace)ON UPDATE CASCADE,
-- 	day varchar(9), --Este campo es para las reservas semestrales, para saber que dia es(lunes, martes....)
-- 	beginHour time,
-- 	endHour time,
-- 	beginDate date,
-- 	endDate date,
-- 	isActive boolean, -- Este campo es para saber si la reserva esta activa o ha sido cancelada.
-- 	destinationReserve varchar(80), check(beginHour < endHour and beginDate <= endDate)
-- );

-- DROP SEQUENCE adavreserve_idreserve_seq;
-- CREATE SEQUENCE adAvReserve_idReserve_seq;
-- CREATE TABLE AdAvReserve
-- (
-- 	idReserve integer default nextval('adAvReserve_idReserve_seq') not null,
-- 	typeReserve varchar(20),
-- 	idUserReserve varchar(20), FOREIGN KEY (idUserReserve) REFERENCES AdUser(loginUser)ON DELETE CASCADE ON UPDATE CASCADE, --Este es el usuario que realiza la reserva 
-- 	idUserResponsable varchar(20), FOREIGN KEY (idUserResponsable) REFERENCES AdUser(loginUser)ON DELETE CASCADE ON UPDATE CASCADE, -- Este es el usuario encargado de la reserva
-- 	idResource varchar(20), FOREIGN KEY (idResource) REFERENCES AdAudioVisual(numberinventoryAV)ON UPDATE CASCADE ON DELETE CASCADE,
-- 	day varchar(9), --Este campo es para las reservas semestrales, para saber que dia es(lunes, martes....
-- 	beginHour time ,
-- 	endHour time, 
-- 	beginDate date,
-- 	endDate date,
-- 	isActive boolean, -- Este campo es para saber si la reserva esta activa o ha sido cancelada.
-- 	destinationReserve varchar(80),
-- 	check(beginHour < endHour and beginDate <= endDate)
-- );

--////////////////////////////// NOTA IMPORTANTISIMA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
-- Las reservas se manejaran en una sola tabla en la cual, para diferencia si es una reserva de un espacio se colocara en idspace el idresource de el espacio y en idaudiovisual se colocara nill, en caso contrario,de que sea una reserva audiovisual, se colocara en idaudiovisual el idresource de la ayuda y en idSpace se colocara  nill.


DROP SEQUENCE adreserve_idreserve_seq;
CREATE SEQUENCE adReserve_idReserve_seq;
CREATE TABLE AdReserve
(
	idReserve integer default nextval('adAvReserve_idReserve_seq') not null,
	typeReserve varchar(20),
	idUserReserve varchar(20), FOREIGN KEY (idUserReserve) REFERENCES AdUser(loginUser)ON DELETE CASCADE ON UPDATE CASCADE, --Este es el usuario que realiza la reserva 
	idUserResponsable varchar(20), FOREIGN KEY (idUserResponsable) REFERENCES AdUser(loginUser)ON DELETE CASCADE ON UPDATE CASCADE, -- Este es el usuario encargado de la reserva
	idAudiovisual varchar(20), FOREIGN KEY (idAudiovisual) REFERENCES AdAudioVisual(numberinventoryAV)ON UPDATE CASCADE ON DELETE CASCADE,
	idSpace varchar(20), FOREIGN KEY (idSpace) REFERENCES AdSpace(codeSpace)ON UPDATE CASCADE ON DELETE CASCADE,
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







