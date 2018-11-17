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
    };

public:
    static RoboView *findView( VRobot *pRobo,
                               RoboView::eRoboViewId vid );
protected:
    static QList<RoboView*> _liveWindows;

    Q_OBJECT

public:
    RoboView( VRobot *pRobo, RoboView::eRoboViewId vid,
              mcModel *pModel, QWidget *parent=NULL);

protected:
    virtual void closeEvent( QCloseEvent *event );

protected:
    VRobot *m_pRobo;
    eRoboViewId mViewId;
};

#endif // ROBOVIEW_H
