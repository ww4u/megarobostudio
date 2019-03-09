#ifndef MOTIONEDIT_H
#define MOTIONEDIT_H

#include <QtWidgets>

#include "../../model/sinanjumotiongroup.h"
#include "../../model/megatronmotiongroup.h"

#include "../../model/comboxdelegate.h"
#include "../../model/dspindelegate.h"
#include "../../model/checkdelegate.h"

#include "../../arith/pathplan/pathplan.h"
#include "../../arith/sinanju_split/sinanju_split.h"

#include "tableedit.h"
#include "roboaxes.h"
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
    virtual void changeEvent( QEvent *event );

public:
    virtual void settingChanged( enumSetting setting, const QVariant &v );
protected:
    virtual void onMcModelUpdated();

protected:
    void buildConnection();

public:
    virtual void setModelObj( mcModelObj *pObj );

    virtual int save( QString &outFileName );
    virtual int saveAs( QString &outFileName );

protected:
    virtual QString activeName();
//    virtual int activeAxes();

    virtual void onNetEvent(const QString &name,
                            int axes,
                            RoboMsg &msg);

    void onMotionStatus( int axes, MRQ_MOTION_STATE_2 stat );

    virtual void onRequest( RpcRequest &req );

    virtual bool context_toHere_able();
    virtual void context_toHere();

protected:
    //! dpc proc
    int postDownload( appMsg msg, void *pPara );
    void beginDownload( void *pPara);
    void endDownload( int ret, void *pPara );

    VRobot *currentRobot();

    void setExportOpt( const QStringList &optList );

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

Q_SIGNALS:
    void sig_joints_trace_changed();
    void sig_robo_changed( const QString &roboName );

protected Q_SLOTS:
    void slot_destination_changed();
    void slot_timeout();
    void slot_data_changed();
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

    void on_btnSmartEdit_toggled(bool checked);

    void on_btnGraph_clicked();

    void on_btnReverse_clicked();
    void on_btnAutoTime_clicked();

protected:
    void updatePlot();
    void tpvGroupPlot( tpvGroup *pGroup, int id );

    bool checkRobotLive( VRobot **ppRobot );

    //! post start
    int postStart( appMsg msg, void *pPara );
    void beginStart( void *pPara );
    void endStart( int ret, void *pPara );

    int doCompile();

protected:
    virtual void context_remove();
    virtual void context_add_before();
    virtual void context_add_below();
    virtual void context_clear();    

private slots:
    void on_cmbTimebase_currentIndexChanged(int index);

private:
    Ui::motionEdit *ui;

    MegaTableModel *m_pMotionGroup;
    tpvPlot *m_pPlot;

    xxxGroup< tracePoint > mTracePlan;
    xxxGroup< jointsTrace > mJointsPlan;

    comboxDelegate *m_pActionDelegate;
    dSpinDelegate *m_ptDelegate;
    checkDelegate * m_pboolDelegate;

    QList< tpvGroup *> mJointsTpvGroup;
    QList< int > mSectionList;

    MotionModel mMotionPref;

    QTimer mDlySaveTime;

    QList<int> mAligns, mDivs;
};

#endif // MOTIONEDIT_H
