
#include "devicemgr.h"

#include "../../../include/mcstd.h"
#include "../../sys/sysapi.h"

#include "deviceconsole.h"

//! test
int deviceMgr::postMotorTest( appMsg msg, void *pPara )
{
    execMotorTest();

    return 0;
}
void deviceMgr::beginMotorTest( void *pPara )
{

}
void deviceMgr::endMotorTest( int ret, void *pPara )
{}
void deviceMgr::execMotorTest()
{}
#if 0
void deviceMgr::execMotorTest()
{
    MegaDevice::deviceMRQ *pMrq;
    QList< MegaDevice::deviceMotor * > motors;

    for ( int device = 0; device < m_pMgr->mDevices.size(); device++ )
    {
        pMrq = (MegaDevice::deviceMRQ*)m_pMgr->mDevices.at( device );
        Q_ASSERT( NULL != pMrq );

        for ( int i = 0; i < pMrq->axes(); i++ )
        {
            motors.append( pMrq->Motor(i));
        }
    }

    if ( motors.size() > 0 )
    {
        tpvRow *pTpvRow;
        QList< tpvRow *> mRows;

        int t = 0;
        int count = 2;
        for ( int i = 0; i < count; i++, t++ )
        {
            pTpvRow = new tpvRow();
            pTpvRow->mT = t*0.5;
            pTpvRow->mP = i*360;
            pTpvRow->mV = 0;

            mRows.append( pTpvRow );
        }

//        for ( int i = count-1; i >= 0; i--, t++ )
//        {
//            pTpvRow = new tpvRow();
//            pTpvRow->mT = t*0.05;
//            pTpvRow->mP = i*100;
//            pTpvRow->mV = 0;

//            mRows.append( pTpvRow );
//        }

        bool bCond;
        int axesSize = motors.size();
        for ( int loopCnt = 1; loopCnt <= 10000; loopCnt++ )
//        for ( int loopCnt = 1; loopCnt <= 1; loopCnt++ )
        {
            MegaDevice::MrqStateCondition cond[axesSize];
            for ( int i = 0; i < axesSize; i++ )
            {
                cond[i].set( motors[i]->Axes() );
                cond[i].setState( MegaDevice::mrq_state_idle );
            }

            for ( int i = 0; i < axesSize; i++ )
            {
                //! write
//                pMrq->attachCondition( i, cond + i );
//                pMrq->pvtWrite( i, mRows );

                motors[i]->attachCondition( cond + i );
                motors[i]->pvtWrite( mRows );

                cond[i].setState( MegaDevice::mrq_state_standby );
            }
sysLog( QString::number(__LINE__) );
            for ( int i = 0; i < axesSize; i++ )
            {
                sysLog( QString::number(__LINE__), QString::number(i) );
                motors[i]->waitCondition( cond + i );
                sysLog( QString::number(__LINE__), QString::number(i) );
            }

            for ( int i = 0; i < axesSize; i++ )
            {
                //! run
//                pMrq->attachCondition( i, cond + i );
//                pMrq->run( i );
                motors[i]->attachCondition( cond + i );
                motors[i]->run();

                cond[i].setState( MegaDevice::mrq_state_idle );
            }
logDbg();
            for ( int i = 0; i < axesSize; i++ )
            {
sysLog( QString::number(__LINE__), QString::number(i) );
                bCond = motors[i]->waitCondition( cond + i );
sysLog( QString::number(__LINE__), QString::number(i) );
                logDbg()<<bCond;

                //! wait completed
//                QThread::sleep( 5 );

                //! not exec success
//                if ( pMrq->status( i ) != MRQ_MOTION_STATE_CALCEND )
                if ( motors[i]->status() != MRQ_MOTION_STATE_CALCEND )
                {
                    sysLog( tr("error"), QString("%1").arg( motors[i]->status() ) );
                    return;
                }

//                ui->btnTest->setText( QString("%1").arg(loopCnt) );

                //! wait end
//                qApp->processEvents();
            }
        }

//        //! device 1
//        pMrq = (MegaDevice::deviceMRQ*)m_pMgr->mDevices.at(1);
//        pMrq->run( 0 );
//        pMrq->pvtWrite( 0, mRows );

//        pMrq = (MegaDevice::deviceMRQ*)m_pMgr->mDevices.at(2);
//        pMrq->run( 0 );
//        pMrq->pvtWrite( 0, mRows );

//        pMrq = (MegaDevice::deviceMRQ*)m_pMgr->mDevices.at(3);sysLog( pMrq->name() );
//        pMrq->run( 0 );
//        pMrq->pvtWrite( 0, mRows );

    }
}
#endif

//! robo test
#include "../../robot/megatron/megatron.h"
int deviceMgr::postRoboTest( appMsg msg, void *pPara )
{
    execRoboTest();
    return 0;
}
void deviceMgr::beginRoboTest( void *pPara )
{}
void deviceMgr::endRoboTest( int ret, void *pPara )
{}

void deviceMgr::execRoboTest()
{
    VRobot * pRobo;

    logDbg()<<m_pMgr->roboResources();

    pRobo = m_pMgr->findRobot( "c@robo1.sce" );
    Q_ASSERT( NULL != pRobo );

    robotMegatron *pMegatron = (robotMegatron*)pRobo;
    Q_ASSERT( NULL != pMegatron );

    //! prepare data
    QList< tpvGroup * > groups;
    groups.append( tpvGroup::createDemoGroup1() );
    groups.append( tpvGroup::createDemoGroup2() );
    groups.append( tpvGroup::createDemoGroup3() );
    groups.append( tpvGroup::createDemoGroup4() );
    groups.append( tpvGroup::createDemoGroup5() );

    QList< int > joints;
    joints.append( 0 );
    joints.append( 1 );
    joints.append( 2 );
    joints.append( 3 );
    joints.append( 4 );

    pMegatron->onLine();

    RawRoboStateCondition cond;
begin_time()
    cond.setState( MegaDevice::mrq_state_standby );
    pMegatron->attachCondition( tpvRegion(0,0), &cond );
    pMegatron->download( groups, joints, tpvRegion(0,0) );
    pMegatron->waitCondition( tpvRegion(0,0), &cond );
end_time()
//    QThread::sleep( 30 );

begin_time()
    cond.setState( MegaDevice::mrq_state_idle );
    pMegatron->attachCondition( tpvRegion(0,0), &cond );
    pMegatron->run( tpvRegion(0,0) );
    pMegatron->waitCondition( tpvRegion(0,0), &cond );
end_time()
//    QThread::sleep( 30 );

    pMegatron->offLine();

    delete_all( groups );
}

//! id1, id2
QString deviceMgr::subGroupString( QList<int> subIds )
{
    QString strGp;
    QString strSep ;
    for ( int i = 0; i <subIds.size(); i++ )
    {
        strGp = strGp + strSep;

        strGp += subGroupString( subIds.at(i) );

        strSep = "/";
    }

    return strGp;
}

QString deviceMgr::subGroupString( int id )
{
    if ( id == robo_channels(0) )
    { return tr("GROUP1"); }
    else if ( id == robo_channels(1) )
    { return tr("GROUP2"); }
    else
    { return "Unknown group"; }
}
