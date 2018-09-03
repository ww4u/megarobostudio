#include "roboconnection.h"
#include "ui_roboconnection.h"

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

void roboConnection::setRoboNames( const QStringList &strList )
{
    ui->cmbRobotName->clear();
    ui->cmbRobotName->addItems( strList );
}
void roboConnection::setCurrentName( const QString &str )
{
    ui->cmbRobotName->setCurrentText( str );
}
void roboConnection::setCurrentName( const int index )
{
    ui->cmbRobotName->setCurrentIndex( index );
}

int roboConnection::page()
{ return ui->widget->page(); }

QComboBox *roboConnection::getCombName()
{ return ui->cmbRobotName; }

QComboBox *roboConnection::getCombPage()
{ return ui->widget->getComb(); }
