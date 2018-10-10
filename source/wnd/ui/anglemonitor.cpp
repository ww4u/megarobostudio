#include "anglemonitor.h"
#include "ui_anglemonitor.h"
#include "anglewidget.h"
#include "../widget/megamessagebox.h"

AngleMonitor::AngleMonitor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AngleMonitor)
{
    ui->setupUi(this);

    connect( &mTimer, SIGNAL(timeout()),
             this, SLOT(slot_timeout()));
    connect( this, SIGNAL(sig_anglewidget_changed()),
             this, SLOT(slot_anglewidget_changed()) );

    m_pModel = NULL;
    mDataId = 0;

    ui->comboBox->setEnabled( false );
    ui->comboBox->setVisible( false );
    ui->labelSubItem->setVisible( false );

    slot_anglewidget_changed();
}

AngleMonitor::~AngleMonitor()
{
    delete ui;
}

void AngleMonitor::setModel( mcModel *pModel )
{
    Q_ASSERT( NULL != pModel );
    m_pModel = pModel;
}

void AngleMonitor::setConnections( const QStringList &conn )
{
    mConns = conn;

    AngleWidget *pAngleWig;
    foreach( QWidget *pWig, mAngleWidgets )
    {
        pAngleWig = (AngleWidget*)pWig;

        pAngleWig->setConnections( conn );
    }
}

void AngleMonitor::setDataId( int id )
{
    mDataId = id;
    if ( ui->comboBox->isEnabled() )
    { ui->comboBox->setValue( id ); }
}

void AngleMonitor::setDataIds( QMap<int, QString> &subItem )
{
    QMap<int, QString>::const_iterator i = subItem.constBegin();
    while (i != subItem.constEnd())
    {
        ui->comboBox->addItem( i.value(), i.key() );
        i++;
    }

    ui->comboBox->setEnabled( true );
    ui->comboBox->setVisible( true );
    ui->labelSubItem->setVisible( true );
}

void  AngleMonitor::addAnAngle()
{
    AngleWidget *pWidget = new AngleWidget();
    if ( NULL == pWidget )
    { return; }

    mAngleWidgets.append( pWidget );
    emit sig_anglewidget_changed();

    ui->verticalLayout->addWidget( pWidget );

    connect( pWidget, SIGNAL(signal_close(QWidget*)),
             this, SLOT(slot_anglewidget_close(QWidget*)) );

    pWidget->setConnections( mConns );
}

void AngleMonitor::sampleProc()
{
    AngleWidget *pAngleWig;

    int err;
    float val;
    foreach( QWidget *pWig, mAngleWidgets )
    {
        pAngleWig = (AngleWidget*)pWig;

        if ( pAngleWig->connection().length() > 0 )
        {}
        else
        { logDbg();continue; }

        //! sample
        val = sampleUnit( pAngleWig->connection(), err );
        if ( err != 0 )
        {
            sysError( tr("sample fail") );
        }
        else
        {
            pAngleWig->setValue( val );
        }
    }
}

float AngleMonitor::sampleUnit( const QString &conn,
                                int &err )
{
    Q_ASSERT( NULL != m_pModel );

    //! convert to devcie
    int subAx;
    MegaDevice::deviceMRQ *pMrq;
    pMrq = m_pModel->m_pInstMgr->findDevice( conn, &subAx );
    if ( NULL == pMrq )
    {
        err = -1;
        return 0;
    }

    err = 0;

    return pMrq->getSensor( subAx, mDataId );
}

void AngleMonitor::on_btnAdd_clicked()
{
    addAnAngle();
}

void AngleMonitor::slot_anglewidget_close( QWidget * pWig )
{
    mAngleWidgets.removeAll( pWig );
    emit sig_anglewidget_changed();

    ui->verticalLayout->removeWidget( pWig );

    pWig->close();
}

void  AngleMonitor::slot_timeout()
{
    if ( isVisible() )  //! only visible to proc
    { sampleProc(); }
}

void AngleMonitor::on_spinInterval_valueChanged(int arg1)
{
    on_checkBox_clicked( ui->checkBox->isChecked() );
}

void AngleMonitor::on_checkBox_clicked(bool checked)
{
    if ( checked )
    {}
    else
    {
        mTimer.stop();
        return;
    }

    //! start timer
    if ( ui->spinInterval->value() > 0 )
    { mTimer.start( ui->spinInterval->value() ); }
    else
    { mTimer.stop(); }
}

void AngleMonitor::on_toolButton_clicked()
{
    MegaDeleteAffirmMessageBox msgBox;
    int ret = msgBox.exec();
    if ( ret == QMessageBox::Ok )
    {
        foreach( QWidget *pWig, mAngleWidgets )
        {
            ui->verticalLayout->removeWidget( pWig );
            pWig->close();
        }

        mAngleWidgets.clear();
        emit sig_anglewidget_changed();
    }
}

void AngleMonitor::slot_anglewidget_changed()
{
    ui->toolButton->setEnabled( mAngleWidgets.size() > 0 );
}

void AngleMonitor::on_comboBox_activated(int index )
{
    mDataId = ui->comboBox->value();
}
