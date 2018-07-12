#include "robonetthread.h"

RoboNetThread::RoboNetThread( QObject *pObj ) : RoboMsgThread( pObj )
{
    mIntervalus = 100;  //! us

    m_pModel = NULL;

    RoboMsgQueue::attachMsgThread( this );
}

void RoboNetThread::setMcModel( mcModel *pModel )
{
    Q_ASSERT( NULL != pModel );

    m_pModel = pModel;
}

void RoboNetThread::onMsg( RoboMsg &msg )
{
    //! proc
    {
        Q_ASSERT( NULL != m_pModel );

        //!  0          1      2         3
        //! event id, devId, frame id, byte array
        if ( msg.mMsg == e_interrupt_occuring )
        {
            //! check
            if ( !msg.checkType( QMetaType::Int,
                           QMetaType::Int,
                           QMetaType::Int,
                           QMetaType::QByteArray ) )
            { logDbg(); return; }

            //! convert to device name
            QString name;
            name = m_pModel->m_pInstMgr->sendIdToName( msg.at(1).toInt(),
                                                       msg.at(2).toInt() );
            if ( name.isEmpty() )
            { logDbg()<<msg.at(1).toInt()<<msg.at(2).toInt(); return; }

            //! signal
            QByteArray ary = msg.at(3).toByteArray();

            emit signal_net( name, ary.at(2), msg );        //! 0,1--maincode,subcode
        }

        //! name, region
        else if ( msg.mMsg == e_download_started
                  || msg.mMsg == e_download_started
                  || msg.mMsg == e_download_completed
                  || msg.mMsg == e_download_terminated
                  || msg.mMsg == e_download_canceled
                  )
        {
            //! check
            if ( !msg.checkType( QMetaType::QString,
                                 TPV_REGEION_TYPE_ID ) )
            { return; }

            tpvRegion region = msg.at(1).value<tpvRegion>();

            emit signal_net( msg.at(0).toString(),
                             region.axes(),
                             msg );
        }

        //! name, region, now, total
        else if ( msg.mMsg == e_download_processing )
        {
            //! check
            if ( !msg.checkType( QMetaType::QString,
                                TPV_REGEION_TYPE_ID,
                                QMetaType::Int,
                                QMetaType::Int) )
            { return; }

            tpvRegion region = msg.at(1).value<tpvRegion>();

            emit signal_net( msg.at(0).toString(),
                             region.axes(),
                             msg );
        }

        //! info
        else if ( msg.mMsg == e_progress_para )
        {
            if ( !msg.checkType( QMetaType::Int,
                                 QMetaType::Int,
                                 QMetaType::Int,
                                 QMetaType::QString) )
            { return; }

            emit signal_progress( msg.at(0).toInt(),
                                  msg.at(1).toInt(),
                                  msg.at(2).toInt(),
                                  msg.at(3).toString() );
        }
        else if ( msg.mMsg == e_progress_visible )
        {
            if ( !msg.checkType( QMetaType::Bool ) )
            { return; }

            emit signal_progress( msg.at(0).toBool() );
        }

        else if ( msg.mMsg == e_status )
        {
            if ( !msg.checkType( QMetaType::QString ) )
            { return; }

            emit signal_status( msg.at(0).toString() );
        }

        else if ( msg.mMsg == e_logout )
        {
            if ( !msg.checkType( QMetaType::QString ) )
            { return; }

            emit signal_logout( msg.at(0).toString() );
        }

        else if ( msg.mMsg == e_prompt )
        {
            if ( !msg.checkType( QMetaType::QString ) )
            { return; }

            emit signal_prompt( msg.at(0).toString() );
        }

        else if ( msg.mMsg == e_rpc )
        {
            //! \note no check

            RpcRequest rpc = msg.at(0).value<RpcRequest>();

            emit signal_request( rpc );
        }

        else if ( msg.mMsg == e_emerge_stop )
        {
            //! \note no para

            emit signal_emergeStop();
        }

        //! name, status
        else if ( msg.mMsg == e_robot_status )
        {
            if ( !msg.checkType( QMetaType::QString,
                                 QMetaType::Int ) )
            { return; }

            emit signal_net( msg.at(0).toString(),
                             msg.at(1).toInt(),
                             msg );
        }

        //! robo opers
        else if ( msg.mMsg > e_robo_operation               //! name, axes
                  && msg.mMsg < e_robo_operation_max )
        {
            //! check type
            if ( !msg.checkType( QMetaType::QString,
                                QMetaType::Int) )
            { Q_ASSERT(false);return; }

            //! to mrq
            VRobot *pRobo;
            pRobo = m_pModel->m_pInstMgr->findRobot( msg.at(0).toString(),
                                                     msg.at(1).toInt() );
            if ( NULL == pRobo )
            {
                logDbg()<<msg.at(0).toString();
                Q_ASSERT(false);
                return;
            }

            int ax = msg.at(1).toInt();
            pRobo->lpc( ax )->postMsg( (eRoboMsg)msg.getMsg(),
                                       ax );

        }
        else
        {

        }
    }
}

