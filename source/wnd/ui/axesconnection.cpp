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

void axesConnection::changeEvent( QEvent *event )
{
    QWidget::changeEvent( event );

    if ( event->type() == QEvent::LanguageChange )
    { ui->retranslateUi( this ); }
}

void axesConnection::setDeviceNames( const QMap<QString,int> &map )
{
    ui->cmbDeviceName->clear();

    QMapIterator< QString, int> iter(map);
    while( iter.hasNext() )
    {
        iter.next();

        ui->cmbDeviceName->addItem( iter.key(), iter.value() );
    }
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

int axesConnection::page()
{ return ui->comboBox->currentIndex(); }

QComboBox *axesConnection::getCombName()
{
    return ui->cmbDeviceName;
}

QComboBox *axesConnection::getCombCH()
{
    return ui->cmbDeviceAxes;
}

QComboBox *axesConnection::getCombPage()
{
    return ui->comboBox;
}

void axesConnection::on_cmbDeviceName_currentIndexChanged(int index)
{
    ui->cmbDeviceAxes->clear();

    //! current user data
    if ( ui->cmbDeviceName->currentData().isValid() )
    {
        int axCnt = ui->cmbDeviceName->currentData().toInt();
        for ( int i = 0; i < axCnt; i++ )
        {
            ui->cmbDeviceAxes->addItem( QString("CH%1").arg(i+1) );
        }
    }

    //! find the regions

}
