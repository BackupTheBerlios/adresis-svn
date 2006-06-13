#ifndef ADUSERFORM_H
#define ADUSERFORM_H

#include "adformbase.h"
#include <QMap>
#include "global.h"
#include "adpermissionsview.h"

/**
@author Charly Aguirre Manzano,033091
*/
class ADUserForm : public ADFormBase
{
	Q_OBJECT
	public:
		ADUserForm(QWidget *parent = 0);
		~ADUserForm();
		
	private:
		QMap<QString, QWidget*> m_inputs;
		ADPermissionsView *m_permission;
		
	signals:
		void requestInsertUser(const QString& name, const QString& code,const QString& login,const QString& passwd,QMap<Logic::TypeModule, bool> permissions  );

	public slots:
		void emitInsertUser();
		
};

#endif
