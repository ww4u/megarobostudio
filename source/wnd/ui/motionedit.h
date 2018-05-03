#ifndef MOTIONEDIT_H
#define MOTIONEDIT_H

#include <QtWidgets>

#include "../../model/sinanjumotiongroup.h"
#include "../../model/megatronmotiongroup.h"

#include "../../model/comboxdelegate.h"

#include "../../arith/pathplan/pathplan.h"
#include "../../arith/sinanju_split/sinanju_split.h"

#include "tableedit.h"
#include "roboAxes.h"
#include "tpvplot.h"

#include "../ui/progressgroup.h"
#include "motionpref.h"

namespace Ui {
class motionEdit;
}

class motionEdit : public tableEdit
{
    Q_OBJECT
public:
    explicit motionEdit(QWidget *parent = nullptr);
    virtual ~motionEdit();

protected:
    void buildConnection();

public:
    virtual void setModelObj( mcModelObj *pObj );

    virtual int save( QString &outFileName );
    virtual int saveAs( QString &outFileName );

protected:
    virtual void onNetEvent(const QString &name,
                            int axes,
                            RoboMsg &msg);

    void onMotionStatus( int axes, MRQ_MOTION_STATE_2 stat );

protected:
    //! dpc proc
    int postDownload( appMsg msg, void *pPara );
    void beginDownload( void *pPara);
    void endDownload( int ret, void *pPara );

//    ProgressGroup *progress();

    VRobot *currentRobot();

protected:
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

//    int checkTrace();
//    int checkJointsTrace();

    //! build
//    int buildTrace();

//    int buildJointsTrace();

//    int planJointsTrace();
//    int splitJointsTrace();

//    int buildHandTrace();

    //! onvert
//    int buildTpvGroups();

//    void jointsRotate( jointsTrace *pJ, int len );

Q_SIGNALS:
    void sig_joints_trace_changed();
    void sig_robo_changed( const QString &roboName );

protected Q_SLOTS:
    void slot_robo_changed( const QString &roboName );
    void slot_joints_trace_changed( );
//    void slot_download_cancel( const QString &name, int id );

    void on_btnAdd_clicked();
    void on_btnDel_clicked();
    void on_btnClr_clicked();

    void on_btnDown_clicked();
    void on_btnStart_clicked();
    void on_btnStop_clicked();

    void on_toolButton_clicked();

    void on_btnPref_clicked();

    void on_btnExport_clicked();

    void on_spinLoop_valueChanged(int arg1);

    void on_btnGraph_clicked();

    void on_btnReverse_clicked();
protected:
    void updatePlot();

protected:
    virtual void context_remove();
    virtual void context_add_before();
    virtual void context_add_below();
    virtual void context_clear();    

private:
    Ui::motionEdit *ui;

//    SinanjuMotionGroup *mMotionGroup;
    MegaTableModel *m_pMotionGroup;
    tpvPlot *m_pPlot;

    xxxGroup< tracePoint > mTracePlan;
    xxxGroup< jointsTrace > mJointsPlan;

//    ProgressGroup *m_pProgress;

    comboxDelegate *m_pActionDelegate;

    QList< tpvGroup *> mJointsTpvGroup;
//    tpvGroup mHandTpvGroup;

    MotionModel mMotionPref;
};

#endif // MOTIONEDIT_H
