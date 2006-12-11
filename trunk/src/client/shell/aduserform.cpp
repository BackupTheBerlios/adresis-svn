#include "aduserform.h"
#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QMessageBox>

#include <QGridLayout>
#include <QCheckBox>
#include <QMap>

#include <ddebug.h>

#include "adevent.h"



ADUserForm::ADUserForm(QWidget *parent)
	: ADFormBase("<h1><b>Usuario</b><h1>" , parent)
{
	m_inserter = true;
	setup();
}

ADUserForm::ADUserForm(const ADUser * user, QWidget *parent) : ADFormBase("<h1><b>Ususario</b><h1>" , parent)
{
	
	m_inserter = false;
	setup();
	if(user)
	{
		static_cast<QLineEdit*>(m_inputs[tr("nombre")])->setText( user->name() );
		static_cast<QLineEdit*>(m_inputs[tr("codigo")])->setText(user->code());
		static_cast<QLineEdit*>(m_inputs[tr("codigo")])->setEnabled(false);
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
	QGroupBox *container = new QGroupBox("Informacion" );
	vBLayout->addWidget(container, Qt::AlignVCenter);
	
	QGridLayout *layout = new QGridLayout;
	container->setLayout(layout);
	QLabel *label;
	QLineEdit *edits;
	QStringList titles, titles2;
	
	titles << tr("Nombre") << tr("Codigo") << tr("Login") << tr("Password");
	
	QValidator *validator;
	QRegExp rx("\\S\\D*");
	v = new QRegExpValidator(rx, this);
	validator = v;
	
	label = new QLabel(titles[0] );
	layout->addWidget(label, 0, 0);
	edits = new QLineEdit();
	edits->setMaxLength(50);	//Se le coloca maximo 30 caracteres al nombre, dado que este es el tamaño maximo en la BD.
	edits->setValidator(validator);
	layout->addWidget(edits, 0, 1);
	m_inputs.insert(titles[0].toLower () , edits);
	
	
	rx.setPattern("\\d{7}");
	v = new QRegExpValidator(rx, this);
	validator = v;
	
	label = new QLabel(titles[1] );
	layout->addWidget(label, 1, 0);
	edits = new QLineEdit();
	edits->setMaxLength(7);
	edits->setValidator(validator);
	layout->addWidget(edits, 1, 1);
	m_inputs.insert(titles[1].toLower () , edits);
	
	rx.setPattern("\\w{8}");
	v = new QRegExpValidator(rx, this);
	validator = v;
	
	label = new QLabel(titles[2] );
	layout->addWidget(label, 2, 0);
	edits = new QLineEdit();
	edits->setMaxLength(8);
	edits->setValidator(validator);
	layout->addWidget(edits, 2, 1);
	m_inputs.insert(titles[2].toLower () , edits);
	
	
	label = new QLabel(titles[3] );
	layout->addWidget(label, 3, 0);
	edits = new QLineEdit();
	layout->addWidget(edits, 3, 1);
	m_inputs.insert(titles[3].toLower () , edits);
	
	
	
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
	if(valite())
	{
		ADUser user( static_cast<QLineEdit*>(m_inputs[tr("nombre")])->text(), static_cast<QLineEdit*>(m_inputs[tr("codigo")])->text(), static_cast<QLineEdit*>(m_inputs[tr("login")])->text(), static_cast<QLineEdit*>(m_inputs[tr("password")])->text(), ADPermission(), m_permission->currentIndex ());
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
		emit requestClose();
	}
	
}

bool ADUserForm::valite()
{
	int pos = 0;
	bool isValid = true;
	QString name = static_cast<QLineEdit*>(m_inputs[tr("nombre")])->text();
	QString code = static_cast<QLineEdit*>(m_inputs[tr("codigo")])->text();
	QString login = static_cast<QLineEdit*>(m_inputs[tr("login")])->text();
	QString pass = static_cast<QLineEdit*>(m_inputs[tr("password")])->text();
	const QValidator * namev = static_cast<QLineEdit*>(m_inputs[tr("nombre")])->validator();
	const QValidator * codev = static_cast<QLineEdit*>(m_inputs[tr("codigo")])->validator();
	const QValidator * loginv = static_cast<QLineEdit*>(m_inputs[tr("login")])->validator();
	
	
	if(name.isEmpty() || code.isEmpty() || login.isEmpty() || pass.isEmpty() || (namev->validate(name, pos) != QValidator::Acceptable) || (codev->validate(code, pos) != QValidator::Acceptable) || (loginv->validate(login, pos) != QValidator::Acceptable) )
	{
		QMessageBox::information ( 0 , "ERROR", "Revise que alguno de los campos no sea incorrecto, por ejemplo:\nNo se encuentra en blanco algun campo\nEl codigo es de 7 numeros\nEl login es de 8 caracteres alfabeticos y no tiene espacios en blanco", 0);
		isValid=false;
	}
	
	return isValid;
}

