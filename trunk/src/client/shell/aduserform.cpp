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

		
	}
}

ADUserForm::~ADUserForm()
{
	
}


void ADUserForm::setup()
{
	QWidget * base = new QWidget();
	QVBoxLayout *vBLayout = new QVBoxLayout(base);
	QGroupBox *container = new QGroupBox("Information" );
	vBLayout->addWidget(container, Qt::AlignVCenter);
	
	QGridLayout *layout = new QGridLayout;
	container->setLayout(layout);
	QLabel *label;
	QLineEdit *edits;
	QStringList titles, titles2;
	QCheckBox *checkB;
	
	titles << tr("Name") << tr("Code") << tr("Login") << tr("Password");
	for(int i = 0; i < titles.count(); i++)
	{
		label = new QLabel(titles[i] );
		layout->addWidget(label, i, 0);
		edits = new QLineEdit();
		layout->addWidget(edits, i, 1);
		m_inputs.insert(titles[i].toLower () , edits);
	}
	static_cast<QLineEdit*>( m_inputs[tr("password")])->setEchoMode( QLineEdit::Password );
	
	m_permission  =new ADPermissionsView();
	vBLayout->addWidget(m_permission);
	setForm(base);
	connect(this, SIGNAL(requestDone()),this, SLOT(emitInsertUser()));
}



void ADUserForm::emitInsertUser()
{
	ADUser user( static_cast<QLineEdit*>(m_inputs[tr("name")])->text(), static_cast<QLineEdit*>(m_inputs[tr("code")])->text(), static_cast<QLineEdit*>(m_inputs[tr("login")])->text(), static_cast<QLineEdit*>(m_inputs[tr("password")])->text(), m_permission->permissions());
	
	ADEvent insertUser( ADEvent::Client, Logic::Users, Logic::Add, QVariant::fromValue(&user));
	emit sendEvent(&insertUser);
}



