
#include "instmgr.h"

#include "../../model/mcmodel.h"

namespace MegaDevice
{

InstMgr *InstMgr::_mpMgr = NULL;

InstMgr::InstMgr( QObject *parent ) : instServer( parent )
{
    m_pMainModel = NULL;

    m_pReceiveCache = new receiveCache();
}

InstMgr::~InstMgr()
{
    gc();

    //! receive end
    m_pReceiveCache->terminate();
    m_pReceiveCache->wait();
    delete m_pReceiveCache;
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
        logDbg()<<rdSize<<retData;
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

//! \todo only one time
int InstMgr::probeBus()
{
    Q_ASSERT( NULL != m_pMainModel );
    int ret;

    preProbeBus();

    ret = probeCanBus();
//    ret = -1;

    postProbeBus();

    if ( ret != 0 )
    { return ret; }

    return 0;
}

int InstMgr::probeCanBus()
{
    int ret;

    //! bus prop.
    mCanBus.setPId( m_pMainModel->mSysPref.mPort );
    mCanBus.setSpeed( m_pMainModel->mSysPref.mSpeed );
    mCanBus.setWtInterval( m_pMainModel->mSysPref.mInterval );
    mCanBus.setRdTmo( m_pMainModel->mSysPref.mTimeout );
    mCanBus.setEnumTmo( m_pMainModel->mSysPref.mEnumerateTimeout );
    mCanBus.setFailTry( m_pMainModel->mSysPref.mFailTryCnt );

    //! open
    ret = mCanBus.open();
    if ( ret != 0 )
    { logDbg()<<ret; return ret; }

    //! start the receive cache
    m_pReceiveCache->attachBus( &mCanBus );
    mCanBus.attachReceiveCache( m_pReceiveCache );
    if ( m_pReceiveCache->isRunning() )
    {}
    else
    {
        m_pReceiveCache->start( QThread::TimeCriticalPriority );
    }

    //! enumerate
    ret = mCanBus.enumerate( m_pMainModel->mSysPref );
    logDbg()<<ret;
    if ( ret != 0 )
    { logDbg(); return ret; }

    //! create the device
    int seq = 1;
    deviceMRQ *pMRQ;
    VRobot *pRobo;

    //! a robo list
    VRoboList *pRoboList = new VRoboList();
    Q_ASSERT( NULL != pRoboList );
    pRoboList->attachBus( &mCanBus );
//    logDbg()<<QString::number( (quint32)pRoboList, 16 );

    foreach( DeviceId * pId, mCanBus.mDevices )
    {
        //! detect device
        {
            pMRQ = new deviceMRQ();
            Q_ASSERT( NULL != pMRQ );

            //! bus config
            pMRQ->setInterval( m_pMainModel->mSysPref.mInterval );
            pMRQ->setTimeout( m_pMainModel->mSysPref.mTimeout );

            pMRQ->attachBus( &mCanBus );
            pMRQ->setInstMgr( this );

            //! iter ref the sibling
            ret = pMRQ->setDeviceId( *pId, mCanBus.mDevices.size() - 1 );
            if ( ret != 0 )
            {
                logDbg()<<ret;
                delete pMRQ;
                return ret;
            }

            //! get info
            pMRQ->rst();
            pMRQ->uploadBaseInfo();

            logDbg()<<QString::number( (uint32)pMRQ, 16 );
            logDbg()<<seq<<pMRQ->getModel()->mCAN_SENDID;
            logDbg()<<seq<<pMRQ->getModel()->mCAN_RECEIVEID;
        }

        //! gen robo
        {
            //! get class
            QString roboClass;

            //! class
            roboClass = m_pMainModel->mDeviceDbs.findClass( pMRQ->getDesc() );
            delete pMRQ;

            pRobo = robotFact::createRobot( roboClass );
            if ( NULL == pRobo )
            { return ERR_ALLOC_FAIL; }

            //! delete the robo
            //! bus config
            pRobo->setInterval( m_pMainModel->mSysPref.mInterval );
            pRobo->setTimeout( m_pMainModel->mSysPref.mTimeout );

            pRobo->attachBus( &mCanBus );
            pRobo->setInstMgr( this );

            //! iter ref the sibling
            ret = pRobo->setDeviceId( *pId, mCanBus.mDevices.size() - 1 );
            if ( ret != 0 )
            {
                logDbg()<<ret;
                delete pMRQ;
                return ret;
            }

            //! get info
            pRobo->rst();
            pRobo->upload();

            //! auto load setup
            if ( m_pMainModel->mSysPref.mbAutoLoadSetup )
            {
                if ( 0!= pRobo->uploadSetting() )
                { sysError( tr("load fail") ) ; }
                else
                { sysLog( tr("load success"), pRobo->name() );}
            }

            pRoboList->append( pRobo );

            //! set def scpi name
            //! default device name
            pRobo->setName( QString("device%1").arg(seq) );
            seq++;

            //! open scpi
            pRobo->open();
        }
    }
logDbg()<<QString::number( (quint32)pRoboList, 16 );
    mDeviceTree.append( pRoboList );

    //! plant devices
    mDevices.append( *pRoboList );

    return 0;
}

//! broadcast
int InstMgr::emergencyStop()
{
    byte buf[] = { mc_MOTION, sc_MOTION_SWITCH, CAN_BROAD_CHAN, MRQ_MOTION_SWITCH_EMERGSTOP, 0 };
    int ret;

    //! 1. broadcast
    DeviceId broadId( CAN_BROAD_ID );
    ret = mCanBus.doWrite( broadId, buf, sizeof(buf) );

    return ret;
}

int InstMgr::hardReset()
{
    byte buf[] = { mc_MOTION, sc_MOTION_SWITCH, CAN_BROAD_CHAN, MRQ_MOTION_SWITCH_RESET, 0 };
    int ret;

    //! 1. broadcast
    DeviceId broadId( CAN_BROAD_ID );
    ret = mCanBus.doWrite( broadId, buf, sizeof(buf) );

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

receiveCache *InstMgr::getInterruptSource()
{
    return m_pReceiveCache;
}

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
    logDbg()<<strList;
    Q_ASSERT( strList.size() >  1 );
    return findRobot( strList[0], strList[1] );
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

VRobot * InstMgr::findRobotBySendId( int sendId, int axesId )
{
    foreach( VRoboList *pRoboList, mDeviceTree )
    {
        Q_ASSERT( NULL != pRoboList );
        foreach( VRobot * pDev, *pRoboList )
        {
            Q_ASSERT( NULL != pDev );

            if ( pDev->canSendId() == sendId )
            { return pDev; }
        }
    }

    return NULL;
}

VRobot * InstMgr::findRobotByRecvId( int recvId, int axesId )
{
    foreach( VRoboList *pRoboList, mDeviceTree )
    {
        Q_ASSERT( NULL != pRoboList );
        foreach( VRobot * pDev, *pRoboList )
        {
            Q_ASSERT( NULL != pDev );

            if ( pDev->canRecvId() == recvId )
            { return pDev; }
        }
    }

    return NULL;
}

deviceMRQ *InstMgr::findDevice( const QString &name, int axesId )
{
    VRobot *pRobot;

    pRobot = findRobot( name, axesId );
    if ( NULL == pRobot )
    { return NULL; }
    else
    { return (deviceMRQ*)pRobot; }
}

//! chx@devicename
deviceMRQ *InstMgr::findDevice( const QString &name, int *pAxes )
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
    return findDevice( strList[1], axesId - 1 );
}

QString InstMgr::sendIdToName( int sendId )
{
    deviceMRQ *pMrq;

    foreach( VRoboList *pRoboList, mDeviceTree )
    {
        Q_ASSERT( NULL != pRoboList );
        foreach( VRobot * pDev, *pRoboList )
        {
            Q_ASSERT( NULL != pDev );

            if ( robot_is_mrq( pDev->robotId()) )
            {}
            else
            { continue; }

            //! convert
            pMrq = (deviceMRQ*)pDev;
            if ( NULL == pMrq )
            { logDbg();return QString(); }

            //! check axes
            if ( pMrq->getModel()->mCAN_SENDID == (quint32)sendId )
            { return pDev->name(); }

        }
    }

    return QString();
}

//! for all devices
QStringList InstMgr::getResources()
{
    QStringList resrc;

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

void InstMgr::setTPVBase( float t, float p, float v )
{
    foreach( VRoboList *pRoboList, mDeviceTree )
    {
        Q_ASSERT( NULL != pRoboList );
        foreach( VRobot * pDev, *pRoboList )
        {
            Q_ASSERT( NULL != pDev );

            pDev->setTPVUnit( t, p, v );
        }
    }
}

void InstMgr::preProbeBus()
{
    gcPhyBus();
}
void InstMgr::postProbeBus()
{}

void InstMgr::gc()
{
    gcPhyBus();

    gcFileBus();

    delete_all( mDevices );
}

void InstMgr::gcPhyBus()
{
    m_pReceiveCache->detachBus();
    mCanBus.close();

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
            if ( pDev->getName() == name )
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
            if ( pDev->getName() == name )
            { return pDev; }
        }
    }

    return NULL;
}

}
