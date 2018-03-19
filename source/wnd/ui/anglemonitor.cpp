#include "anglemonitor.h"
#include "ui_anglemonitor.h"
#include "anglewidget.h"
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

void  AngleMonitor::addAnAngle()
{
    AngleWidget *pWidget = new AngleWidget();

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
        { continue; }

        //! sample

        val = sampleUnit( pAngleWig->connection(), err );

        if ( err != 0 )
        {
            sysError("sample fail");
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
    return pMrq->getAngle( subAx );
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
    qDebug()<<__FUNCTION__<<__LINE__;
    sampleProc();
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
    QMessageBox msgBox;
    msgBox.setText( tr("Sure to delete all?") );
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel );
    msgBox.setDefaultButton(QMessageBox::Ok);
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

//    adjustSize();
    ui->verticalLayout->update();
}
