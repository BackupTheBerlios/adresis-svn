#ifndef ADAUDIOVISUAL_H
#define ADAUDIOVISUAL_H


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
		
		/**
		 * deprecada
		 * @return 
		 */
		ADInsertPackage insertPackage();
		/**
		 * deprecada
		 *
		 * @return 
		 */
		ADUpdatePackage updatePackage();
		
		QString toXml() const;
		void fromXml(const QString & xml );
		
		void setValues(XMLResults values);
		bool isValid() const;
		
		QString type() const;
		QString marksEquipment() const;
		QString estate() const;
		QString numberInventory() const;
		QString codeSpace() const;
		
	private:
		QString m_type;
		QString m_marksEquipment;
		QString m_estate;
		QString m_numberInventory;
		QString m_codeSpace;
		bool m_valid;
		
};

#endif

