#include "roboconnection.h"
#include "ui_roboconnection.h"
#include "../../include/mydebug.h"
roboConnection::roboConnection(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::roboConnection)
{
    ui->setupUi(this);
}

roboConnection::~roboConnection()
{
    delete ui;
}

void roboConnection::changeEvent(QEvent * event)
{
    QWidget::changeEvent( event );

    if (event->type() == QEvent::LanguageChange)
    { ui->retranslateUi( this ); }
}

void roboConnection::setRoboNames( const QStringList &strList )
{
    ui->cmbRobotName->clear();
    ui->cmbRobotName->addItems( strList );
}
void roboConnection::setCurrentName( const QString &str )
{logDbg()<<str;
    ui->cmbRobotName->setCurrentText( str );
}
void roboConnection::setCurrentName( const int index )
{logDbg()<<index;
    ui->cmbRobotName->setCurrentIndex( index );
}

int roboConnection::page()
{ return ui->widget->page(); }

QComboBox *roboConnection::getCombName()
{ return ui->cmbRobotName; }

QComboBox *roboConnection::getCombPage()
{ return ui->widget->getComb(); }
