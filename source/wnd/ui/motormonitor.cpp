#include "motormonitor.h"
#include "ui_motormonitor.h"

#include "motorstate.h"
#include "../../device/mrq/deviceMRQ.h"

MotorMonitor::MotorMonitor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MotorMonitor)
{
    ui->setupUi(this);

    m_pFlowLayout = new FlowLayout();
    ui->scrollAreaWidgetContents_2->setLayout( m_pFlowLayout );

    m_pModel = NULL;
}

MotorMonitor::~MotorMonitor()
{
    delete ui;
}

void MotorMonitor::setModel( mcModel *pModel )
{
    Q_ASSERT( NULL != pModel );
    m_pModel = pModel;
}

void MotorMonitor::setMonitors( const QStringList &conns )
{
    clearMonitors();

    mMonitors = conns;

    //! create each item
    foreach( QString str, mMonitors )
    { addMonitor( str ); }
}
QStringList MotorMonitor::monitors()
{ return mMonitors; }

void MotorMonitor::clearMonitors()
{
    foreach( MotorState *pStat, mMotors )
    {
        Q_ASSERT( NULL != pStat );
        m_pFlowLayout->removeWidget( pStat );
        pStat->close();     //! delete on close
    }

    //! clear
    mMotors.clear();
}

void MotorMonitor::addMonitor( const QString &str )
{
    if ( str.length() < 1 )
    { return; }

    MotorState *pMotorStat = new MotorState(this);
    if ( NULL == pMotorStat )
    { return; }

    pMotorStat->setName( str );

    m_pFlowLayout->addWidget( pMotorStat );
    mMotors.append( pMotorStat );
}

MotorState *MotorMonitor::getMotor( const QString &name, int ax )
{
    //! format to full name
    QString fullName = QString("CH%1@%2").arg(ax+1).arg(name);

    foreach( MotorState *pWig, mMotors )
    {
        if ( pWig->name() == fullName )
        { return pWig; }
    }

    return NULL;
}

void MotorMonitor::motorStateChanged( const QString &name,
                                      int axes,
                                      RoboMsg msg )
{
    Q_ASSERT( NULL != m_pModel );

    //! get widget
    MotorState *pWig = getMotor( name, axes );
    if ( NULL == pWig )
    { return; }

    //! now for status
    QByteArray ary;
    ary = msg.at(3).toByteArray();

    //! get status
    int stat = ary.at(4);
    int page = ary.at(3);
    QString statStr = MRQ_MOTION_STATE_2_toString( (MRQ_MOTION_STATE_2)stat );

    pWig->setState( statStr, page );

    //! get progress
    if ( stat == MRQ_MOTION_STATE_2_CALCING )
    {
        //! get range
        MegaDevice::deviceMRQ *pDev;

        pDev = m_pModel->m_pInstMgr->findDevice( name, axes );
        if ( NULL == pDev )
        { return; }

        quint16 prog;
        int ret;
        ret = pDev->getMOTIONPLAN_REMAINPOINT( (byte)axes,
                                               (MRQ_MOTION_SWITCH_1)page,
                                                &prog );
        if ( ret != 0 )
        { return; }

        int capMax;
        capMax = pDev->getTpvCap( tpvRegion( axes, page) );

        pWig->setProgress( page, 0, capMax, prog );
        pWig->setProgress( page, true );
    }
    else
    {
        pWig->setProgress( page, false );
    }
}

void MotorMonitor::slot_net_event(
                     const QString &name,       //! device name
                     int axes,                  //! device ax
                     RoboMsg msg )
{
    //! interrupt
    if ( msg.getMsg() != e_interrupt_occuring )
    { return; }

    if ( isVisible() )
    {}
    else
    { return; }

    //! status
    int eId;
    eId = msg.at(0).toInt();
    if ( eId == event_status )
    { }
    else
    { return; }

    //! auto view
    if ( m_pModel->mSysPref.mbAutoStatusView && isHidden() )
    { show(); }

    motorStateChanged( name, axes, msg );
}

