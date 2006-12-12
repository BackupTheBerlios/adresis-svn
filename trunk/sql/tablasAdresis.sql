DROP TABLE AdUser CASCADE;
DROP TABLE AdRols CASCADE;
DROP TABLE AdAudioVisual CASCADE;
DROP TABLE AdAudioVisualType CASCADE;
DROP TABLE AdSpace CASCADE;
DROP TABLE AdSpaceType CASCADE;
DROP TABLE AdReserve CASCADE;
DROP TABLE ConfigurationSchooll CASCADE;
DROP TABLE ADCancellation CASCADE;


CREATE TABLE AdRols(
	rol integer,
	action varchar(22),
	permission integer, check(permission in (0,1))
);

INSERT INTO AdRols VALUES(0,'gestionarUsuario',1);
INSERT INTO AdRols VALUES(0,'consultarUsuario',1);
INSERT INTO AdRols VALUES(0,'gestionarEspacio',1);
INSERT INTO AdRols VALUES(0,'consultarEspacio',1);
INSERT INTO AdRols VALUES(0,'gestionarAudiovisual',1);
INSERT INTO AdRols VALUES(0,'consultarAudiovisual',1);
INSERT INTO AdRols VALUES(0,'gestionarSemestral',1);
INSERT INTO AdRols VALUES(0,'gestionarTemporal',1);
INSERT INTO AdRols VALUES(0,'consultarReservas',1);
INSERT INTO AdRols VALUES(0,'gestionarReportes',1);
INSERT INTO AdRols VALUES(0,'consultarReportes',1);
INSERT INTO AdRols VALUES(0,'consultarCancelaciones',1);

INSERT INTO AdRols VALUES(1,'gestionarUsuario',0);
INSERT INTO AdRols VALUES(1,'consultarUsuario',0);
INSERT INTO AdRols VALUES(1,'gestionarEspacio',0);
INSERT INTO AdRols VALUES(1,'consultarEspacio',1);
INSERT INTO AdRols VALUES(1,'gestionarAudiovisual',0);
INSERT INTO AdRols VALUES(1,'consultarAudiovisual',1);
INSERT INTO AdRols VALUES(1,'gestionarSemestral',0);
INSERT INTO AdRols VALUES(1,'gestionarTemporal',1);
INSERT INTO AdRols VALUES(1,'consultarReservas',1);
INSERT INTO AdRols VALUES(1,'gestionarReportes',0);
INSERT INTO AdRols VALUES(1,'consultarReportes',1);
INSERT INTO AdRols VALUES(1,'consultarCancelaciones',1);

INSERT INTO AdRols VALUES(2,'gestionarUsuario',0);
INSERT INTO AdRols VALUES(2,'consultarUsuario',0);
INSERT INTO AdRols VALUES(2,'gestionarEspacio',0);
INSERT INTO AdRols VALUES(2,'consultarEspacio',1);
INSERT INTO AdRols VALUES(2,'gestionarAudiovisual',0);
INSERT INTO AdRols VALUES(2,'consultarAudiovisual',1);
INSERT INTO AdRols VALUES(2,'gestionarSemestral',0);
INSERT INTO AdRols VALUES(2,'gestionarTemporal',0);
INSERT INTO AdRols VALUES(2,'consultarReservas',1);
INSERT INTO AdRols VALUES(2,'gestionarReportes',0);
INSERT INTO AdRols VALUES(2,'consultarReportes',1);
INSERT INTO AdRols VALUES(2,'consultarCancelaciones',1);


CREATE TABLE AdUser(
	rol integer,
	nameUser varchar(50),
	codeUser varchar(20) unique,
	loginUser varchar(20) unique,
	passwdUser varchar(20),
	PRIMARY KEY(loginUser, codeUser)
);

INSERT INTO AdUser VALUES(0,'root','root','root','root');
INSERT INTO AdUser VALUES(2,'estudiante','estudiante','estudiante');
-- INSERT INTO AdUser VALUES(0,'juan','0330912','juank','juank');
-- INSERT INTO AdUser VALUES(0,'carlos','0330913','carlosbor','carlosbor');
-- INSERT INTO AdUser VALUES(0,'hector','0330914','hecfa','hecfa');
-- INSERT INTO AdUser VALUES(0,'jorge','0330915','jorgecua','jorgecua');
-- INSERT INTO AdUser VALUES(0,'humberto','0330916','humbertocu','humbertocu');
-- INSERT INTO AdUser VALUES(0,'sebastian','0330917','sebrand','sebrand');
-- INSERT INTO AdUser VALUES(0,'deisy','0330918','deisych','deisych');
-- INSERT INTO AdUser VALUES(0,'rafael','0330919','rafaca','rafaca');
-- INSERT INTO AdUser VALUES(0,'jhon','0330910','jhonmu','jhonmu');



CREATE TABLE AdSpace(
	codeSpace varchar(20) PRIMARY KEY,
	typespace varchar(20),
	coolAirSpace boolean,
	capacitySpace integer,
	nameSpace varchar(30)
);
INSERT INTO AdSpace VALUES('null','null');



CREATE TABLE ADSpaceType(
	type varchar(20)
);

INSERT INTO ADSpaceType VALUES('sala de sistemas');
INSERT INTO ADSpaceType VALUES('sala de reuniones');
INSERT INTO ADSpaceType VALUES('auditorio');
INSERT INTO ADSpaceType VALUES('salon');




CREATE TABLE AdAudioVisual(
	typeAV varchar(20),
	marksEquipmentAV varchar(20),
	estateAV varchar(5) check(estateAv in ('bueno', 'malo')),
	numberinventoryAV varchar(20) PRIMARY KEY,
	codeSpace varchar(20) DEFAULT 'null' REFERENCES AdSpace(codeSpace)ON UPDATE CASCADE ON DELETE SET DEFAULT
	--Si la ayuda no tiene espacio es Null
	--este codigo se utiliza para determinar que ayudas tiene un espacio
);




CREATE TABLE ADAudioVisualType(
	type varchar(20)
);

INSERT INTO ADAudioVisualType VALUES('computador portatil');
INSERT INTO ADAudioVisualType VALUES('computador personal');
INSERT INTO ADAudioVisualType VALUES('proyector acetatos');
INSERT INTO ADAudioVisualType VALUES('reproductor VHS');
INSERT INTO ADAudioVisualType VALUES('televisor');
INSERT INTO ADAudioVisualType VALUES('video beam');




--////////////////////////////// NOTA IMPORTANTISIMA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
-- Las reservas se manejaran en una sola tabla en la cual, para diferencia si es una reserva de un espacio se colocara en idspace el idresource de el espacio y en idaudiovisual se colocara nill, en caso contrario,de que sea una reserva audiovisual, se colocara en idaudiovisual el idresource de la ayuda y en idSpace se colocara  "null".
DROP SEQUENCE adreserve_idreserve_seq;
CREATE SEQUENCE adReserve_idReserve_seq;
CREATE TABLE ADReserve
(
	idReserve integer default nextval('adReserve_idReserve_seq') not null unique,
	typeReserve varchar(20),
	idUserReserve varchar(20) REFERENCES AdUser(loginUser) ON DELETE CASCADE ON UPDATE CASCADE, --Este es el usuario que realiza la reserva 
	idUserResponsable varchar(20) REFERENCES AdUser(loginUser) ON DELETE CASCADE ON UPDATE CASCADE, -- Este es el usuario encargado de la reserva
	idAudiovisual varchar(20) REFERENCES AdAudioVisual(numberinventoryAV)ON UPDATE CASCADE,
	idSpace varchar(20) REFERENCES AdSpace(codeSpace)ON UPDATE CASCADE,
	day varchar(9), --Este campo es para las reservas semestrales, para saber que dia es(lunes, martes....
	beginHour time,
	endHour time, 
	beginDate date, -- DD/MM/YY
	endDate date,	-- DD/MM/YY
	isActive boolean, -- Este campo es para saber si la reserva esta activa o ha sido cancelada.
	destinationReserve text,
	check(beginHour < endHour and beginDate <= endDate)
);


CREATE TABLE ConfigurationSchooll(
	beginDateSem date,	--DD/MM/YY
	endDateSem date		--DD/MM/YY
);

SET DATESTYLE TO dmy;
INSERT INTO configurationschooll VALUES ('01/09/2006','15/12/2006');



CREATE TABLE ADCancellation(
	idcancellation integer REFERENCES ADReserve(idReserve) ON DELETE CASCADE ON UPDATE CASCADE,
	idUserCancellation varchar(10) REFERENCES AdUser(loginUser) ON DELETE CASCADE ON UPDATE CASCADE,
	hourCancellation time,
	dateCancellation date,
	razonCancellation text
);


DROP TABLE ADREPORT CASCADE;
CREATE TABLE ADReport(
	creator varchar(20) REFERENCES ADUser(loginUser) ON UPDATE CASCADE ON DELETE CASCADE,
	typeReport int,
	consult int,
	beginDate date,
	endDate date,
	created timestamp,
	content text,
	PRIMARY KEY(creator, created)
);
