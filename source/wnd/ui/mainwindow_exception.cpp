
#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::exceptionProc( const QString &name,
                                int exceptionId,
                                RoboMsg &msg )
{
    Q_ASSERT( NULL != mMcModel.mEventActionModel.items() );

    foreach( EventAction *pAction, *mMcModel.mEventActionModel.items() )
    {
        Q_ASSERT( NULL != pAction );

        if ( pAction->enable() )
        {
            int exceptionAction;

            exceptionAction = eventViewer::actionCode( pAction->action() );

            if ( exceptionAction == e_device_action_prompt )
            {
                if ( NULL == m_pWarnPrompt )
                { m_pWarnPrompt = new WarnPrompt(this); }

                Q_ASSERT( NULL != m_pWarnPrompt );

                m_pWarnPrompt->setInfo( name + ":" + pAction->event() );

                m_pWarnPrompt->show();
            }
            else if ( exceptionAction == e_device_action_stop )
            { on_actionForceStop_triggered(); }
            else
            {}
        }
    }
}
