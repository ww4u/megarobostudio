#include "axesconnection.h"
#include "ui_axesconnection.h"

axesConnection::axesConnection(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::axesConnection)
{
    ui->setupUi(this);
}

axesConnection::~axesConnection()
{
    delete ui;
}

void axesConnection::setDeviceNames( const QStringList &strList )
{
    ui->cmbDeviceName->clear();
    ui->cmbDeviceName->addItems( strList );
}
void axesConnection::setCurrentName( const QString &str )
{
    ui->cmbDeviceName->setCurrentText( str );
}
void axesConnection::setCurrentName( const int index )
{
    ui->cmbDeviceName->setCurrentIndex( index );
}

void axesConnection::setDeviceCHs( const QStringList &strList )
{
    ui->cmbDeviceAxes->clear();
    ui->cmbDeviceAxes->addItems( strList );
}
void axesConnection::setCurrentCH( const QString &str )
{
    ui->cmbDeviceAxes->setCurrentText( str );
}
void axesConnection::setCurrentCH( const int index )
{
    ui->cmbDeviceAxes->setCurrentIndex( index );
}

QComboBox *axesConnection::getCombName()
{
    return ui->cmbDeviceName;
}

QComboBox *axesConnection::getCombCH()
{
    return ui->cmbDeviceAxes;
}
