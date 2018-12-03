#include "roboview.h"

QList<RoboView*> RoboView::_liveWindows;

RoboView* RoboView::findView( VRobot *pRobo,
                              RoboView::eRoboViewId vid,
                              int subId )
{//logDbg()<<QString::number( (uint32)pRobo, 16 )<<DlgView::_liveWindows.size();
    foreach( RoboView *iter, RoboView::_liveWindows )
    {
        Q_ASSERT( NULL != iter );

        if ( iter->m_pRobo == pRobo && iter->mViewId == vid && iter->mSubId == subId )
        { return iter; }
    }

    return NULL;
}

RoboView::RoboView( VRobot *pRobo, RoboView::eRoboViewId vid, int subId,
                    mcModel *pModel, QWidget *parent ) : DlgView( pModel, parent )
{
    mViewId = vid;
    m_pRobo = pRobo;

    mSubId = subId;

    //! append
    RoboView::_liveWindows.append( this );
}

void RoboView::closeEvent( QCloseEvent *event )
{
    //! remove the list
    RoboView::_liveWindows.removeAll( this );

    DlgView::closeEvent( event );
}
