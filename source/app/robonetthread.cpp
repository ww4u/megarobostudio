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

        //! event id, frame id, byte array
        if ( msg.mMsg == e_interrupt_occuring )
        {
            //! check
            if ( !msg.checkType( QMetaType::Int,
                           QMetaType::Int,
                           QMetaType::QByteArray ) )
            { logDbg(); return; }

            //! convert to device name
            QString name;
            name = m_pModel->m_pInstMgr->sendIdToName( msg.at(1).toInt() );
            if ( name.isEmpty() )
            { logDbg(); return; }

            //! signal
            QByteArray ary = msg.at(2).toByteArray();
            logDbg()<<ary;
            emit signal_net( name, ary.at(2), msg );
        }

        //! name, axesid
        else if ( msg.mMsg == e_download_started
                  || msg.mMsg == e_download_started
                  || msg.mMsg == e_download_completed )
        {
            //! check
            if ( !msg.checkType( QMetaType::QString,
                           QMetaType::Int) )
            { return; }

            emit signal_net( msg.at(0).toString(),
                             msg.at(1).toInt(),
                             msg );
        }

        //! name, axesid, now, total
        else if ( msg.mMsg == e_download_processing )
        {
            //! check
            if ( !msg.checkType( QMetaType::QString,
                                QMetaType::Int,
                                QMetaType::Int,
                                QMetaType::Int) )
            { return; }

            emit signal_net( msg.at(0).toString(),
                             msg.at(1).toInt(),
                             msg );
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

