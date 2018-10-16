#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "../../model/mcmodelobj.h"
#include "../../com/comassist.h"
#include "../../sys/sysapi.h"
#include "../../bus/filebus.h"

#include "tpedit.h"

#include "quicktool.h"
#include "dlghelp.h"

MainWindow::MainWindow(dpcObj *pObj, QWidget *parent) :
    QMainWindow(parent),
     m_pDpcObj(pObj)
{
    //! service thread
    modelView::attachDpcObj( m_pDpcObj );

    init();

    setupUi();

    setupToolbar();

    setupStatusbar();

    loadSetup();

    setupData();

    setupService();

    regSysVar();

    buildConnection();

    applyConfigs();

    postSetup();
}

MainWindow::~MainWindow()
{
    deinit();

    stopService();
}

void MainWindow::closeEvent( QCloseEvent *event )
{
    QMainWindow::closeEvent( event );

    //! gc the models
    if ( event->isAccepted() )
    {
        mcModelObj::gc();
    }
}

deviceMgr *MainWindow::getDeviceMgr()
{ return m_pDeviceMgr; }

mcModel *  MainWindow::getMcModel()
{ return &mMcModel; }

void MainWindow::init()
{
    m_pLogout = NULL;

    m_pToolbarAxesConn = NULL;
    m_pToolbarRoboConn = NULL;

    m_pToolbarQuickOp = NULL;
    m_pRoboConnTool = NULL;
    m_pAxesConnTool = NULL;

    m_pSpyCfgModel = NULL;
    m_pSpyMgrView = NULL;

    m_pDlgWndList = NULL;
    m_pWarnPrompt = NULL;

    m_pAngleMonitor = NULL;
    m_pDistMonitor = NULL;
    m_pMotorMonitor = NULL;

    m_pProgress = NULL;

    m_pInterruptThread = NULL;
    m_pSampleThread = NULL;

    m_pProcess = NULL;
}
void MainWindow::deinit()
{
    if ( NULL != m_pSpyCfgModel )
    {
        delete m_pSpyCfgModel;
    }

    if ( NULL != m_pProcess )
    {
        m_pProcess->terminate();
        delete m_pProcess;
    }

    delete ui;
}

void MainWindow::setupUi()
{
    ui = new Ui::MainWindow();
    ui->setupUi(this);

    setupUi_workarea();

    setupUi_docks();

    loadPlugin();
}

void MainWindow::loadPlugin()
{
    //! find the plugin_xxx.bat
    QString pluginPath = QCoreApplication::applicationDirPath() + QDir::separator() + QStringLiteral("package/mrq");
    QStringList pluginDirs;

    pluginDirs<<pluginPath
              <<"G:/work/mc/develope/installer/package/mrq";        //! for debug used

    foreach( QString subPath, pluginDirs )
    {
        loadPlugin( subPath, mPluginList );
    }

    //! add action
    QString menuName;
    QAction *pAction;
    foreach ( QString subStr, mPluginList )
    {
        menuName = pluginName( subStr );
        if ( menuName.size() > 0 )
        {
            pAction = ui->menuPlugin->addAction( QIcon(":/res/image/icon2/settings_light.png"),
                                       menuName
                                       );
            Q_ASSERT( NULL != pAction );
            pAction->setData( subStr );
        }
    }

    connect( ui->menuPlugin, SIGNAL(triggered(QAction*)),
             this, SLOT(slot_action_plugin(QAction*)));

    if ( mPluginList.size() > 0 )
    { ui->menuPlugin->setEnabled( true ); }
    else
    { ui->menuPlugin->setEnabled( false ); }
}

void MainWindow::loadPlugin( const QString &dirPath,
                             QStringList &plugins )
{
    QDir dir( dirPath );
    QStringList pluginFilterList;
    QStringList pluginList;
    pluginFilterList<<"plugin_*.bat";
    pluginList = dir.entryList( pluginFilterList );

    foreach( QString subStr, pluginList )
    { plugins<<QDir::toNativeSeparators( dir.absolutePath() + QDir::separator() + subStr ); }
}

QString MainWindow::pluginName( const QString &fullName )
{
    QString pureName;

    pureName = comAssist::pureFileName( fullName, false );

    QStringList segList;
    segList = pureName.split( '_', QString::SkipEmptyParts );
    if ( segList.size() == 2 && segList[1].size() >0 )
    { return segList[1]; }
    else
    { return ""; }
}

void MainWindow::setupUi_workarea()
{
}
void MainWindow::setupUi_docks()
{
    //! ---- docks
    QDockWidget *pScriptDock, *pRoboDock, *pDeviceDock, *pLogDock;

    //! left dock
    pScriptDock = new QDockWidget( tr("Project"), this );
    pScriptDock->setAllowedAreas(  Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea );
    addDockWidget( Qt::LeftDockWidgetArea, pScriptDock );

    //! left dock1
    pRoboDock = new QDockWidget( tr("Robot"), this );
    pRoboDock->setAllowedAreas(  Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea );
    addDockWidget( Qt::LeftDockWidgetArea, pRoboDock );

    //! right dock
    pDeviceDock = new QDockWidget( tr("Device"), this );
    pDeviceDock->setAllowedAreas(  Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea );
    addDockWidget( Qt::RightDockWidgetArea, pDeviceDock );

    //! logout dock
    pLogDock = new QDockWidget( tr("Log"), this );
    pLogDock->setAllowedAreas( Qt::BottomDockWidgetArea );
    addDockWidget( Qt::BottomDockWidgetArea, pLogDock );

    //! menu action
    QIcon icon;
    QAction *pAction;

    m_pScriptMgr = new scriptMgr();
    pScriptDock->setWidget( m_pScriptMgr );

    icon = QIcon( QString(":/res/image/icon/manage.png") );
    pScriptDock->setWindowIcon( icon );
    pAction = pScriptDock->toggleViewAction();

    pAction->setIcon( icon );
    pAction->setToolTip( tr("Project") );
    ui->menuView_V->addAction( pAction );

    m_pRoboMgr = new roboMgr();
    pRoboDock->setWidget( m_pRoboMgr );

    icon = QIcon( QString(":/res/image/icon/group.png") );
    pRoboDock->setWindowIcon( icon );

    pAction = pRoboDock->toggleViewAction();
    pAction->setIcon( icon );
    pAction->setToolTip( tr("Robot") );
    ui->menuView_V->addAction( pAction );

    m_pDeviceMgr = new deviceMgr();
    pDeviceDock->setWidget( m_pDeviceMgr );

    icon = QIcon( QIcon( QString(":/res/image/icon2/mobile.png") ) );
    pDeviceDock->setWindowIcon( icon );

    pAction = pDeviceDock->toggleViewAction();
    pAction->setIcon( icon );
    pAction->setToolTip( tr("Device") );
    ui->menuView_V->addAction( pAction );

    ui->menuView_V->addSeparator();

    m_pLogout = new logOut();
    pLogDock->setWidget( m_pLogout );

    icon = QIcon( QIcon( QString(":/res/image/icon2/write.png") ) );
    pLogDock->setWindowIcon( icon );

    pAction = pLogDock->toggleViewAction();
    pAction->setIcon( icon );
    pAction->setToolTip( tr("Log") );
    ui->menuView_V->addAction( pAction );

    //! menu event
    m_pMotorMonitor = new MotorMonitor(this);
    Q_ASSERT( NULL != m_pMotorMonitor );
}

void MainWindow::setupToolbar()
{
    //! file tool
    ui->mainToolBar->addAction( ui->actionOpen_Prj );
    ui->mainToolBar->addAction( ui->actionSave_Prj );
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction( ui->actionNewMotion );
    ui->mainToolBar->addAction( ui->actionNewPVT );
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction( ui->actionOpen );
    ui->mainToolBar->addAction( ui->actionSave );
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction( ui->actionpref );

    //! device conn
    m_pToolbarAxesConn = new QToolBar( tr("Device") );
    m_pAxesConnTool = new axesConnection();
    m_pToolbarAxesConn->addWidget( m_pAxesConnTool );
    addToolBar( m_pToolbarAxesConn );

    //! robo conn
    m_pToolbarRoboConn = new QToolBar( tr("Robot") );
    m_pRoboConnTool = new roboConnection();
    m_pToolbarRoboConn->addWidget( m_pRoboConnTool );
    addToolBar( m_pToolbarRoboConn );

    //! op tool
    m_pToolbarQuickOp = new QToolBar( tr("Quick") );
    m_pToolbarQuickOp->addAction( ui->actionEvent_E );
    m_pToolbarQuickOp->addWidget( new QuickTool() );        //! for the spacer
    m_pToolbarQuickOp->addAction( ui->actionReset );
    m_pToolbarQuickOp->addAction( ui->actionForceStop );

    addToolBar( m_pToolbarQuickOp );

    m_pToolbarAxesConn->setVisible( false );
    m_pToolbarRoboConn->setVisible( false );
}
void MainWindow::setupStatusbar()
{
    m_pStateBar = new stateBar();
    ui->statusBar->addWidget( m_pStateBar );
    ui->statusBar->setObjectName( tr("status") );
}

void MainWindow::setupMenu()
{
}

void MainWindow::buildConnection()
{
    connect( this, SIGNAL(sig_pref_request_save()),
             this, SLOT(slot_pref_request_save()) );

    connect( this, SIGNAL(sig_post_load_prj()),
             this, SLOT(slot_post_load_prj()) );

    //! status bar
    connect( m_pStateBar->downloadBar(), SIGNAL( signal_clicked() ),
             this, SLOT( slot_downloadbar_clicked() ) );

    //! prop
    connect( m_pDeviceMgr,
             SIGNAL(itemXActivated( mcModelObj *)),
             this,
             SLOT(on_itemXActivated( mcModelObj *)));
    connect( m_pDeviceMgr,
             SIGNAL(signal_itemXHelp(eItemHelp)),
             this,
             SLOT(slot_itemXHelp(eItemHelp)) );

    connect( m_pDeviceMgr,
             SIGNAL(signalModelUpdated(mcModelObj*)),
             this,
             SLOT(slot_itemModelUpdated(mcModelObj*)));

    connect( m_pRoboMgr,
             SIGNAL(itemXActivated(mcModelObj*)),
             this,
             SLOT(on_itemXActivated(mcModelObj*)));
    connect( m_pRoboMgr,
             SIGNAL(signal_itemXHelp(eItemHelp)),
             this,
             SLOT(slot_itemXHelp(eItemHelp)) );

    connect( m_pScriptMgr,
             SIGNAL(itemXActivated(mcModelObj*)),
             this,
             SLOT(on_itemXActivated(mcModelObj*)));
    connect( m_pScriptMgr,
             SIGNAL(signal_itemXHelp(eItemHelp)),
             this,
             SLOT(slot_itemXHelp(eItemHelp)) );

    connect( m_pScriptMgr, SIGNAL(signal_scriptmgr_changed()),
             this, SLOT(slot_scriptmgr_changed()));
    connect( m_pScriptMgr, SIGNAL(signal_prj_edited()),
             this, SLOT(on_actionSave_Prj_triggered()));
    connect( m_pScriptMgr, SIGNAL(signal_prj_edited()),
                 this, SLOT(slot_scene_changed()));

    //! new
    connect( this,
             SIGNAL(itemXActivated( mcModelObj* )),
             this,
             SLOT(on_itemXActivated(mcModelObj*)));

    //! tab close
    connect( ui->widget,
             SIGNAL(tabCloseRequested(int)),
             this,
             SLOT(cfgTab_tabCloseRequested(int)));

    connect( ui->widget,
             SIGNAL(currentChanged(int)),
             this,
             SLOT(slot_tabwidget_currentChanged(int)));

    //! device mgr error
    connect( m_pDeviceMgr,
             SIGNAL(signalReport(int, const QString &)),
             this,
             SLOT(on_signalReport(int,const QString &))
             );
    connect( m_pDeviceMgr,
             SIGNAL(signal_instmgr_changed(bool, MegaDevice::InstMgr*)),
             this,
             SLOT(slot_instmgr_changed(bool, MegaDevice::InstMgr*)) );

    connect( m_pDeviceMgr,
             SIGNAL(signal_btnState_clicked()),
             this,
             SLOT(on_actionMotor_Panel_triggered()) );

    //! event viewer
    connect( mMcModel.m_pInstMgr->getInterruptSource(),
             SIGNAL(sig_event(eventId,frameData)),
             m_pEventViewer,
             SLOT(slot_event(eventId,frameData)),
             Qt::QueuedConnection );
    connect( m_pEventViewer,
             SIGNAL(accepted()),
             this,
             SLOT(slot_eventviewer_hide()) );
    connect( m_pEventViewer,
             SIGNAL(rejected()),
             this,
             SLOT(slot_eventviewer_hide()) );

    //! robonet
    connect( m_pRoboNetThread, SIGNAL(signal_net( const QString&,int,RoboMsg)),
             this, SLOT(slot_net_event( const QString &,int,RoboMsg)) );
    connect( m_pRoboNetThread, SIGNAL(signal_progress(bool)),
             this, SLOT(slot_progress_visible(bool)));
    connect( m_pRoboNetThread, SIGNAL(signal_progress(int,int,int,const QString&)),
             this, SLOT(slot_progress_para(int,int,int,const QString&)));

    connect( m_pRoboNetThread, SIGNAL(signal_status( const QString &)),
             this, SLOT(slot_status( const QString &)) );
    connect( m_pRoboNetThread, SIGNAL(signal_logout( const QString &)),
             this, SLOT(slot_logout( const QString &)) );
    connect( m_pRoboNetThread, SIGNAL(signal_prompt( const QString &)),
             this, SLOT(slot_prompt( const QString &)));

    connect( m_pRoboNetThread, SIGNAL(signal_emergeStop()),
             this, SLOT(on_actionForceStop_triggered()));

    //! robo conn
    connect( m_pRoboConnTool->getCombName(),
             SIGNAL(currentTextChanged(const QString &)),
             this,
             SLOT(slot_robo_name_changed(const QString&))
             );
    connect( m_pRoboConnTool->getCombPage(),
             SIGNAL(currentIndexChanged(int)),
             this,
             SLOT(slot_robo_page_changed(int))
             );

    //! axes conn
    connect( m_pAxesConnTool->getCombName(),
             SIGNAL(currentTextChanged(const QString &)),
             this,
             SLOT(slot_device_name_changed(const QString&))
             );

    connect( m_pAxesConnTool->getCombCH(),
             SIGNAL(currentIndexChanged(int)),
             this,
             SLOT(slot_device_ch_index_changed(int))
             );

    connect( m_pAxesConnTool->getCombPage(),
             SIGNAL(currentIndexChanged(int)),
             this,
             SLOT(slot_device_page_changed(int))
             );

    //! motor panel
    Q_ASSERT( NULL != m_pRoboNetThread );
    Q_ASSERT( NULL != m_pMotorMonitor );
    connect( m_pRoboNetThread, SIGNAL(signal_net( const QString&,int,RoboMsg)),
             m_pMotorMonitor, SLOT(slot_net_event( const QString &,int,RoboMsg)));
}

void MainWindow::loadSetup()
{
    mMcModel.mSysPref.load( pref_file_name );

    mRoboModelMgr.load(  robo_mgr_name );

    //! default action
    EventAction *pAction = new EventAction();
    if ( NULL != pAction )
    {
        pAction->setEnable( true );
        pAction->setEvent( QStringLiteral("Lose Step") );
        pAction->setAction( QStringLiteral("Prompt") );
        pAction->setComment( tr("Lose Step") );
        mMcModel.mEventActionModel.items()->append( pAction );
    }

    pAction = new EventAction();
    if ( NULL != pAction )
    {
        pAction->setEnable( true );
        pAction->setEvent( QStringLiteral("Over Distance") );
        pAction->setAction( QStringLiteral("Prompt+Stop") );
        pAction->setComment( tr("MRX-T4 Distance warning") );
        mMcModel.mEventActionModel.items()->append( pAction );
    }

    pAction = new EventAction();
    if ( NULL != pAction )
    {
        pAction->setEnable( true );
        pAction->setEvent( QStringLiteral("Over Angle") );
        pAction->setAction( QStringLiteral("Prompt+Stop") );
        pAction->setComment( tr("MRX-T4 Angle warning") );
        mMcModel.mEventActionModel.items()->append( pAction );
    }

    m_pEventViewer = new eventViewer(
                                      &mMcModel.mEventActionModel,
                                      this );
    Q_ASSERT( NULL != m_pEventViewer );
    m_pEventViewer->setMcModel( &mMcModel );
}

void MainWindow::setupData()
{
    //! model
    mMcModel.preload();

    mMcModel.m_pInstMgr->setMainModel( &mMcModel );
    mMcModel.m_pInstMgr->setMainShell( this );
    VRobot::attachSysPara( &mMcModel.mSysPref );

    Q_ASSERT( NULL != m_pDeviceMgr );
    m_pDeviceMgr->setInstMgr( mMcModel.m_pInstMgr );
    m_pDeviceMgr->setDeviceDb( &mMcModel.mDeviceDbs );
    m_pDeviceMgr->setSysPref( &mMcModel.mSysPref );

    m_pDeviceMgr->setMcModel( &mMcModel );
    Q_ASSERT( NULL != m_pRoboMgr );
    m_pRoboMgr->setModel( mRoboModelMgr );

    Q_ASSERT( NULL != m_pMotorMonitor );
    m_pMotorMonitor->setModel( &mMcModel );

    if ( mMcModel.mSysPref.mbMaximizeStartup )
    { showMaximized(); }

    //! conn
    m_pAxesConnTool->getCombPage()->setCurrentIndex( mMcModel.mConn.mDevicePage );
    m_pRoboConnTool->getCombPage()->setCurrentIndex( mMcModel.mConn.mRoboPage );
}

void MainWindow::applyConfigs()
{
    //! remote path
    comAssist::setRemotePath( mMcModel.mSysPref.mRemoteDirPath.split(',', QString::SkipEmptyParts) );

    MegaMessageBox::setZeroAffirm( mMcModel.mSysPref.mbAffirmZero );
    VRobot::setTempPath( mMcModel.mSysPref.mDumpPath );

    //! downloader speeed
    tpvDownloader::setInterval( mMcModel.mSysPref.mTpvInterval );

    //! inst mgr
    Q_ASSERT( NULL != mMcModel.m_pInstMgr );
    mMcModel.m_pInstMgr->setTPVBase( mMcModel.mSysPref.mTimeUnit,
                                     mMcModel.mSysPref.mPosUnit,
                                     mMcModel.mSysPref.mVelUnit );

    //! sample interval
    Q_ASSERT( NULL != m_pSampleThread );
    m_pSampleThread->setSampleInterval( mMcModel.mSysPref.mSampleTick );

}

void MainWindow::postSetup()
{
    mMcModel.postload();

    //! load prj
    if ( mMcModel.mSysPref.mbAutoLoadPrj )
    { emit sig_post_load_prj(); }
}

void MainWindow::setupService()
{
//    //! load
//    mMcModel.postload();

    //! thread
    m_pInterruptThread = new interruptThread();
    Q_ASSERT( NULL != m_pInterruptThread );
    m_pInterruptThread->connectInterrupt( mMcModel.m_pInstMgr->getInterruptSource() );
    m_pInterruptThread->setInstMgr( mMcModel.m_pInstMgr );
    m_pInterruptThread->start(  );

    //! interrupt event
    frameEvent event;
    event.setEnable( true );
    event.setRepeatAble( false );
    event.setId( event_status );
    event.setMainSubCode( MRQ_mc_MOTION, MRQ_sc_MOTION_STATE_Q );
    receiveCache::setFrameEventEnable( event, true );

    //! sample thread
    m_pSampleThread = new sampleThread();
    Q_ASSERT( NULL != m_pSampleThread );

    m_pSampleThread->start( QThread::IdlePriority );
    m_pSampleThread->attachDbMeta( &mMcModel.mSysPref.mDbMeta );
    m_pSampleThread->attachInstMgr( mMcModel.m_pInstMgr );
    m_pSampleThread->setSampleInterval( mMcModel.mSysPref.mSampleTick );

    //! robo net thread
    m_pRoboNetThread = new RoboNetThread();
    Q_ASSERT( NULL != m_pRoboNetThread );
    m_pRoboNetThread->setMcModel( &mMcModel );
    m_pRoboNetThread->start();

    //! timer thread
    m_pSysTimerThread = new SysTimerThread();
    Q_ASSERT( NULL != m_pSysTimerThread );
    m_pSysTimerThread->start();

    //! com thread
    m_pComThread = new ComThread();
    Q_ASSERT( NULL != m_pComThread );
    connect( m_pComThread, SIGNAL(signal_receive(const QString &)),
             this, SLOT(slot_com_receive(const QString &)) );
    connect( this, SIGNAL(sig_com_send( const QByteArray &)),
             m_pComThread, SLOT(slot_transmit( const QByteArray &)));

    slot_pref_changed();
}

void MainWindow::stopService()
{
    //! interrupt
    Q_ASSERT( NULL != m_pInterruptThread );
    m_pInterruptThread->terminate();
    m_pInterruptThread->wait();
    delete m_pInterruptThread;
    m_pInterruptThread = NULL;

    //! sample
    m_pSampleThread->requestInterruption();
    m_pSampleThread->wait();
    delete m_pSampleThread;
    m_pSampleThread = NULL;

    //! robo net
    m_pRoboNetThread->requestInterruption();
    m_pRoboNetThread->wait();
    delete m_pRoboNetThread;
    m_pRoboNetThread = NULL;

    //! timer
    m_pSysTimerThread->sigExit();
    m_pSysTimerThread->wait();
    delete m_pSysTimerThread;
    m_pSysTimerThread = NULL;

    //! com thread
    if ( m_pComThread->isRunning() )
    {
        m_pComThread->requestInterruption();
        m_pComThread->wait();
    }
    delete m_pComThread;
}

void MainWindow::regSysVar()
{
    attachSysQueue( m_pRoboNetThread );
    attachSysPara( &mMcModel.mSysPref );
}

void MainWindow::statusShow( const QString &str )
{
    m_pStateBar->showState( str );
}

void MainWindow::slot_downloadbar_clicked()
{
    Q_ASSERT( NULL != m_pProgress );

    m_pProgress->setVisible( !m_pProgress->isVisible() );
}

void MainWindow::slot_post_load_prj()
{
    QString fullName;

    fullName = mMcModel.mSysPref.latestPrjPath() + QDir::separator() + mMcModel.mSysPref.latestPrjName();
    fullName = QDir::toNativeSeparators( fullName );
    if ( QFile::exists( fullName ) )
    {
        doLoadPrj( mMcModel.mSysPref.latestPrjPath(),
                   fullName
                 );
        sysLog( fullName );
    }
    else
    {
        sysError( fullName, tr("not exist") );
    }
}

void MainWindow::cfgTab_tabCloseRequested( int index )
{
    modelView *pView;
    pView = (modelView *)ui->widget->widget(index);
    if ( NULL == pView )
    { return; }

    if ( pView->modified() )
    {
        MegaMessageBox msgBox( tr("Setting may has been changed without apply, confirm to close?") );
        if ( msgBox.exec() == QMessageBox::Ok )
        {}
        else
        { return; }
    }
    else
    {}

    //! close the tab
    ui->widget->widget(index)->close();
    ui->widget->removeTab( index );
}

//! show model
void MainWindow::on_itemXActivated( mcModelObj *pObj )
{
    if ( NULL == pObj )
    { return; }

    //! find view
    modelView *pView = findView( pObj );logDbg();
    if ( NULL != pView )
    {
        int index;
        index = ui->widget->indexOf( pView );
        if ( index >= 0 )
        {
            ui->widget->setCurrentWidget( pView );
            return;
        }
        //! no view
        else
        {
            Q_ASSERT( false );
            return;
        }
    }

    //! new view

    //! tpv
    if ( pObj->getType() == mcModelObj::model_tpv )
    {
        pvtEdit *pEdit;
        pEdit = new pvtEdit();
        Q_ASSERT( NULL != pEdit );

        createModelView( pEdit, pObj );
    }

    else if ( pObj->getType() == mcModelObj::model_tp )
    {
        TpEdit *pEdit;
        pEdit = new TpEdit();
        Q_ASSERT( NULL != pEdit );

        createModelView( pEdit, pObj );
    }

    //! motion
    else if ( pObj->getType() == mcModelObj::model_motion_file )
    {
        motionEdit *pMotionEdit;
        pMotionEdit = new motionEdit();logDbg();
        Q_ASSERT( NULL != pMotionEdit );logDbg();

        createModelView( pMotionEdit, pObj );logDbg();

        connect( this, SIGNAL(sig_robo_name_changed(const QString&)),
                 pMotionEdit, SLOT(slot_robo_changed( const QString&)));
        emit sig_robo_name_changed( mMcModel.mConn.getRoboName() );
    }

    //! from robo mgr
    else if ( pObj->getType() == mcModelObj::model_robot )
    {
        roboScene *pRoboScene = currentRoboScene();
        if ( NULL != pRoboScene )
        {
            pRoboScene->addRobot(pObj);
            Q_ASSERT( NULL != pRoboScene->getModelObj() );
            pObj->setPath( pRoboScene->getModelObj()->getPath() );
        }
        else
        { delete pObj; }
    }

    //! setup from scene
    else if ( pObj->getType() == mcModelObj::model_scene_variable
              || pObj->getType() == mcModelObj::model_device
              || pObj->getType() == mcModelObj::model_composite_device
              )
    {
        pView = createRoboProp( pObj );
        Q_ASSERT( NULL != pView );
        pView->setActive();
    }

    //! scene file
    else if ( pObj->getType() == mcModelObj::model_scene_file )
    {logDbg();
        roboScene *pWorkScene;
        pWorkScene = new roboScene();
        Q_ASSERT( NULL != pWorkScene );

        createModelView( pWorkScene, pObj );

        connect( pWorkScene,
                 SIGNAL(itemXActivated(mcModelObj*)),
                 this,
                 SLOT(on_itemXActivated(mcModelObj*)));

        connect( pWorkScene,
                 SIGNAL(signalSceneChanged()),
                 this,
                 SLOT(slot_scene_changed()) );
    }

    //! py file
    else if ( pObj->getType() == mcModelObj::model_py_file )
    {
        scriptEditor *pScriptEditor;
        pScriptEditor = new scriptEditor();
        Q_ASSERT( NULL != pScriptEditor );

        createModelView( pScriptEditor, pObj );
    }
    else
    {}
}

//! model updated
void MainWindow::slot_itemModelUpdated( mcModelObj *pObj )
{
    Q_ASSERT( NULL != pObj );

    for ( int i = 0; i < ui->widget->count(); i++ )
    {
        Q_ASSERT( NULL != ui->widget->widget(i) );
        //! match model
        if ( pObj == ((modelView*)ui->widget->widget(i))->getModelObj() )
        {
            ((modelView*)ui->widget->widget(i))->updateScreen();
        }
    }
}

void MainWindow::slot_itemXHelp( eItemHelp hlp )
{
    if ( mMcModel.mSysPref.mbShowHelp )
    {
        DlgHelp help( this );

        //! on help ic
        switch( hlp )
        {
        case e_help_new_robot:
            help.showHelp( "", (":/res/image/help/new robo.png") );
            break;

        default:
            return;
        }

        help.exec();

        mMcModel.mSysPref.mbShowHelp = help.showAgain();

        emit sig_pref_request_save();
    }
    else
    {}
}

void MainWindow::slot_pref_request_save()
{
    //! save pref
    mMcModel.mSysPref.save( pref_file_name );
}

void MainWindow::on_signalReport( int err, const QString &str )
{
    m_pStateBar->showState( QString("%1:%2").arg(err).arg(str) );
}

void MainWindow::slot_action_plugin( QAction *pAction )
{
   QStringList args;
   QString str = pAction->data().toString();
   str.replace("/", QDir::separator() );
   args<<"/c";
   args<<str;

   //! \todo linux
   RpcThread *pThread = new RpcThread( "cmd", args );
   if ( NULL != pThread )
   {
       connect( pThread, SIGNAL(signal_completed(RpcThread*)),
                this, SLOT(slot_rpc_completed(RpcThread*)) );

       pThread->start();
   }
}

void MainWindow::slot_rpc_completed( RpcThread *pThread )
{
    delete pThread;
}

void MainWindow::modelview_destroyed( QObject *pObj )
{
    Q_ASSERT( NULL != pObj );

    destroyModelView( (modelView*)pObj );
}

void MainWindow::modelview_closed( QWidget *pObj )
{
    int index;
    index = ui->widget->indexOf( pObj );

    cfgTab_tabCloseRequested( index );
}

void MainWindow::slot_modelView_modified( modelView *pView,
                                          bool b )
{
    int index;
    index = ui->widget->indexOf( pView );
    if ( index >= 0 )
    {
        QString name = ui->widget->tabText(index);
        if ( b )
        {
            if ( name.contains("*") )
            {}
            else
            { name.append("*"); }
        }
        else
        {  name.remove('*'); }

        ui->widget->setTabText( index, name );
    }
}

//! sample thread do not use the direct pointer
void MainWindow::slot_instmgr_changed( bool bEnd, MegaDevice::InstMgr *pMgr )
{
    Q_ASSERT( NULL != pMgr );

    //! close robo/device setting tab
    slot_wndcloseRobo();

    if ( !bEnd )
    {
        m_pSampleThread->clear();
        sysLog( tr("samples cleared!") );
    }
    //! add again
    else
    {
        //! \note for debug only
//        m_pSampleThread->attachSampleProxy( MegaDevice::MRQ_model::REPORT_STATE_TORQUE,
//                                            pMgr->getDevice(0)
//                                            );
    }

    if ( bEnd )
    {
        //! add resrc
        Q_ASSERT( NULL != pMgr );
        m_pAxesConnTool->setDeviceNames( pMgr->getResources() );

        //! add robot
        QStringList strList;
        strList = mMcModel.m_pInstMgr->roboResources();
        m_pRoboConnTool->setRoboNames( strList );

        //! reset monitors
        m_pMotorMonitor->setMonitors( mMcModel.m_pInstMgr->getChans() );
    }
}

//! device changed
void MainWindow::slot_device_active_changed( const QString &name )
{
    //! change the device name
    if ( name.length() > 0 )
    {
        m_pAxesConnTool->setCurrentName( name );
    }
}

void MainWindow::slot_net_event(
                                const QString &name,
                                int axes,
                                RoboMsg msg )
{
    //! event id, frame id, byte array
    if ( msg.getMsg() == e_interrupt_occuring )
    {
        int eId;
        eId = msg.at(0).toInt();
        if ( eId >= event_exception_min
             && eId < event_exception_max )
        {
            exceptionProc( name, axes, eId, msg );
        }
        else
        {

        }

        return;
    }

    //! progress
    if ( progressProc(name,axes,msg) )
    { return; }
}

void MainWindow::slot_progress_para( int mi, int ma, int n, const QString &str )
{
    Q_ASSERT( NULL != m_pStateBar );
    m_pStateBar->progressBar()->setRange( mi, ma );
    m_pStateBar->progressBar()->setValue( n );

    m_pStateBar->progressInfo()->setText( str );
}
void MainWindow::slot_progress_visible( bool b )
{
    Q_ASSERT( NULL != m_pStateBar );

    if ( b  )
    {
        if ( m_pStateBar->progressInfo()->isHidden() )
        {
            QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        }
    }
    else
    {
        if ( m_pStateBar->progressInfo()->isVisible() )
        {
            QApplication::restoreOverrideCursor();
        }
    }

    m_pStateBar->progressBar()->setVisible( b );
    m_pStateBar->progressInfo()->setVisible( b );
}

void MainWindow::slot_status( const QString &str )
{
    Q_ASSERT( NULL != m_pStateBar );
    m_pStateBar->statusLabel()->setText( str );
}

void MainWindow::slot_logout( const QString &str )
{
    Q_ASSERT( NULL != m_pLogout );
    m_pLogout->logIn(str);
}

void MainWindow::slot_prompt( const QString &str )
{
    if ( m_pWarnPrompt == NULL  )
    { m_pWarnPrompt = new WarnPrompt(this); }

    if ( NULL == m_pWarnPrompt )
    { return; }

    m_pWarnPrompt->addInfo( str );
    m_pWarnPrompt->show();
    m_pWarnPrompt->activateWindow();
}

void MainWindow::slot_robo_name_changed( const QString &name )
{
    mMcModel.mConn.setRoboName( name );
    logDbg()<<name;

    emit sig_robo_name_changed( name );
}

void MainWindow::slot_robo_page_changed( int page )
{
    mMcModel.mConn.setRoboPage( page );
}

void MainWindow::slot_device_name_changed( const QString &name )
{
    mMcModel.mConn.setDeviceName( name );
}

void MainWindow::slot_device_ch_index_changed( int id )
{
    mMcModel.mConn.setDeviceCH( id );
}

void MainWindow::slot_device_page_changed( int page )
{ mMcModel.mConn.setDevicePage(page); }

void MainWindow::slot_tabwidget_currentChanged(int index)
{
    //! disable
    m_pToolbarAxesConn->setVisible( false );
    m_pToolbarRoboConn->setVisible( false );

    m_pRoboMgr->setOperable( false );

    //! enabled
    ui->actionRun_Script->setEnabled( false );

    //! to enable
    modelView *pViewModel;
    pViewModel = (modelView*)ui->widget->widget(index);
    if ( NULL == pViewModel )
    { return; }

    pViewModel->setActive();

    Q_ASSERT( pViewModel->getModelObj() != NULL );
    mcModelObj::obj_type objType = pViewModel->getModelObj()->getType();
    if (  objType == mcModelObj::model_tpv
          || objType == mcModelObj::model_tp
          || objType == mcModelObj::model_device )
    {
        m_pToolbarAxesConn->setVisible( true );
    }
    else if ( objType == mcModelObj::model_motion_file )
    {
        m_pToolbarRoboConn->setVisible( true );
    }
    else if ( objType == mcModelObj::model_scene_variable )
    {
        m_pToolbarAxesConn->setVisible( true );
    }
    else if ( objType == mcModelObj::model_scene_file )
    {
        m_pRoboMgr->setOperable( true );
    }
    else if ( objType == mcModelObj::model_py_file )
    {
        mCurrentScript = pViewModel->getModelObj()->getPath() + QDir::separator() + pViewModel->getModelObj()->getName();
        ui->actionRun_Script->setEnabled( true );
    }
    else
    {}

    //! check filled
    if ( objType == mcModelObj::model_device )
    {
        if ( pViewModel->getModelObj()->isFilled() )
        {}
        else
        {
            MegaMessageBox::information( this, tr("Info"), tr("Device setting has not been updated") );
        }
    }
}

modelView *MainWindow::findView( mcModelObj *pModel )
{
    foreach( modelView *pView, mModelViews )
    {
        Q_ASSERT( NULL != pView );

        //! model in view?
        if ( pView->getModelObj() == pModel )
        { return pView; }

        //! model match?
        if ( pView->matchModelObj( pModel ) )
        {
            delete pModel;
            return pView;
        }
    }

    return NULL;
}

modelView *MainWindow::createModelView( modelView *pView,
                                        mcModelObj *pObj )
{
    Q_ASSERT( pView != NULL );
    Q_ASSERT( pObj != NULL );

    pView->setMcModel( &mMcModel );
    pView->setModelObj( pObj );

    ui->widget->addTab( pView, comAssist::pureFileName(pObj->getName()) );
    ui->widget->setCurrentWidget( pView );

    //! wnd manage
    connect( pView,
             SIGNAL(destroyed(QObject*)),
             this,
             SLOT(modelview_destroyed(QObject*)) );

    connect( pView,
             SIGNAL(sigClose(QWidget *)),
             this,
             SLOT(modelview_closed(QWidget*)) );logDbg();

    connect( pView,
             SIGNAL(sigSaveRequest(QWidget * )),
             this,
             SLOT(on_actionSave_triggered()) );

    //! modify manage
    connect( pView,
             SIGNAL(sigModified(modelView*,bool)),
             this,
             SLOT(slot_modelView_modified(modelView*,bool)));

    //! state manage
    Q_ASSERT( NULL != m_pRoboNetThread );
    connect( m_pRoboNetThread,
             SIGNAL(signal_net(const QString &,int,RoboMsg)),
             pView,
             SLOT(slot_net_event(const QString &,int,RoboMsg)));

    connect( m_pRoboNetThread,
             SIGNAL(signal_request( const RpcRequest&)),
             pView,
             SLOT(slot_request( const RpcRequest &)));

    mModelViews.append( pView );logDbg();

    return pView;
}

void MainWindow::destroyModelView( modelView *pView )
{
    Q_ASSERT( NULL != pView );
    mModelViews.removeAll( pView );
}

modelView *MainWindow::createRoboProp( mcModelObj *pObj )
{
    Q_ASSERT( NULL != pObj );

    VRobot *pBase = (VRobot*)pObj;
    Q_ASSERT( NULL != pBase );

    //! mrq
    if ( robot_is_mrq( pBase->getId() ) )
    {
        mrqProperty *pProp;
        pProp = new mrqProperty( pBase );
        Q_ASSERT( NULL != pProp );
        createModelView( pProp, pBase );        //! device is a model

        connect( pProp, SIGNAL(sigActiveDeviceChanged(const QString &)),
                 this, SLOT(slot_device_active_changed(const QString &)));

        return pProp;
    }
    //! mrv
    else if ( robot_is_mrv( pBase->getId() ) )
    {
        MrvProp *pmrvProp;
        pmrvProp = new MrvProp( pBase );

        Q_ASSERT( NULL != pmrvProp );
        createModelView( pmrvProp, pBase );

        connect( pmrvProp, SIGNAL(sigActiveDeviceChanged(const QString &)),
                 this, SLOT(slot_device_active_changed(const QString &)));

        return pmrvProp;
    }
    //! robot
    else if ( robot_is_robot( pBase->getId() ) )
    {
        roboProp *pProp;
        pProp = new roboProp( pBase->getId() );
        Q_ASSERT( NULL != pProp );

        //! set instmgr
        ((VRobot*)pObj)->setInstMgr( mMcModel.m_pInstMgr );

        createModelView( pProp, pObj );

        return pProp;
    }
    else
    { return NULL; }
}

roboScene *MainWindow::currentRoboScene()
{
    //! get widget
    QWidget *pWidget = ui->widget->currentWidget();
    if ( NULL == pWidget )
    { return NULL; }

    //! get type
    modelView *pModelView = (modelView*)pWidget;
    if ( pModelView->getModelObj()->getType() != mcModelObj::model_scene_file )
    { return NULL; }

    return (roboScene*)pModelView;
}

void MainWindow::on_actionSpy_triggered()
{
    //! create the config
    if ( NULL == m_pSpyCfgModel )
    {
        m_pSpyCfgModel = new spyCfgModel;

        m_pSpyMgrView = new queryMgr(this);
        m_pSpyMgrView->setModal( m_pSpyCfgModel );

        connect( m_pSpyMgrView, SIGNAL(accepted()),
                 this,
                 SLOT(slot_spymgr_hide()));
        connect( m_pSpyMgrView, SIGNAL(rejected()),
                 this,
                 SLOT(slot_spymgr_hide()));

        m_pSpyMgrView->attachSampleThread( m_pSampleThread );

        connect( m_pSampleThread, SIGNAL(sigSpyUpdated( const QString &,int,int)),
                 m_pSpyMgrView, SLOT(slot_spy_changed( const QString&,int,int)));
    }

    //! attach mgr
    m_pSpyMgrView->attachInstmgr( m_pDeviceMgr->m_pMgr );

    m_pSpyMgrView->setVisible( !m_pSpyMgrView->isVisible() );
    ui->actionSpy->setChecked( m_pSpyMgrView->isVisible() );
}

void MainWindow::slot_spymgr_hide()
{
    ui->actionSpy->setChecked( m_pSpyMgrView->isVisible() );
}

void MainWindow::on_actionEvent_E_triggered()
{
    m_pEventViewer->setVisible( !m_pEventViewer->isVisible() );
    ui->actionEvent_E->setChecked( m_pEventViewer->isVisible() );
}

void MainWindow::slot_eventviewer_hide()
{
    ui->actionEvent_E->setChecked( m_pEventViewer->isVisible() );
}

void MainWindow::on_actionWindows_W_triggered(bool checked)
{
    //! create the item
    if ( m_pDlgWndList == NULL )
    {
        m_pDlgWndList = new dlgWndList(this);

        connect( m_pDlgWndList, SIGNAL(signal_closeAll()),
                 this, SLOT(slot_wndcloseAll()) );
        connect( m_pDlgWndList, SIGNAL(signal_activeWnd(QString)),
                 this, SLOT(slot_wndActive(QString)));

        connect( m_pDlgWndList, SIGNAL(accepted()),
                 this, SLOT(slot_wndHide()));
        connect( m_pDlgWndList, SIGNAL(rejected()),
                 this, SLOT(slot_wndHide()));
    }

    m_pDlgWndList->clear();

    //! set data
    QStringList strList;
    for ( int i = 0; i <  ui->widget->count(); i++ )
    {
        strList<<ui->widget->tabText(i);
    }
    m_pDlgWndList->addWnd( strList );

    //! toggle visible
    m_pDlgWndList->setVisible( !m_pDlgWndList->isVisible() );
}

void MainWindow::slot_wndcloseAll()
{
    ui->widget->clear();

    foreach ( modelView *pView, mModelViews )
    {
        Q_ASSERT( NULL != pView );
        pView->close();
    }
    mModelViews.clear();
}

//! close robo,device
void MainWindow::slot_wndcloseRobo()
{
    mcModelObj *pObj;
    modelView *pView;

    //! get the view
    for ( int i = 0 ;i < ui->widget->count(); i++ )
    {
        pView = (modelView*)ui->widget->widget( i );
        if ( pView != NULL )
        {
            pObj = pView->getModelObj();
            Q_ASSERT( NULL != pObj );
            //! for mrq/robo setting
            if ( pObj->getType() == mcModelObj::model_scene_variable
                 || pObj->getType() == mcModelObj::model_device
                 || pObj->getType() == mcModelObj::model_composite_device )
            {
                pView->close();
                mModelViews.removeAll( pView );
            }
            else
            {}
        }
        else
        {
            Q_ASSERT( false );
        }
    }
}

//! active window
void MainWindow::slot_wndActive( QString str )
{
    for( int i = 0; i < ui->widget->count(); i++ )
    {
        if ( str_is( ui->widget->tabText(i), str ) )
        {
            ui->widget->setCurrentWidget( ui->widget->widget(i) );
        }
    }
}

void MainWindow::slot_wndHide()
{
    if ( NULL != m_pDlgWndList )
    {
        ui->actionWindows_W->setChecked( false );
    }
}

void MainWindow::slot_scriptmgr_changed()
{
    slot_scene_changed();
}

void MainWindow::on_actionForceStop_triggered()
{
    mMcModel.resetCommunicate();

    RoboTask::killAll();

    slot_pref_changed();

    if ( NULL != m_pDeviceMgr->m_pMgr )
    { m_pDeviceMgr->m_pMgr->emergencyStop(); }

    Q_ASSERT( NULL != m_pSysTimerThread );
    m_pSysTimerThread->stopAll();
}

void MainWindow::on_actionReset_triggered()
{
    if ( NULL != m_pDeviceMgr->m_pMgr )
    { m_pDeviceMgr->m_pMgr->hardReset(); }

    m_pSysTimerThread->stopAll();
}

void MainWindow::on_actionAngle_A_triggered()
{
    if ( NULL == m_pAngleMonitor )
    {
        m_pAngleMonitor = new AngleMonitor(this);

        //! fail to open window
        if ( NULL == m_pAngleMonitor )
        { return; }

        m_pAngleMonitor->setWindowTitle( tr("Angle") );

        QMap<int, QString> subMap;
        subMap[ MRQ_DATA_XANGLE ] = tr("Increase Angle");
        subMap[ MRQ_DATA_ABSANGLE ] = tr("Absolute Angle");
        m_pAngleMonitor->setDataIds( subMap );
    }

    m_pAngleMonitor->show();

    //! set connection
    QStringList strList = mMcModel.m_pInstMgr->getChans();

    m_pAngleMonitor->setModel( &mMcModel );
    m_pAngleMonitor->setConnections( strList );
}

void MainWindow::on_actionDistance_D_triggered()
{
    if ( NULL == m_pDistMonitor )
    {
        m_pDistMonitor = new AngleMonitor(this);

        //! fail to open window
        if ( NULL == m_pDistMonitor )
        { return; }

        m_pDistMonitor->setWindowTitle( tr("Distance") );
        m_pDistMonitor->setDataId( MRQ_DATA_DISTANCE );
    }

    m_pDistMonitor->show();
    //! set connection

    QStringList strList = mMcModel.m_pInstMgr->getChans();

    m_pDistMonitor->setModel( &mMcModel );
    m_pDistMonitor->setConnections( strList );
}

void MainWindow::on_actionMotor_Panel_triggered()
{
    if ( NULL == m_pMotorMonitor )
    { return; }

    if ( m_pMotorMonitor->isVisible() )
    {
        m_pMotorMonitor->hide();
    }
    else
    {
        m_pMotorMonitor->show();
//        m_pMotorMonitor->activateWindow();
    }
}

//void MainWindow::on_actionCamera_triggered()
//{
//    QString app;
//    app = QCoreApplication::applicationDirPath() + QDir::separator() + QStringLiteral("camera.exe");
//    //! \todo linux
//    QProcess::execute( "\"" + app + "\"" );
//}

void MainWindow::on_actionClose_All_triggered()
{
    slot_wndcloseAll();
}

void MainWindow::on_actionImport_I_triggered()
{
    Q_ASSERT( NULL != m_pScriptMgr );
    m_pScriptMgr->slot_context_import();
}

void MainWindow::on_actiontest_triggered()
{
//    QFile qss( "G:/work/mc/develope/source/wnd/res/qss/mega.qss" );
//    qss.open(QFile::ReadOnly);
//    qApp->setStyleSheet(qss.readAll());
//    qss.close();
logDbg();
//    QProcess *pProcess = new QProcess();
    if ( NULL == m_pProcess )
    {
        m_pProcess = new QProcess();
        connect( m_pProcess, SIGNAL(readyReadStandardOutput()),
                 this, SLOT(slot_process_output()) );

        connect( m_pProcess, SIGNAL(readyReadStandardError()),
                 this, SLOT(slot_process_output()) );
    }

    QStringList strList;
//    strList<<"-c"<<"print(\"hello\")";
//    m_pProcess->execute( "python", strList );

    strList<<"G:/study/py/hello.py";
    m_pProcess->start( "python", strList );

//    logDbg()<<pProcess->readAllStandardOutput();
//    logDbg()<<pProcess->readAllStandardError();
//    delete pProcess;

}

void MainWindow::slot_com_receive( const QString &str )
{
    //! find device and do
    if ( str.length() < 3 )
    {
        sysError( tr("Invalid cmd"), str );
        return;
    }

    //! get name
    QStringList secList = str.split( " ", QString::SkipEmptyParts );
    if ( secList.size() < 2 )
    {
        sysError( tr("Invalid cmd: format"), str );
        return;
    }

    scpiShell *pShell = mMcModel.m_pInstMgr->findShell( secList.at(0) );
    if ( NULL == pShell )
    {
        sysError( tr("Invalid cmd: device name"), str );
        return;
    }

    //! remove the device name
    QString strFullCmd = str;
    strFullCmd = strFullCmd.remove( 0, secList.at(0).size() );
    strFullCmd.append( "\r\n" );

    pShell->write( strFullCmd.toUtf8().data(), strFullCmd.length() );
    //! read
    {
        int retSize = pShell->size();
        if ( retSize > 0 )
        {
            char *pOut = new char[ retSize ];
            Q_ASSERT( NULL != pOut );
            if ( retSize == pShell->read( pOut, retSize ) )
            {
                QByteArray ary( pOut, retSize );
                emit sig_com_send( ary );
            }
            else
            {}

            delete []pOut;
        }
    }
}

void MainWindow::slot_pref_changed()
{
    Q_ASSERT( NULL != m_pComThread );

    if ( m_pComThread->isRunning() )
    {
        m_pComThread->requestInterruption();
        m_pComThread->wait();
    }

    if ( mMcModel.mSysPref.mComEn )
    {
        m_pComThread->setPort( mMcModel.mSysPref.mComName );
        m_pComThread->start();
    }
}

void MainWindow::slot_process_output()
{
    Q_ASSERT( NULL != m_pProcess );
    {
        sysLog( m_pProcess->readAll() );
    }
}

void MainWindow::slot_process_exit( int code, QProcess::ExitStatus stat )
{
    sysLog( tr("script complted") + QString::number( code ), QString::number( (int)stat ) );
    ui->actionTerminate->setEnabled( false );
    ui->actionRun_Script->setEnabled( true );
}
