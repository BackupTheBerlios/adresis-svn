#ifndef ADUSERFORM_H
#define ADUSERFORM_H

#include "adformbase.h"
#include <QMap>
#include "global.h"
#include "adpermissionsview.h"
#include "aduser.h"
/**
@author Charly Aguirre Manzano,033091
*/
class ADUserForm : public ADFormBase
{
	Q_OBJECT
	public:
		ADUserForm(QWidget *parent = 0);
		ADUserForm(const ADUser& user, QWidget *parent = 0);
		~ADUserForm();
		void setup();
		
	private:
		QMap<QString, QWidget*> m_inputs;
		ADPermissionsView *m_permission;
		bool m_inserter;
		
	signals:
		void requestInsertUser(const QString& name, const QString& code,const QString& login,const QString& passwd,QMap<Logic::TypeUser, bool> permissions  );
		void requestUpdateUser(const QString& name, const QString& code,const QString& login,const QString& passwd,QMap<Logic::TypeUser, bool> permissions  );
		
	public slots:
		void emitInsertUser();
};

#endif
