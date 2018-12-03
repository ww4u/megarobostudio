#ifndef ROBOVIEW_H
#define ROBOVIEW_H

#include "dlgview.h"

class RoboView : public DlgView
{
public:
    enum eRoboViewId
    {
        view_none,
        view_joint,
        view_robo,
        view_axes,
    };

public:
    static RoboView *findView( VRobot *pRobo,
                               RoboView::eRoboViewId vid,
                               int subId = 0 );
protected:
    static QList<RoboView*> _liveWindows;

    Q_OBJECT

public:
    RoboView( VRobot *pRobo, RoboView::eRoboViewId vid, int subId,
              mcModel *pModel, QWidget *parent=NULL);

protected:
    virtual void closeEvent( QCloseEvent *event );

protected:
    VRobot *m_pRobo;
    eRoboViewId mViewId;

    int mSubId;
};

#endif // ROBOVIEW_H
