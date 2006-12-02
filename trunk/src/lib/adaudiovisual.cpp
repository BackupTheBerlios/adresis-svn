#include "adaudiovisual.h"
#include "ddebug.h"

ADAudioVisual::ADAudioVisual()
	: ADObject()
{
	m_valid = false;
}

ADAudioVisual::ADAudioVisual(const QString & type, const QString & marksEquipment, const QString & state, const QString & numberInventory, const QString & codeSpace) 
	: ADObject(), m_type(type), m_marksEquipment(marksEquipment), m_estate(state), m_numberInventory(numberInventory), m_codeSpace(codeSpace)
{
	m_valid = true;
}

ADAudioVisual::~ADAudioVisual()
{
}

QDomElement ADAudioVisual::toXml(QDomDocument &doc) const
{
	QDomElement root = doc.createElement("audiovisual");
	
	root.setAttribute( "typeav", m_type );
	root.setAttribute( "marksequipmentav", m_marksEquipment );
	root.setAttribute( "estateav", m_estate );
	root.setAttribute( "numberInventoryav", m_numberInventory );
	root.setAttribute( "codespace", m_codeSpace );
	
	return root;
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

QString ADAudioVisual::state() const
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
