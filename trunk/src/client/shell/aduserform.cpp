#include "aduserform.h"
#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>

#include <QGridLayout>
#include <QCheckBox>
#include <ddebug.h>

ADUserForm::ADUserForm(QWidget *parent)
	: ADFormBase("<h1><b>User</b><h1>" , parent)
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
		layout->addWidget(label, i, 0/*, Qt::AlignHCenter*/);
		edits = new QLineEdit();
		layout->addWidget(edits, i, 1/*, Qt::AlignHCenter*/);
		m_inputs.insert(titles[i].toLower () , edits);
	}
	static_cast<QLineEdit*>( m_inputs[tr("clave")])->setEchoMode( QLineEdit::Password );
	
	m_permission  =new ADPermissionsView();
	vBLayout->addWidget(m_permission);
	setForm(base);
	connect(this, SIGNAL(requestDone()),this, SLOT(emitInsertUser()));
}

ADUserForm::~ADUserForm()
{
}

void ADUserForm::emitInsertUser()
{
	
	emit requestInsertUser(
			static_cast<QLineEdit*>(m_inputs[tr("nombre")])->text(),
			static_cast<QLineEdit*>(m_inputs[tr("codigo")])->text(),
			static_cast<QLineEdit*>(m_inputs[tr("login")])->text(),
			static_cast<QLineEdit*>(m_inputs[tr("clave")])->text(),
			m_permission->permissions()
	);
}

