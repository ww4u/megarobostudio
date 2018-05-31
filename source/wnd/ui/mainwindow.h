#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

//! widgets
#include "../widget/megamessagebox.h"

//! uis

#include "devicemgr.h"
#include "robomgr.h"
#include "scriptmgr.h"

#include "mrqproperty.h"
#include "pvtedit.h"
#include "motionedit.h"

#include "roboprop.h"

#include "cfgtab.h"

#include "syspref.h"
#include "aboutdlg.h"

#include "logout.h"
#include "roboscene.h"

#include "statebar.h"

#include "querymgr.h"

#include "eventviewer.h"
#include "scripteditor.h"
#include "dlgwndlist.h"

#include "runtool.h"
#include "roboconnection.h"
#include "axesconnection.h"

#include "warnprompt.h"
#include "anglemonitor.h"
#include "motormonitor.h"

#include "warnprompt.h"

//! models
#include "../../model/mcmodel.h"
#include "../../model/modelsyspref.h"
#include "../../model/modelrobotmgr.h"

#include "../../model/spycfgmodel.h"

//! threads
#include "../../app/dpcobj.h"
#include "../../app/interrupthandler.h"
#include "../../app/samplethread.h"
#include "../../app/robonetthread.h"
#include "../../app/systimer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(dpcObj *pObj, QWidget *parent = 0);
    ~MainWindow();

protected:
    virtual void closeEvent( QCloseEvent *event );

protected:
    void init();
    void deinit();

    void setupUi();

    void setupUi_workarea();
    void setupUi_docks();

    void setupToolbar();
    void setupStatusbar();

    void setupMenu();

    void buildConnection();

    void loadSetup();

    void setupData();
    void applyConfigs();
    void postSetup();

    void setupService();
    void stopService();

    void regSysVar();

    void statusShow( const QString &str );

Q_SIGNALS:
    void sig_post_load_prj( );
    void sig_robo_name_changed( const QString &name );


protected Q_SLOTS:
    void slot_post_load_prj();
    void cfgTab_tabCloseRequested( int index );

    void on_itemXActivated( mcModelObj *pObj );
    void slot_itemModelUpdated( mcModelObj *pObj );

    void on_signalReport( int err, const QString &str );

    void modelview_destroyed( QObject *pObj );
    void modelview_closed( QWidget *pObj );
    void slot_modelView_modified( modelView *pView, bool b );

    void slot_instmgr_changed( bool bEnd, MegaDevice::InstMgr *pMgr );

    void slot_net_event( const QString &name,
                         int axes,
                         RoboMsg msg );
    void slot_progress_para( int mi, int ma, int n, const QString &str );
    void slot_progress_visible( bool b );
    void slot_status( const QString &str );

    void slot_logout( const QString &str );
    void slot_prompt( const QString &str );

    //! connection
    void slot_robo_name_changed( const QString& );

    void slot_device_name_changed( const QString &name );
    void slot_device_ch_index_changed( int id );

    //! tabwidget
    void slot_tabwidget_currentChanged(int index);

    void slot_scene_changed();
    void slot_download_cancel( const QString &name, int id );
protected:
    modelView *findView( mcModelObj *pModel );
    modelView *createModelView( modelView *pView, mcModelObj *pModel );
    void destroyModelView( modelView *pView );

    modelView *createRoboProp( mcModelObj *pObj );

    roboScene *currentRoboScene();

    DeviceTree sceneRobotTree(  const QStringList &pathList,
                                const QStringList &fileList );

    VRoboList sceneRobotList( const QString &path,
                              const QString &name );
    void updateSceneRobot();

    void exceptionProc( const QString &name,
                        int ax,
                        int exceptionId,
                        RoboMsg &msg );

    ProgressGroup *progress();

    bool progressProc(  const QString &name,
                        int axes,
                        RoboMsg msg );

    void doLoadPrj( const QString &path,
                    const QString &name );

Q_SIGNALS:
    void itemXActivated( mcModelObj* );

private slots:
    void on_actionProject_triggered();
    void on_actionOpen_Prj_triggered();
    void on_actionSave_Prj_triggered();
    void on_actionSave_Prj_As_triggered();

    void on_actionSave_triggered();
    void on_actionSave_As_triggered();
    void on_actionSave_All_A_triggered();

    void on_actionOpen_triggered();

    void on_actionNewMotion_triggered();
    void on_actionNewPVT_triggered();
    void on_actionScene_triggered();

    void on_actionAbout_triggered();
    void on_actionDocs_triggered();
    void on_actionPackage_triggered();
    void on_actionExample_triggered();

    void on_actionpref_triggered();

    void on_actionSpy_triggered();
    void slot_spymgr_hide();

    void on_actionEvent_E_triggered();
    void slot_eventviewer_hide();

    void on_actionWindows_W_triggered(bool checked);
    void slot_wndcloseAll();
    void slot_wndcloseRobo();
    void slot_wndActive( QString str );
    void slot_wndHide();

    void slot_scriptmgr_changed();

    void on_actionForceStop_triggered();

    void on_actionReset_triggered();

    void on_actionAngle_A_triggered();

    void on_actionDistance_D_triggered();

    void on_actionMotor_Panel_triggered();

    void on_actionCamera_triggered();

    void on_actionClose_All_triggered();

    void on_actionImport_I_triggered();

    void on_actiontest_triggered();

private:
    //! uis
    Ui::MainWindow *ui;
    dpcObj *m_pDpcObj;

    //! toolbar
    QToolBar *m_pToolbarRoboConn;
    QToolBar *m_pToolbarAxesConn;
    QToolBar *m_pToolbarQuickOp;

    roboConnection *m_pRoboConnTool;
    axesConnection *m_pAxesConnTool;

    //! editors
    QList< modelView *> mModelViews;

    stateBar *m_pStateBar;

    //! docks
    roboMgr *m_pRoboMgr;
    deviceMgr *m_pDeviceMgr;
    scriptMgr *m_pScriptMgr;

    logOut *m_pLogout;

    //! modles
    mcModel mMcModel;

    modelRobotMgr mRoboModelMgr;

    //! spyCfg
    spyCfgModel *m_pSpyCfgModel;
    queryMgr *m_pSpyMgrView;

    //! event viewer
    eventViewer *m_pEventViewer;

    dlgWndList *m_pDlgWndList;

    //! warn prompt
    WarnPrompt *m_pWarnPrompt;

    //! monitor
    AngleMonitor *m_pAngleMonitor;
    AngleMonitor *m_pDistMonitor;
    MotorMonitor *m_pMotorMonitor;

    //! progress
    ProgressGroup *m_pProgress;

    //! interrupt
    interruptThread *m_pInterruptThread;
    sampleThread *m_pSampleThread;
    RoboNetThread *m_pRoboNetThread;
    SysTimerThread *m_pSysTimerThread;
};

#endif // MAINWINDOW_H
