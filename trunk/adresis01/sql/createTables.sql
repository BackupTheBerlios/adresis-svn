DROP TABLE AdUser CASCADE;
DROP TABLE AdSpace CASCADE;
DROP TABLE AdAudioVisual CASCADE;
DROP TABLE AdSpaceWhitAV CASCADE;
DROP TABLE AdSpaceReserve CASCADE;
DROP TABLE AdAVReserve CASCADE;
DROP TABLE AdSpaceCancelation CASCADE;
DROP TABLE AdAVCancelation CASCADE;
DROP TABLE AdSchedule CASCADE;

CREATE TABLE AdUser(
	permissionsUser char(4),
	nameUser varchar(30),
	codeUser varchar(10),
	loginUser varchar(10) PRIMARY KEY,
	passwdUser varchar(10)
);

CREATE TABLE AdSpace(
	codeSpace varchar(10) PRIMARY KEY,
	typeSpace varchar(20),
	coolAirSpace boolean,
	capacitySpace integer,
	nameSpace varchar(20)
);

CREATE TABLE AdAudioVisual(
	typeAV varchar(20),
	marksEquipmentAV varchar(10),
	estateAV varchar(50),
	numberinventoryAV varchar(10) PRIMARY KEY,
	codeSpace varchar(10), FOREIGN KEY (codeSpace) REFERENCES AdSpace(codeSpace)
	--Si la ayuda no tiene espacio es Null
	--este codigo se utiliza para determinar que ayudas tiene un espacio
);

-- CREATE TABLE AdSpaceWhitAV(
-- 	nroInventarioAyuda varchar(10), FOREIGN KEY (nroInventarioAyuda) REFERENCES AyudaAudioVisual(numeroInventario),
-- 	codigoEspacio varchar(10), FOREIGN KEY (codigoEspacio) REFERENCES Espacio(codigo),
-- 	PRIMARY KEY(nroInventarioAyuda, codigoEspacio)
-- );

CREATE TABLE AdScheduleAV(
	idAudioVisual varchar(10), FOREIGN KEY (idAudioVisual) REFERENCES AdAudioVisual(numberinventoryAV),
	idScheduleAV varchar(10) PRIMARY KEY,
	beginHour time,
	endHour time,
	beginDate date,
	endDate date
);

CREATE TABLE AdScheduleSpace(
	idSpace varchar(10), FOREIGN KEY (idSpace) REFERENCES AdSpace(codeSpace),
	idScheduleSpace varchar(10) PRIMARY KEY,
	beginHour time,
	endHour time,
	beginDate date,
	endDate date
);

CREATE TABLE AdSpaceReserve(
	typeReserve varchar(20),
	--tipoRecurso varchar(20),
	idUser varchar(10), FOREIGN KEY (idUser) REFERENCES AdUser(loginUser),
	idSpace varchar(10), FOREIGN KEY (idSpace) REFERENCES AdSpace(codeSpace),
	idScheduleSpace varchar(10), FOREIGN KEY (idScheduleSpace) REFERENCES AdScheduleSpace(idScheduleSpace), 
	destinationReserve varchar(50),
	PRIMARY KEY(idUser, idSpace, idScheduleSpace)
);

CREATE TABLE AdAVReserve(
	typeReserve varchar(20),
	--tipoRecurso varchar(20),
	idUser varchar(10), FOREIGN KEY (idUser) REFERENCES AdUser(loginUser),
	idAudioVisual varchar(10), FOREIGN KEY (idAudioVisual) REFERENCES AdAudioVisual(numberinventoryAV),
	idScheduleAV varchar(10), FOREIGN KEY (idScheduleAV) REFERENCES AdScheduleAV(idScheduleAV), 
	destinationReserve varchar(50),
	PRIMARY KEY(idUser, idAudioVisual, idScheduleAV)
);

CREATE TABLE AdSpaceCancelation(
	typeCancelation varchar(20),
	--tipoRecurso varchar(20),
	idUser varchar(10), FOREIGN KEY (idUser) REFERENCES AdUser(loginUser),
	idSpace varchar(10), FOREIGN KEY (idSpace) REFERENCES AdSpace(codeSpace),
	idScheduleSpace varchar(10), FOREIGN KEY (idScheduleSpace) REFERENCES AdScheduleSpace(idScheduleSpace), 
	cancelationDetails varchar(50),
	PRIMARY KEY(idUser, idSpace, idScheduleSpace)
);

CREATE TABLE AdAVCancelation(
	typeCancelation varchar(20),
	--tipoRecurso varchar(20),
	idUser varchar(10), FOREIGN KEY (idUser) REFERENCES AdUser(loginUser),
	idAudioVisual varchar(10), FOREIGN KEY (idAudioVisual) REFERENCES AdAudioVisual(numberinventoryAV),
	idScheduleAV varchar(10), FOREIGN KEY (idScheduleAV) REFERENCES AdScheduleAV(idScheduleAV), 
	cancelationDetails varchar(50),
	PRIMARY KEY(idUser, idAudioVisual, idScheduleAV)
);





INSERT INTO AdUser VALUES('1111','charly','0330911','charagmz','charagmz');
