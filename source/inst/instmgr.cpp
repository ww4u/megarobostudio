
#include <QHostInfo>
#include "instmgr.h"
#include "../../model/mcmodel.h"


#define MRH_T_PORT  2

namespace MegaDevice
{

INTRThread::INTRThread(QObject *parent) : QThread( parent )
{}

INTRThread::~INTRThread()
{}

void INTRThread::slot_event( eventId id, frameData dat )
{
    receiveCache::decEvent();       //! dec
    emit sig_event( id, dat );
}

//! instmgr
InstMgr *InstMgr::_mpMgr = NULL;

InstMgr::InstMgr( QObject *parent ) : instServer( parent )
{
    m_pMainModel = NULL;
    m_pMainShell = NULL;

    m_pINTR = new INTRThread();
    Q_ASSERT( NULL != m_pINTR );
    m_pINTR->start( QThread::HighPriority );
}

InstMgr::~InstMgr()
{
    m_pINTR->terminate();
    m_pINTR->wait();
    delete m_pINTR;

    gc();

    receiveCache::clearFrameEvent();
}

InstMgr *InstMgr::proxy()
{
    if ( NULL == InstMgr::_mpMgr )
    {
        InstMgr::_mpMgr = new InstMgr();
    }
    else
    {}

    return InstMgr::_mpMgr;
}

void InstMgr::free()
{
    if ( NULL != InstMgr::_mpMgr )
    {
        delete InstMgr::_mpMgr;
    }
}

//! to some device
void InstMgr::dataIn(  QTcpSocket *socket,
                       const QString &name,
                       QByteArray &ary )
{
//    logDbg()<<ary.length()<<name;
    //! find the name
    scpiShell *pShell = findShell( name );
    if ( NULL == pShell )
    { return; }
//    logDbg()<<ary.length();

    //! wait lpc idle
    RoboMsgQueue::waitIdle();

    pShell->setObjPara( name, socket );

    pShell->write( ary.data(), ary.length() );

    int retSize = pShell->size();
    if ( retSize > 0 )
    {
        char retData[ retSize + 1];
        retData[ retSize ] = 0;
        int rdSize;
        rdSize = pShell->read( retData, retSize );

        dataOut( socket, retData, rdSize );
//        logDbg()<<rdSize<<retData;
//        for ( int i = 0; i < rdSize; i++ )
//        { logDbg()<<QString::number( retData[i],16); }
    }
//    logDbg()<<retSize;
}

void InstMgr::setMainModel( mcModel *pModel )
{
    Q_ASSERT( NULL != pModel );

    m_pMainModel = pModel;
}

void InstMgr::setMainShell( scpiShell *pShell )
{
    Q_ASSERT( NULL != pShell );

    m_pMainShell = pShell;

    m_pMainShell->open();
}

//! \todo only one time
int InstMgr::probeBus()
{
    Q_ASSERT( NULL != m_pMainModel );
    int ret;
logDbg();
    preProbeBus();
logDbg();
    QThread::msleep( 1000 );        //! USB-CAN can not enumerate on close - open
logDbg();
    ret = probeCanBus();
logDbg();
//    ret = -1;

    postProbeBus();

    if ( ret != 0 )
    { return ret; }

    return 0;
}

int InstMgr::probeCanBus()
{
    CANBus *pNewBus;
    int ret;

    Q_ASSERT( NULL != m_pMainModel );

    //! -T
    int devCount;
    if ( m_pMainModel->mSysPref.mPort == MRH_T_PORT )
    { devCount = m_pMainModel->mSysPref.mVisaList.count(); }
    else
    { devCount = m_pMainModel->mSysPref.mDeviceCount; }

    for ( int i = 0; i < devCount; i++ )
    {
        pNewBus = new CANBus();
        if ( NULL == pNewBus )
        { return ERR_ALLOC_FAIL; }

        VRoboList *pRoboList;
        pRoboList = new VRoboList();
        if ( NULL == pRoboList )
        {
            delete pNewBus;
            return ERR_ALLOC_FAIL;
        }

        //! receive cache
        ret = probeCANBus( pNewBus,
                           m_pMainModel->mSysPref.mDeviceId + i,
                           m_pMainModel->mSysPref.mVisaList.value( i, "" ),
                           *pRoboList
                            );
        do
        {
            //! fail
            if ( ret != 0 )
            {
                pNewBus->close();
                delete pNewBus;
                delete pRoboList;
                break;
            }

            //! no item
            if ( pRoboList->size() > 0 )
            { }
            else
            { break; }

            //! success
            {
                //! add the bus
                mCanBuses.append( pNewBus );

                //! add the tree
                mDeviceTree.append( pRoboList );

                //! each robot
                //!
                mDevices.append( *pRoboList );

                //! connect
                connect( pNewBus->receiveProxy(), SIGNAL(sig_event(eventId,frameData)),
                         m_pINTR, SLOT(slot_event(eventId,frameData)) );

            }
        }while( 0 );
    }

    //! assign device id for each device
    quint32 devSig;
    int deviceSeq = 1;

    QStringList seqList;
    QString strDevName;

    //! phy robo
    QList<VRobot*> phyRoboList;
    foreach( VRoboList *pList, mDeviceTree )
    {
        Q_ASSERT( NULL != pList );
        phyRoboList.append( *pList );
    }

    //! match the sn
    foreach( VRobot* pRobo, phyRoboList )
    {
        Q_ASSERT( NULL != pRobo );

        ret = m_pMainModel->mSysPref.findAlias( pRobo->getSN(), strDevName );
        if ( ret == 0 )
        {
            pRobo->setName( strDevName );
            seqList.append( strDevName.toLower() );
        }
    }

    //! pre match
    mcModelObj::obj_type objType = mcModelObj::model_none;
    foreach( VRobot* pRobo, phyRoboList )
    {
        Q_ASSERT( NULL != pRobo );

        //! not set name
        if ( pRobo->getName().length() < 1 )
        {logDbg();
            devSig = pRobo->getSignature();

            //! check
            if ( matchSignature(devSig, strDevName ) )
            {
                deviceSeq = 0;
                while( seqList.contains( strDevName.toLower() ) )
                {
                    //! rst the type
                    if ( pRobo->Type() != objType )
                    {
                        deviceSeq = 0;
                        objType = pRobo->Type();
                    }

                    deviceSeq++;
                    strDevName = QString("%1%2").arg(pRobo->typeString()).arg(deviceSeq);

                    Q_ASSERT( deviceSeq < 256 );

                }

                pRobo->setName( strDevName );
                seqList.append( strDevName.toLower() );
            }
        }
    }

    //! assign the others
    deviceSeq = 0;
    objType = mcModelObj::model_none;
    foreach( VRobot *pRobo, phyRoboList )
    {
        Q_ASSERT( NULL != pRobo );

        //! not set
        if ( pRobo->getName().length() < 1 )
        {
            do
            {
                //! rst the type
                if ( pRobo->Type() != objType )
                {
                    deviceSeq = 0;
                    objType = pRobo->Type();
                }

                deviceSeq++;
                strDevName = QString("%1%2").arg(pRobo->typeString()).arg(deviceSeq);

                Q_ASSERT( deviceSeq < 256 );
                logDbg();
            }while( seqList.contains( strDevName.toLower() ) );

            pRobo->setName( strDevName );
            seqList.append( strDevName.toLower() );
        }
    }

    return 0;
}

//! broadcast
int InstMgr::emergencyStop()
{
    byte bufstp[] = { MRQ_mc_MOTION, MRQ_sc_MOTION_SWITCH, CAN_BROAD_CHAN, MRQ_MOTION_SWITCH_EMERGSTOP, 0 };
    byte bufrst[] = { MRQ_mc_MOTION, MRQ_sc_MOTION_SWITCH, CAN_BROAD_CHAN, MRQ_MOTION_SWITCH_RESET, 0 };
    int ret;

    //! 1. broadcast
    DeviceId broadId( CAN_BROAD_ID );
    //! for each page
    for ( int i = 0; i < x_pages; i++ )
    {
        bufstp[4] = i;
        bufrst[4] = i;
        foreach ( CANBus *pBus, mCanBuses)
        {
            Q_ASSERT( NULL != pBus );
            ret = pBus->doWrite( broadId, bufstp, sizeof(bufstp) );

            ret = pBus->doWrite( broadId, bufrst, sizeof(bufrst) );
        }
    }

    //! 2. request
    ret = requestStates();

    return ret;
}

int InstMgr::hardReset()
{
    byte buf[] = { MRQ_mc_MOTION, MRQ_sc_MOTION_SWITCH, CAN_BROAD_CHAN, MRQ_MOTION_SWITCH_RESET, 0 };
    int ret;

    //! 1. broadcast
    DeviceId broadId( CAN_BROAD_ID );
    for ( int i = 0; i < x_pages; i++ )
    {
        buf[4] = i;
        foreach ( CANBus *pBus, mCanBuses)
        {
            Q_ASSERT( NULL != pBus );
            ret = pBus->doWrite( broadId, buf, sizeof(buf) );
        }
    }

    //! 2. request state
    ret = requestStates();

    return ret;
}

int InstMgr::requestStates()
{
    int ret = 0;
    DeviceId broadId( CAN_BROAD_ID );
    //! request state for each page
    byte stateBuf[]= { MRQ_mc_MOTION, MRQ_sc_MOTION_STATE_Q, CAN_BROAD_CHAN, 0 };

    //! for each page
    for ( byte i = 0; i < 10; i++ )
    {
        stateBuf[3] = i;
        foreach ( CANBus *pBus, mCanBuses)
        {
            Q_ASSERT( NULL != pBus );
            ret = pBus->doWrite( broadId, stateBuf, sizeof(stateBuf) );
        }

        if ( ret != 0 )
        { return ret; }
    }

    return ret;
}

DeviceTree InstMgr::getDeviceTree()
{
    DeviceTree tree;

    tree.append( mDeviceTree );
    tree.append( mFileDeviceTree );

    return tree;
}

//! append device
void InstMgr::appendFileDeviceTree( DeviceTree &devTree )
{
    mFileDeviceTree.append( devTree );

    //! rsrc manage
    foreach( VRoboList *pList, devTree )
    {
        Q_ASSERT( NULL != pList );
        mDevices.append( *pList );

        //! attach inst mgr
        foreach( VRobot *pRobo, *pList )
        {
            Q_ASSERT( NULL != pRobo );
            pRobo->setInstMgr( this );

            //! scpi open
            pRobo->open();
        }

        //! attach bus
        mFileBusList.append( pList->bus() );
    }
}

void InstMgr::clearFileDeviceTree()
{
    foreach( VRoboList *pList, mFileDeviceTree )
    {
        Q_ASSERT( NULL != pList );

        foreach( VRobot *pDev, *pList )
        {
            Q_ASSERT( NULL != pDev );
            delete pDev;
            mDevices.removeAll( pDev );
        }
    }
logDbg();
    delete_all( mFileDeviceTree );
    delete_all( mFileBusList );
}

DeviceTree InstMgr::roboTree( const DeviceTree &devTree )
{
    DeviceTree outTree;

    foreach( VRoboList *pRoboList, devTree )
    {
        Q_ASSERT( NULL != pRoboList );

        //! collect
        VRoboList *pOutList;
        pOutList = new VRoboList();
        Q_ASSERT( NULL != pOutList );

        foreach( VRobot *pDev, *pRoboList )
        {
            Q_ASSERT( NULL != pDev );

            if ( robot_is_robot( pDev->getId()) )
            {
                pOutList->append( pDev );
                pOutList->attachBus( pRoboList->bus() );
            }
        }

        //! export
        if ( pOutList->size() > 0 )
        { outTree.append( pOutList ); }
        else
        { delete pOutList; }
    }

    return outTree;
}

//! delete the out tree
DeviceTree InstMgr::roboTree()
{
    DeviceTree tree1, tree2;
    DeviceTree outTree;

    tree1 = roboTree( mDeviceTree );
    tree2 = roboTree( mFileDeviceTree );

    outTree.append( tree1 );
    outTree.append( tree2 );

    return outTree;
}

QStringList InstMgr::roboResources()
{
    DeviceTree tree;
    tree = roboTree();

    QStringList strList;
    foreach( VRoboList *pRobList, tree )
    {
        Q_ASSERT( NULL != pRobList );
        foreach( VRobot *pDev, *pRobList )
        {
            Q_ASSERT( NULL != pDev );
            strList<<QString("%1@%2").arg( pDev->name() ).arg( pRobList->bus()->name() );
        }
    }

    delete_all( tree );

    return strList;
}

//! \note only debug used
deviceMRQ *InstMgr::getDevice( int id )
{
    return (deviceMRQ *)mDeviceTree.at( 0 )->at( id );
}

INTRThread *InstMgr::getInterruptSource()
{ return m_pINTR; }

VRobot * InstMgr::findRobot( const QString &name, int axesId  )
{
    foreach( VRoboList *pRoboList, mDeviceTree )
    {
        Q_ASSERT( NULL != pRoboList );
        foreach( VRobot * pDev, *pRoboList )
        {
            Q_ASSERT( NULL != pDev );

            if ( pDev->name() == name )
            {
                //! check axes
                if ( axesId >= pDev->axes() )
                { return NULL; }

                return pDev;
            }
        }
    }

    return NULL;
}

VRobot * InstMgr::findRobot( const QString &name, int *pAxes )    //! chx@name
{
    Q_ASSERT( NULL != pAxes );

    QStringList strList = name.split("@", QString::SkipEmptyParts );

    if ( strList.length() < 2 )
    { logDbg()<<name; return NULL; }

    //! try convert the ch id
    if ( strList[0].startsWith( "CH" ) )
    {}
    else
    { logDbg()<<name; return NULL; }

    QString numStr = strList[0].right( strList[0].length() - 2 );/*logDbg()<<numStr;*/
    bool bOk;
    int axesId = numStr.toInt( &bOk );
    if( !bOk )
    { logDbg()<<name; return NULL; }

    if ( axesId < 1 )
    { logDbg()<<name; return NULL; }

    *pAxes = axesId - 1;
    return findRobot( strList[1], axesId - 1 );
}

VRobot * InstMgr::findRobot( const QString &name, const QString &bus )
{
    VRoboList *pList = findBus( bus );

    if ( NULL == pList )
    { return NULL; }

    foreach( VRobot *pRobo, *pList )
    {
        Q_ASSERT( NULL != pRobo );
        if ( pRobo->name() == name )
        { return pRobo; }
    }

    return NULL;
}
VRobot * InstMgr::findRobot( const QString &fullname )
{
    QStringList strList = fullname.split( "@", QString::SkipEmptyParts );
    if ( strList.size() == 0 )
    { return NULL; }
//    logDbg()<<strList;
    Q_ASSERT( strList.size() >  1 );
    return findRobot( strList[0], strList[1] );
}

VRobot * InstMgr::findAbbRobot( const QString &abbName )
{
    VRobot *lRobo;
    foreach( VRoboList *pList, mDeviceTree )
    {
        Q_ASSERT( NULL != pList );

        lRobo = findRobot( abbName, pList->bus()->name() );
        if ( NULL != lRobo )
        { return lRobo; }
    }

    foreach( VRoboList *pList, mFileDeviceTree )
    {

        lRobo = findRobot( abbName, pList->bus()->name() );
        if ( NULL != lRobo )
        { return lRobo; }
    }

    return NULL;
}

VRoboList *InstMgr::findBus( const QString &busName )
{
    foreach( VRoboList *pList, mDeviceTree )
    {
        Q_ASSERT( NULL != pList );
        if ( pList->bus()->name() == busName )
        { return pList; }
    }

    foreach( VRoboList *pList, mFileDeviceTree )
    {
        Q_ASSERT( NULL != pList );
        if ( pList->bus()->name() == busName )
        { return pList; }
    }

    return NULL;
}

VRobot * InstMgr::findRobotBySendId( int sendId, int devId, int axesId )
{
    foreach( VRoboList *pRoboList, mDeviceTree )
    {        
        Q_ASSERT( NULL != pRoboList );
        Q_ASSERT( pRoboList->bus() != NULL );

        if ( pRoboList->bus()->devId() == devId )
        {}
        else
        { continue; }

        foreach( VRobot * pDev, *pRoboList )
        {
            Q_ASSERT( NULL != pDev );

            if ( pDev->canSendId() == sendId )
            { return pDev; }
        }
    }

    return NULL;
}

VRobot * InstMgr::findRobotByRecvId( int recvId, int devId, int axesId )
{
    foreach( VRoboList *pRoboList, mDeviceTree )
    {
        Q_ASSERT( NULL != pRoboList );

        Q_ASSERT( pRoboList->bus() != NULL );

        if ( pRoboList->bus()->devId() == devId )
        {}
        else
        { continue; }

        foreach( VRobot * pDev, *pRoboList )
        {
            Q_ASSERT( NULL != pDev );

            if ( pDev->canRecvId() == recvId )
            { return pDev; }
        }
    }

    return NULL;
}

MegaDevice::deviceMRQ  *InstMgr::findDevice( const QString &name, int axesId )
{
    VRobot *pRobot;

    pRobot = findRobot( name, axesId );
    if ( NULL == pRobot )
    { return NULL; }
    else
    { return (MegaDevice::deviceMRQ *)pRobot; }
}

//! chx@devicename
MegaDevice::deviceMRQ *InstMgr::findDevice( const QString &name, int *pAxes )
{
    Q_ASSERT( NULL != pAxes );

    VRobot *pRobot;

    pRobot = findRobot( name, pAxes );
    if ( NULL == pRobot )
    { return NULL; }
    else
    { return (MegaDevice::deviceMRQ *)pRobot; }
}

QString InstMgr::sendIdToName( int devId, int sendId)
{

    foreach( VRoboList *pRoboList, mDeviceTree )
    {
        Q_ASSERT( NULL != pRoboList );

        Q_ASSERT( NULL != pRoboList->bus() );
        if ( pRoboList->bus()->devId() == devId )
        {}
        else
        { continue; }

        foreach( VRobot * pDev, *pRoboList )
        {
            Q_ASSERT( NULL != pDev );

            if ( robot_is_mrq( pDev->robotId()) )
            {
                deviceMRQ *pMrq;

                //! convert
                pMrq = (deviceMRQ*)pDev;
                if ( NULL == pMrq )
                { logDbg();return QString(); }

                //! check axes
                if ( pMrq->getModel()->mCAN_SENDID == (quint32)sendId )
                { return pDev->name(); }
            }
            else if ( robot_is_mrv( pDev->robotId()) )
            {
                deviceMRV *pMrv;

                //! convert
                pMrv = (deviceMRV*)pDev;
                if ( NULL == pMrv )
                { logDbg();return QString(); }

                //! check axes
                if ( pMrv->getModel()->mCAN_SENDID == (quint32)sendId )
                { return pDev->name(); }
            }
            else
            { continue; }



        }
    }

    return QString();
}

//! for all devices
QStringList InstMgr::getResources()
{
    QStringList resrc;

    //! device
    foreach( VRoboList *pRoboList, mDeviceTree )
    {
        Q_ASSERT( NULL != pRoboList );
        foreach( VRobot * pDev, *pRoboList )
        {
            Q_ASSERT( NULL != pDev );

            resrc<<pDev->getName();
        }
    }

    return resrc;
}

QStringList InstMgr::getChans()     //! chx@devicename
{
    QStringList resrc;
    QString chanName;

    foreach( VRoboList *pRoboList, mDeviceTree )
    {
        Q_ASSERT( NULL != pRoboList );
        foreach( VRobot * pDev, *pRoboList )
        {
            Q_ASSERT( NULL != pDev );
            for ( int i = 0; i < pDev->axes(); i++ )
            {
                chanName = QString("CH%1@%2").arg( i+1).arg( pDev->name() );
                resrc<<chanName;
            }
        }
    }

    return resrc;
}

QStringList InstMgr::resources()
{
    QStringList resrc;

    //! device
    foreach( VRoboList *pRoboList, mDeviceTree )
    {
        Q_ASSERT( NULL != pRoboList );
        foreach( VRobot * pDev, *pRoboList )
        {
            Q_ASSERT( NULL != pDev );

            resrc<<pDev->getName();
        }
    }

    //! robot
    foreach( VRoboList *pRoboList, mFileDeviceTree )
    {
        Q_ASSERT( NULL != pRoboList );
        foreach( VRobot * pDev, *pRoboList )
        {
            Q_ASSERT( NULL != pDev );

            resrc<<pDev->getName();
        }
    }

    return resrc;
}

void InstMgr::setTPVBase( float t, float p, float v )
{
    VRobot::setTpvBase( t, p, v );
}

int InstMgr::openBus()
{


    return 0;
}
int InstMgr::closeBus()
{
    //! can bus
    foreach( CANBus *pBus, mCanBuses )
    {
        Q_ASSERT( NULL != pBus );

        pBus->close();
    }

    return 0;
}

void InstMgr::preProbeBus()
{
    gcPhyBus();
}
void InstMgr::postProbeBus()
{}

int InstMgr::probeCANBus( CANBus *pNewBus,
                          int id,
                          const QString &devRsrc,
                          VRoboList &roboList )
{
    Q_ASSERT( NULL != pNewBus );

    //! check type
    int ret;

    int portDevType[]={VCI_MR_USBCAN,
                       VCI_MR_USBCAN,
                       VCI_MR_LANCAN,
                       VCI_USBCAN2 };

    Q_ASSERT( NULL != m_pMainModel );
    //! bus prop.
    pNewBus->setPId( m_pMainModel->mSysPref.mPort );
    pNewBus->setSpeed( m_pMainModel->mSysPref.mSpeed );
    pNewBus->setWtInterval( m_pMainModel->mSysPref.mInterval );
    pNewBus->setRdTmo( m_pMainModel->mSysPref.mTimeout );
    pNewBus->setRecvTmo( m_pMainModel->mSysPref.mRecvTmo );
    pNewBus->setEnumTmo( m_pMainModel->mSysPref.mEnumerateTimeout );
    pNewBus->setFailTry( m_pMainModel->mSysPref.mFailTryCnt );

    //! open
    Q_ASSERT( m_pMainModel->mSysPref.mPort < sizeof_array(portDevType) );
    ret = pNewBus->open( portDevType[m_pMainModel->mSysPref.mPort],
                         id,
                         0,
                         devRsrc );
    if ( ret != 0 )
    { logDbg()<<ret; return ret; }

    sysProgress( 5, tr("enumerate") );

    //! enumerate
    ret = pNewBus->enumerate( m_pMainModel->mSysPref );
    logDbg()<<ret;
    if ( ret != 0 )
    { logDbg(); return ret; }
    sysProgress( 30, tr("enumerate") );

    //! create the device
    deviceMRQ *pMRQ;
    VRobot *pRobo;

    //! a robo list
//    VRoboList *pRoboList = new VRoboList();
//    Q_ASSERT( NULL != pRoboList );
    roboList.attachBus( pNewBus );

    int uiProgBase = 30;
    float uiProgStep = 0;
    if ( pNewBus->mDevices.size() > 0 )
    { uiProgStep = 60/( pNewBus->mDevices.size()*2); }
    int uiStep = 1;

    //! apply for each id
    foreach( DeviceId * pId, pNewBus->mDevices )
    {
        //! detect device
        {
            pMRQ = new deviceMRQ();
            Q_ASSERT( NULL != pMRQ );

            //! bus config
            pMRQ->setInterval( m_pMainModel->mSysPref.mInterval );
            pMRQ->setTimeout( m_pMainModel->mSysPref.mTimeout );

            pMRQ->attachBus( pNewBus );
            pMRQ->setInstMgr( this );

            //! iter ref the sibling
            ret = pMRQ->applyDeviceId( *pId );
            if ( ret != 0 )
            {
                logDbg()<<ret;
                delete pMRQ;
                return ret;
            }

            delete pMRQ;
        }
    }

    //! iterate
    foreach( DeviceId * pId, pNewBus->mDevices )
    {
        //! detect device
        {
            pMRQ = new deviceMRQ();
            Q_ASSERT( NULL != pMRQ );

            //! bus config
            pMRQ->setInterval( m_pMainModel->mSysPref.mInterval );
            pMRQ->setTimeout( m_pMainModel->mSysPref.mTimeout );

            pMRQ->attachBus( pNewBus );
            pMRQ->setInstMgr( this );

            //! iter ref the sibling
            ret = pMRQ->setDeviceId( *pId );
            if ( ret != 0 )
            {
                logDbg()<<ret;
                delete pMRQ;
                return ret;
            }

            //! get info
            MRQ_SYSTEM_WORKMODE wm;
            if ( pMRQ->getSYSTEM_WORKMODE( &wm ) == 0 )
            {
                pMRQ->uploadDesc();
            }
            else
            {
            }
        }
        sysProgress( uiProgBase + (uiStep++)*uiProgStep, tr("device") );

        //! gen robo
        {
            //! get class
            QString roboClass;
            QString deviceDesc;

            //! info
            deviceDesc = pMRQ->getDesc();

            //! class
            roboClass = m_pMainModel->mDeviceDbs.findClass( deviceDesc );
            delete pMRQ;

            pRobo = robotFact::createRobot( roboClass );
            if ( NULL == pRobo )
            { return ERR_ALLOC_FAIL; }

            //! delete the robo
            //! bus config
            pRobo->setInterval( m_pMainModel->mSysPref.mInterval );
            pRobo->setTimeout( m_pMainModel->mSysPref.mTimeout );

            pRobo->attachBus( pNewBus );
            pRobo->setInstMgr( this );

            //! iter ref the sibling
            ret = pRobo->setDeviceId( *pId );
            if ( ret != 0 )
            {
                logDbg()<<ret;
                delete pMRQ;
                return ret;
            }

            //! get info
            if ( deviceDesc.length() > 0 )
            {
                pRobo->rst();
                pRobo->upload();
            }
            //! only for id
            else
            {
                pRobo->upload( VDevice::e_device_content_id );
            }

            //! auto load setup
            if ( m_pMainModel->mSysPref.mbAutoLoadSetup )
            {
                if ( 0!= pRobo->uploadSetting() )
                { sysError( tr("load fail") ) ; }
                else
                { sysLog( tr("load success"), pRobo->name() );}
            }

            //! add robot
            roboList.append( pRobo );

            //! open scpi
            pRobo->open();
        }
        sysProgress( uiProgBase + (uiStep++)*uiProgStep, tr("robo") );
    }

    //! hub for e
    if ( pNewBus->pId() == 0 )
    {
        pRobo = new Mrh_e();
        if ( NULL == pRobo )
        { return ERR_ALLOC_FAIL; }

        pRobo->attachBus( pNewBus );
        pRobo->setInstMgr( this );

        pRobo->open();
        roboList.append( pRobo );
    }

    return 0;
}

void InstMgr::gc()
{
    gcPhyBus();

    gcFileBus();

    delete_all( mDevices );
}

void InstMgr::gcPhyBus()
{
//    Q_ASSERT( NULL != m_pReceiveCache );
//    m_pReceiveCache->detachBus();
//    mCanBus.close();

    //! collect the current device map
    mDeviceMap.clear();
    foreach( VRoboList *pList, mDeviceTree )
    {
        Q_ASSERT( NULL != pList );
        foreach( VRobot*pRobo, *pList )
        {
            Q_ASSERT( NULL != pRobo );
            mDeviceMap.insert( pRobo->getSignature(), pRobo->getName() );
        }
    }

    foreach( CANBus *pBus, mCanBuses )
    {
        Q_ASSERT( NULL != pBus );
        pBus->close();
    }

    delete_all( mCanBuses );

    delete_all( mDeviceTree );

    logDbg();
}
void InstMgr::gcFileBus()
{
    delete_all( mFileDeviceTree );
    delete_all( mFileBusList );

    logDbg();
}

scpiShell *InstMgr::findShell( const QString &name )
{
    //! device
    foreach( VRoboList *pRoboList, mDeviceTree )
    {
        Q_ASSERT( NULL != pRoboList );
        foreach( scpiShell * pDev, *pRoboList )
        {
            Q_ASSERT( NULL != pDev );
            if ( QString::compare( pDev->getName(), name, Qt::CaseInsensitive)==0 )
            { return pDev; }
        }
    }

    //! robo
    foreach( VRoboList *pRoboList, mFileDeviceTree )
    {
        Q_ASSERT( NULL != pRoboList );
        foreach( scpiShell * pDev, *pRoboList )
        {
            Q_ASSERT( NULL != pDev );
            if ( QString::compare( pDev->getName(), name, Qt::CaseInsensitive)==0 )
            { return pDev; }
        }
    }

    //! for mgr
    QString str = QHostInfo::localHostName();
    if ( QString::compare(str, name, Qt::CaseInsensitive) == 0 )
    { return m_pMainShell; }
    else if ( QString::compare( "localhost", name, Qt::CaseInsensitive) == 0 )
    { return m_pMainShell; }
    else
    {}

    return NULL;
}

bool InstMgr::matchSignature( quint32 sig, QString &alias )
{
    QMapIterator<quint32, QString> iter(mDeviceMap);

    //! exist
    while (iter.hasNext())
    {
        iter.next();
        if ( iter.key() == sig )
        {
            alias = iter.value();
            return true;
        }
    }

    return false;
}


}
