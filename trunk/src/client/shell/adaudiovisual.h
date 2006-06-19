#ifndef ADAUDIOVISUAL_H
#define ADAUDIOVISUAL_H

#include <QMap>
#include "adobject.h"

/**
@author Charly Aguirre Manzano,033091
*/
class ADAudioVisual: public ADObject
{
	public:
		ADAudioVisual();
		ADAudioVisual(const QString & type, const QString & marksEquipment, const QString & estate, const QString & numberInventory, const QString & codeSpace);
		~ADAudioVisual();
		ADInsertPackage insertPackage();
		ADUpdatePackage updatePackage();
		void setValues(XMLResults values);
		bool isValid();
		
		QString type();
		QString marksEquipment();
		QString estate();
		QString numberInventory();
		QString codeSpace();
		
	private:
		QString m_type;
		QString m_marksEquipment;
		QString m_estate;
		QString m_numberInventory;
		QString m_codeSpace;
		bool m_valid;
		
};

#endif


