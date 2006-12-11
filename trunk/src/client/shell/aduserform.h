
#ifndef ADUSERFORM_H
#define ADUSERFORM_H

#include <QMap>
#include <QComboBox>
#include <QRegExpValidator>

#include "adformbase.h"
#include "global.h"
#include "adpermissionsview.h"
#include "aduser.h"
/**
 * @author Jorge Cuadrado
*/
class ADUserForm : public ADFormBase
{
	Q_OBJECT
	public:
		ADUserForm(QWidget *parent = 0);
		ADUserForm(const ADUser * user, QWidget *parent = 0);
		~ADUserForm();
		
		void setup();
		
	private:
		QMap<QString, QWidget*> m_inputs;
		QComboBox *m_permission;
		bool m_inserter;
		QRegExpValidator *v;
		bool valite();
		void clearFields();
		
	public slots:
		void emitEvent();
		
};

#endif
