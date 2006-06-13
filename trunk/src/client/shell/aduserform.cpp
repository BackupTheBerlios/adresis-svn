#include "aduserform.h"
#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>

#include <QGridLayout>
#include <QCheckBox>

ADUserForm::ADUserForm(QWidget *parent)
 : QWidget(parent)
{
	QHBoxLayout *vBLayout = new QHBoxLayout(this);
		
	QGroupBox *container = new QGroupBox( this );
	vBLayout->addWidget(container);
	
	QGridLayout *layout = new QGridLayout(container);
	container->setLayout(layout);
	QLabel *label;
	QLineEdit *edits;
	QStringList titles, titles2;
	QCheckBox *checkB;
	
	titles << tr("Codigo") << tr("Nombre") << tr("Login") << tr("Clave");
	for(int i = 0; i < titles.count(); i++)
	{
		label = new QLabel(titles[i] );
		layout->addWidget(label, i, 0);
		edits = new QLineEdit(container);
		layout->addWidget(edits, i, 1);
		m_inputs.insert(titles[i] , edits);
	}
	
	container = new QGroupBox("Permisos", this );
	vBLayout->addWidget(container);
	layout = new QGridLayout(container);
	titles2 << tr("Usuarios") << tr("Espacio") << tr("Ayuda") << tr("Reserva");
	for(int i = 0; i < titles2.count(); i++)
	{
		checkB = new QCheckBox(titles2[i], container);
		layout->addWidget(checkB, i, 0);
	}
}


ADUserForm::~ADUserForm()
{
}


