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

ADUserForm::ADUserForm(const ADUser * user, QWidget *parent) : ADFormBase("<h1><b>User</b><h1>" , parent)
{
	
	m_inserter = false;
	setup();
	if(user)
	{
		static_cast<QLineEdit*>(m_inputs[tr("name")])->setText( user->name() );
		static_cast<QLineEdit*>(m_inputs[tr("code")])->setText(user->code());
		static_cast<QLineEdit*>(m_inputs[tr("code")])->setEnabled(false);
		static_cast<QLineEdit*>(m_inputs[tr("login")])->setText(user->login());
		m_permission->setCurrentIndex(user->rol());
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
	
	m_permission  =new QComboBox(this);
	QMap<QString, int> rols = ADUser::rols();
	QMap<QString, int>::iterator it = rols.begin();
	while(it != rols.end())
	{
		m_permission-> insertItem (it.value(), it.key());
		++it;
	}
	vBLayout->addWidget(m_permission);
	setForm(base);
	connect(this, SIGNAL(requestDone()),this, SLOT(emitEvent()));
}



void ADUserForm::emitEvent()
{
	ADUser user( static_cast<QLineEdit*>(m_inputs[tr("name")])->text(), static_cast<QLineEdit*>(m_inputs[tr("code")])->text(), static_cast<QLineEdit*>(m_inputs[tr("login")])->text(), static_cast<QLineEdit*>(m_inputs[tr("password")])->text(), ADPermission(), m_permission->currentIndex ());
	Logic::Action action;
	if(m_inserter)
	{
		action = Logic::Add;
	}
	else
	{
		action = Logic::Update;
	}
	ADEvent event( ADEvent::Client, Logic::Users, action, QVariant::fromValue(&user));
	emit sendEvent(&event);
	
}

