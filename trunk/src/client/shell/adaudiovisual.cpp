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

ADInsertPackage ADAudioVisual::insertPackage()
{
	ADInsertPackage insert("adaudiovisual", QStringList() << "typeav" << "marksequipmentav" << "estateav"<< "numberinventoryav" << "codespace", QStringList() << SQLSTR(m_type) << SQLSTR(m_marksEquipment) << SQLSTR(m_estate) << SQLSTR(m_numberInventory) << SQLSTR(m_codeSpace) );
	
	dDebug() << insert.toString();
	return insert;
	
}

ADUpdatePackage ADAudioVisual::updatePackage() 
{
	ADUpdatePackage update("adaudiovisual", QStringList() << "typeav" << "marksEquipmentav" << "estateav" << "numberinventoryav" << "codeSpace", QStringList() << SQLSTR(m_type) << SQLSTR(m_marksEquipment) << SQLSTR(m_estate) << SQLSTR(m_numberInventory) << SQLSTR(m_codeSpace) );
	
	return update;
}

void ADAudioVisual::setValues(XMLResults values)
{
	m_type = values["typeav"];
	m_marksEquipment  = values["marksequipmentav"];
	m_estate  = values["estateav"];
	m_numberInventory  = values["numberinventoryav"];
	m_codeSpace  = values["codespace"];

	if(m_codeSpace ==("null"))
	{
		m_codeSpace ="";
	}

	m_valid = true;
}

QString ADAudioVisual::toXml() const
{
	
}

void ADAudioVisual::fromXml(const QString & xml )
{
	
}

bool ADAudioVisual::isValid() const
{
	return m_valid;
}

QString ADAudioVisual::type() const
{
	return m_type;
}

QString ADAudioVisual::marksEquipment() const
{
	return m_marksEquipment;
}

QString ADAudioVisual::estate() const
{
	return m_estate;
}

QString ADAudioVisual::numberInventory() const
{
	return m_numberInventory;
}

QString ADAudioVisual::codeSpace() const
{
	return m_codeSpace;
}


QString toXml() ;

void fromXml(const QString & xml );

