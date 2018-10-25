
#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::exceptionProc( const QString &name,
                                int ax,
                                int exceptionId,
                                RoboMsg &msg )
{
    Q_ASSERT( NULL != mMcModel.mSysPref.mEventActionModel.items() );

    int actId;
    quint64 ts = 0;
    foreach( EventAction *pAction, *mMcModel.mSysPref.mEventActionModel.items() )
    {
        Q_ASSERT( NULL != pAction );

        actId = eventViewer::exceptionCode( pAction->event() );

        if ( exceptionId == actId
             && pAction->enable() )
        {
            int exceptionAction = eventViewer::actionCode( pAction->action() );

            //! stop
            if ( exceptionAction == e_device_action_stop
                 || exceptionAction == e_device_action_prompt_stop )
            {
                //! filter by timestamp
                if ( (msg.timeStamp() - ts) > 64 )
                { on_actionForceStop_triggered(); }
                ts = msg.timeStamp();
            }
            else
            {}

            //! prompt
            QString diagStr = QString("%1 CH%2:%3").arg( name ).arg( ax + 1).arg( pAction->event() );
            if ( exceptionAction == e_device_action_prompt
                 || exceptionAction == e_device_action_prompt_stop
                 )
            {
                if ( NULL == m_pWarnPrompt )
                { m_pWarnPrompt = new WarnPrompt(this); }

                Q_ASSERT( NULL != m_pWarnPrompt );

                m_pWarnPrompt->addInfo( diagStr );
                m_pWarnPrompt->show();
                m_pWarnPrompt->activateWindow();
            }
            else
            {}

            //! diagnosis
            mDiagnosisMutex.lock();
                if ( mDiagnosisLog.size() > 128 )
                { mDiagnosisLog.removeFirst(); }

                mDiagnosisLog.append( diagStr  );
            mDiagnosisMutex.unlock();
        }
    }
}

