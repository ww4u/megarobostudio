#ifndef MOTIONEDIT_H
#define MOTIONEDIT_H

#include <QtWidgets>

#include "../../model/motiongroup.h"
#include "../../model/comboxdelegate.h"

#include "tableedit.h"
#include "roboAxes.h"

#include "../ui/progressgroup.h"

namespace Ui {
class motionEdit;
}

//! for joints trace
class jointsTrace;
class tracePoint;

class motionEdit : public tableEdit
{
    Q_OBJECT
public:
    explicit motionEdit(QWidget *parent = nullptr);
    virtual ~motionEdit();

protected:
    void buildConnection();

Q_SIGNALS:
    void sig_joints_trace_changed();

public Q_SLOTS:
    void slot_robo_changed( const QString &roboName );
    void slot_joints_trace_changed( );

public:
    virtual void setModelObj( mcModelObj *pObj );

    virtual int save( QString &outFileName );
    virtual int saveAs( QString &outFileName );

protected:
    virtual void onNetEvent(const QString &name,
                            int axes,
                            RoboMsg &msg);

    void onMotionStatus( int axes, MRQ_MOTION_STATE stat );

protected:
    //! dpc proc
    int postDownload( appMsg msg, void *pPara );
    void beginDownload( void *pPara);
    void endDownload( int ret, void *pPara );

    ProgressGroup *progress();

    VRobot *currentRobot();

protected:
    int setLoop( int loop );
    int doDownload( QList<tpvGroup *> &groups,
                    QList<int> &jointTabId );

    void testDownload();

    //! trace plan
protected:
    int compileTrace();

    int preCompileTrace();
    int postCompileTrace( int ret );

    //! check
    int checkRobot();

    int checkTrace();
    int checkJointsTrace();
    int checkHandTrace();

    //! build
    int buildTrace();

    int buildJointsTrace();

    int planJointsTrace();
    int splitJointsTrace();

    int buildHandTrace();

    //! onvert
    int buildTpvGroups();

    void jointsRotate( jointsTrace *pJ, int len );

signals:

protected Q_SLOTS:
    void on_btnAdd_clicked();
    void on_btnDel_clicked();
    void on_btnClr_clicked();
    void on_btnGraph_clicked();

    void slot_download_cancel( const QString &name, int id );

protected:
    virtual void context_remove();
    virtual void context_add_before();
    virtual void context_add_below();

private slots:
    void on_btnKnob_clicked();

    void on_btnDown_clicked();

    void on_btnStart_clicked();

    void on_btnStop_clicked();

    void on_toolButton_clicked();

private:
    Ui::motionEdit *ui;

    motionGroup *mMotionGroup;
    roboAxes *m_pRoboAxes;

    jointsTrace *m_pJointsTrace;
    int mJointsTraceSize;

    tracePoint *m_pTracePoint;          //! all trace point
    int mTracePointSize;

    QIntValidator *m_pLoopValidator;
    ProgressGroup *m_pProgress;

    comboxDelegate *m_pActionDelegate;

    QList< tpvGroup *> mJointsTpvGroup;
    tpvGroup mHandTpvGroup;
};

#endif // MOTIONEDIT_H
