#ifndef ADUSERFORM_H
#define ADUSERFORM_H

#include <QWidget>
#include <QMap>


/**
@author Charly Aguirre Manzano,033091
*/
class ADUserForm : public QWidget
{
	Q_OBJECT
	public:
		ADUserForm(QWidget *parent = 0);
		~ADUserForm();
		
	private:
		QMap<QString, QWidget*> m_inputs;
};

#endif
