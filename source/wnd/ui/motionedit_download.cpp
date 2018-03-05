
#include "motionedit.h"
#include "ui_motionedit.h"

#include "../../arith/pathplan/pathplan.h"
#include "../../arith/kinematic/kinematic.h"

int motionEdit::postDownload( appMsg msg, void *pPara )
{
//    testDownload();
//    return 0;
logDbg();
    int ret;
    ret = setLoop( ui->spinLoop->value() );
    if ( ret != 0 )
    { return ret; }
logDbg();
    ret = buildTpvGroups();
    if ( ret != 0 )
    { logDbg();return ret; }
logDbg();
    //! check groups
    if ( mJointsTpvGroup.size() > 0 )
    {}
    else
    { logDbg();return ERR_NO_TPV_DATA; }
logDbg();
    QList<int> jointTabId;
    jointTabId<<0<<1<<2<<3<<4;
    ret = doDownload( mJointsTpvGroup, jointTabId );
logDbg()<<ret;
    return ret;
}
void motionEdit::beginDownload( void *pPara)
{ logDbg(); }
void motionEdit::endDownload( int ret, void *pPara )
{ logDbg(); }

int motionEdit::doDownload( QList<tpvGroup *> &groups,
                            QList<int> &jointTabId )
{
    //! check robo
    VRobot *pRobo;
    pRobo = currentRobot();
    if ( NULL == pRobo )
    { return ERR_INVALID_ROBOT_NAME; }

    setChildAgents( pRobo->axesDeviceName() );

    pRobo->onLine();

    pRobo->download( groups, jointTabId );

    return 0;
}

int motionEdit::buildTpvGroups()
{
    delete_all( mJointsTpvGroup );

    if ( mJointsTraceSize > 0 && m_pJointsTrace != NULL )
    { }
    else
    { return ERR_NO_TPV_DATA; }

    //! create each joints group
    int ret;
    for ( int i = 0; i < 4; i++ )
    {
        tpvGroup *pGroup = new tpvGroup();
        Q_ASSERT( NULL != pGroup );

        for ( int j = 0; j < mJointsTraceSize; j++ )
        {
            ret = pGroup->addItem( m_pJointsTrace[j].t,
                             m_pJointsTrace[j].p[i],
                             m_pJointsTrace[j].v[i] );
            if ( ret != 0 )
            {
                delete_all( mJointsTpvGroup );
                return ret;
            }
        }

        mJointsTpvGroup.append( pGroup );
    }

    //! add the hand tpv
    for ( int i = 0; i < 1; i++ )
    {
        tpvGroup *pGroup = new tpvGroup();
        Q_ASSERT( NULL != pGroup );

        foreach( tpvItem *pItem, mHandTpvGroup.mItems )
        {
            Q_ASSERT( NULL != pItem );
            ret = pGroup->addItem( pItem->getT(),
                                   pItem->getP(),
                                   pItem->getV() );
            if ( ret != 0 )
            {
                delete_all( mJointsTpvGroup );
                return ret;
            }
        }

        mJointsTpvGroup.append( pGroup );
    }

    return 0;
}

void motionEdit::jointsRotate( jointsTrace *pJ, int len )
{
    Q_ASSERT( NULL != pJ );

    for ( int i = 0; i < len; i++ )
    {
        for ( int j = 0; j < 4; j++ )
        {
            pJ[i].p[j] = RAD_TO_DEG( pJ[i].p[j] )*5;
        }
    }
}
