#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMutex>

//! widgets
#include "../widget/megamessagebox.h"

//! uis

#include "devicemgr.h"
#include "robomgr.h"
#include "scriptmgr.h"

#include "mrqproperty.h"
#include "pvtedit.h"
#include "tpedit.h"
#include "motionedit.h"

#include "mrvprop.h"
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
#include "operatorlib.h"

#include "../com/rpcthread.h"

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

#include "../../app/comthread.h"
#include "../../app/rpcmanager.h"
#include "../../app/appmgr.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public scpiShell
{
    Q_OBJECT

public:
    explicit MainWindow(dpcObj *pObj, QWidget *parent = 0);
    ~MainWindow();

protected:
    virtual const void* loadScpiCmd();

    virtual void closeEvent( QCloseEvent *event );

    virtual void changeEvent( QEvent * event );

public:
    deviceMgr *getDeviceMgr();
    mcModel *  getMcModel();
    RpcManager *rpcMgr();

    QString exportDiagnosis( int &n );

protected:
    void init();
    void deinit();

    void setupUi();
    void loadPlugin();
    void loadPlugin( const QString &dirPath, QStringList &plugins );
    QString pluginName( const QString &fullName );

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
    void sig_post_start_app();
    void sig_robo_name_changed( const QString &name );

    void sig_com_send( const QByteArray &str );

    void sig_pref_request_save();

    void sig_view_added( const QString &str );
    void sig_view_removed( const QString &str );

protected Q_SLOTS:
    void slot_downloadbar_clicked();

    void slot_post_load_prj();
    void slot_post_start_app();
    void slot_output( QByteArray ary );

    void cfgTab_tabCloseRequested( int index );

    void on_itemXActivated( mcModelObj *pObj, mcModelObj_Op op=model_obj_op_none );
    void slot_itemModelUpdated( mcModelObj *pObj );

    void slot_itemXHelp( eItemHelp hlp, const QString &name );

    void slot_pref_request_save();

    void on_signalReport( int err, const QString &str );
    void slot_action_plugin( QAction *pAction );
    void slot_rpc_completed( RpcThread *pThread );

    void modelview_destroyed( QObject *pObj );
    void modelview_closed( QWidget *pObj );
    void slot_modelView_modified( modelView *pView, bool b );

    void slot_instmgr_changed( bool bEnd, MegaDevice::InstMgr *pMgr );

    void slot_instmgr_scene_robo_changed( MegaDevice::InstMgr *pMgr );
    void slot_instmgr_device_changed( MegaDevice::InstMgr *pMgr );

    void slot_device_active_changed( const QString &name );

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
    void slot_robo_page_changed( int page );

    void slot_device_name_changed( const QString &name );
    void slot_device_ch_index_changed( int id );
    void slot_device_page_changed( int page );

    //! tabwidget
    void slot_tabwidget_currentChanged(int index);

    void slot_scene_changed();
    void slot_download_cancel( const QString &name, int id );
public Q_SLOTS:
    void slot_com_receive( const QString &str );
    void slot_rpc_script( const QString &str );

protected Q_SLOTS:
    void slot_pref_changed();

    void slot_process_output();
    void slot_process_exit( int, QProcess::ExitStatus );

protected:
    modelView *findView( mcModelObj *pModel );
    modelView *findView( const QString &fullName );

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

    bool asurePrjValid();
    bool savePrjAs( QString &path, QString &name );
    void newMrp( QString className );

    void on_itemx_active( mcModelObj* );
    void on_itemx_new_mrp( mcModelObj* );

    void changeLang( int langIndex );

Q_SIGNALS:
    void itemXActivated( mcModelObj*, mcModelObj_Op op=model_obj_op_none );

public slots:
    void on_actionProject_triggered();
    void on_actionOpen_Prj_triggered();
    void on_actionSave_Prj_triggered();
    void on_actionSave_Prj_As_triggered();
    void on_actionClose_Prj_triggered();

    void on_actionSave_triggered();
    void on_actionSave_As_triggered();
    void on_actionSave_All_A_triggered();

    void on_actionOpen_triggered();

    void on_actionNewMotion_triggered();
    void on_actionNewPVT_triggered();
    void on_actionPT_triggered();
    void on_actionScene_triggered();
    void on_actionGraph_triggered();

    void on_actionAbout_triggered();
    void on_actionDocs_triggered();
    void on_actionPackage_triggered();
    void on_actionExample_triggered();
    void on_actionDataSet_triggered();
    void on_actionReadMe_triggered();
    void on_actionErrant_triggered();

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
    void slot_oplib_hide();

    void slot_scriptmgr_changed();

    void on_actionForceStop_triggered();
    void on_actionStop_triggered();
    void on_actionReset_triggered();

    void on_actionAngle_A_triggered();

    void on_actionDistance_D_triggered();

    void on_actionMotor_Panel_triggered();

    void on_actionClose_All_triggered();

    void on_actionImport_I_triggered();

    void on_actiontest_triggered();

private slots:
    void on_actionRun_Script_triggered();

    void on_actionTerminate_triggered();

    void on_actionConsole_triggered();

    void on_actionApp_triggered();

    void on_actionEnglish_triggered(bool checked);

    void on_actionChinese_triggered(bool checked);

    void on_actionTraditional_Chinese_triggered(bool checked);

    void on_actionOperator_Lib_triggered(bool checked);

private:
    //! uis
    Ui::MainWindow *ui;
    dpcObj *m_pDpcObj;

    QStringList mPluginList;

    //! toolbar
    QToolBar *m_pToolbarRoboConn;
    QToolBar *m_pToolbarAxesConn;
    QToolBar /**m_pBaseToolbarQuickOp,*/ *m_pToolbarQuickOp;

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

    //! operator lib
    OperatorLib *m_pOperatorLib;

    //! interrupt
    interruptThread *m_pInterruptThread;
    sampleThread *m_pSampleThread;
    RoboNetThread *m_pRoboNetThread;
    SysTimerThread *m_pSysTimerThread;
    ComThread *m_pComThread;

    //! Process
    QProcess *m_pProcess;
    QString mCurrentScript;

    RpcManager mRpcMgr;

    AppMgr mAppMgr;

    QMutex mDiagnosisMutex;
    QStringList mDiagnosisLog;

    quint64 mLastExceptionStopTs;

    QTranslator mTranslator;

    //! ui elements
    QList<QDockWidget *> mDockWidgets;
    QList<QAction *> mDockActions;
};

#endif // MAINWINDOW_H
