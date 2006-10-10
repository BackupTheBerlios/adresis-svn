#include "aduserform.h"
#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>

#include <QGridLayout>
#include <QCheckBox>
#include <QMap>

#include <ddebug.h>

#include "adevent.h"



ADUserForm::ADUserForm(QWidget *parent)
	: ADFormBase("<h1><b>User</b><h1>" , parent)
{
	m_inserter = true;
	setup();
}

ADUserForm::ADUserForm(const ADUser& user, QWidget *parent) : ADFormBase("<h1><b>User</b><h1>" , parent)
{
	D_FUNCINFO;
	setup();
	m_inserter = false;
	if(user.isValid())
	{
		static_cast<QLineEdit*>(m_inputs[tr("nombre")])->setText(user.name());
		static_cast<QLineEdit*>(m_inputs[tr("codigo")])->setText(user.code());
		static_cast<QLineEdit*>(m_inputs[tr("login")])->setText(user.login());
		static_cast<QLineEdit*>(m_inputs[tr("clave")])->setText(user.passwd());
// 		m_permission->setPermissions(user.permissions());
		static_cast<QLineEdit*>(m_inputs[tr("login")])->setReadOnly ( true );
	}
}

ADUserForm::~ADUserForm()
{
	
}


void ADUserForm::setup()
{
	QWidget * base = new QWidget();
	QVBoxLayout *vBLayout = new QVBoxLayout(base);
	QGroupBox *container = new QGroupBox("Informacion" );
	vBLayout->addWidget(container, Qt::AlignVCenter);
	
	QGridLayout *layout = new QGridLayout;
	container->setLayout(layout);
	QLabel *label;
	QLineEdit *edits;
	QStringList titles, titles2;
	QCheckBox *checkB;
	
	titles << tr("Nombre") << tr("Codigo") << tr("Login") << tr("Clave");
	for(int i = 0; i < titles.count(); i++)
	{
		label = new QLabel(titles[i] );
		layout->addWidget(label, i, 0);
		edits = new QLineEdit();
		layout->addWidget(edits, i, 1);
		m_inputs.insert(titles[i].toLower () , edits);
	}
	static_cast<QLineEdit*>( m_inputs[tr("clave")])->setEchoMode( QLineEdit::Password );
	
	m_permission  =new ADPermissionsView();
	vBLayout->addWidget(m_permission);
	setForm(base);
	connect(this, SIGNAL(requestDone()),this, SLOT(emitInsertUser()));
}



void ADUserForm::emitInsertUser()
{
	if(m_inserter)
	{
		QMap<QString, QVariant> map;
		map.insert(tr("nombre"), static_cast<QLineEdit*>(m_inputs[tr("nombre")])->text());
		map.insert(tr("codigo"), static_cast<QLineEdit*>(m_inputs[tr("codigo")])->text());
		map.insert(tr("login"), static_cast<QLineEdit*>(m_inputs[tr("login")])->text());
		map.insert(tr("clave"), static_cast<QLineEdit*>(m_inputs[tr("clave")])->text());
		map.insert(tr("permisos"), &m_permission->permissions() );
		
		
// 		ADEvent insertEvent( Logic::Users, ADEvent::Add, map  );
// 		emit sendEvent( &insertEvent );
		
// 	emit requestInsertUser(
// 			static_cast<QLineEdit*>(m_inputs[tr("nombre")])->text(),
// 			static_cast<QLineEdit*>(m_inputs[tr("codigo")])->text(),
// 			static_cast<QLineEdit*>(m_inputs[tr("login")])->text(),
// 			static_cast<QLineEdit*>(m_inputs[tr("clave")])->text(),
// 			m_permission->permissions()
// 	);
	}
	else
	{
// 		emit requestUpdateUser(
// 			static_cast<QLineEdit*>(m_inputs[tr("nombre")])->text(),
// 			static_cast<QLineEdit*>(m_inputs[tr("codigo")])->text(),
// 			static_cast<QLineEdit*>(m_inputs[tr("login")])->text(),
// 			static_cast<QLineEdit*>(m_inputs[tr("clave")])->text(),
// 			m_permission->permissions()
// 			      );
	}
}



