#include "adaudiovisual.h"
#include "ddebug.h"

ADAudioVisual::ADAudioVisual()
	: ADObject()
{
	m_valid = false;
}

ADAudioVisual::ADAudioVisual(const QString & type, const QString & marksEquipment, const QString & estate, const QString & numberInventory, const QString & codeSpace) 
	: ADObject(), m_type(type), m_marksEquipment(marksEquipment), m_estate(estate), m_numberInventory(numberInventory), m_codeSpace(codeSpace)
{
	m_valid = true;
}

ADAudioVisual::~ADAudioVisual()
{
}

/*		typeAV varchar(20),
		marksEquipmentAV varchar(10),
		estateAV varchar(50),
		numberinventoryAV varchar(10) PRIMARY KEY,
		codeSpace varchar(10), FOREIGN KEY (codeSpace) REFERENCES AdSpace(codeSpace)*/

ADInsertPackage ADAudioVisual::insertPackage()
{
	ADInsertPackage insert("adaudiovisual", QStringList() << "typeav" << "marksEquipmentav" << "estateav"<< "numberinventoryav" << "codespace", QStringList() << m_type << m_marksEquipment << m_estate << m_numberInventory << m_codeSpace );
	
	dDebug() << insert.toString();
	return insert;
	
}

ADUpdatePackage ADAudioVisual::updatePackage() 
{
	ADUpdatePackage update("adaudiovisual", QStringList() << "typeav" << "marksEquipmentav" << "estateav" << "numberinventoryav" << "codeSpace", QStringList() << m_type << m_marksEquipment << m_estate << m_numberInventory << m_codeSpace );
	
	return update;
}

void ADAudioVisual::setValues(XMLResults values)
{
	dDebug() << "here adaudiovisual";
	m_type = values["typeav"];
	m_marksEquipment  = values["marksEquipmentav"];
	m_estate  = values["estateav"];
	m_numberInventory  = values["numberinventoryav"];
	m_codeSpace  = values["codeSpace"];
	
	m_valid = true;
}

bool ADAudioVisual::isValid()
{
	return m_valid;
}

QString /*&*/ ADAudioVisual::type() /*const*/
{
	return m_type;
}

QString ADAudioVisual::marksEquipment()
{
	return m_marksEquipment;
}

QString ADAudioVisual::estate()
{
	return m_estate;
}

QString ADAudioVisual::numberInventory()
{
	return m_numberInventory;
}

QString ADAudioVisual::codeSpace()
{
	return m_codeSpace;
}
