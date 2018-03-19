#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "../../model/mcmodelobj.h"
#include "../../com/comassist.h"
#include "../../sys/sysapi.h"
#include "../../bus/filebus.h"



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

    m_pInterruptThread = NULL;
}
void MainWindow::deinit()
{
    if ( NULL != m_pSpyCfgModel )
    {
        delete m_pSpyCfgModel;
    }

    delete ui;
}

void MainWindow::setupUi()
{
    ui = new Ui::MainWindow();
    ui->setupUi(this);

    setupUi_workarea();

    setupUi_docks();
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
    m_pEventViewer = new eventViewer( mMcModel.m_pInstMgr->getInterruptSource(),
                                      &mMcModel.mEventActionModel,
                                      this );
}

void MainWindow::setupToolbar()
{
    //! file tool
    ui->mainToolBar->addAction( ui->actionOpen_Prj );
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction( ui->actionOpen );
    ui->mainToolBar->addAction( ui->actionSave );

    //! device conn
    m_pToolbarAxesConn = new QToolBar();
    m_pAxesConnTool = new axesConnection();
    m_pToolbarAxesConn->addWidget( m_pAxesConnTool );
    addToolBar( m_pToolbarAxesConn );

    //! robo conn
    m_pToolbarRoboConn = new QToolBar();
    m_pRoboConnTool = new roboConnection();
    m_pToolbarRoboConn->addWidget( m_pRoboConnTool );
    addToolBar( m_pToolbarRoboConn );

    //! op tool
    m_pToolbarQuickOp = new QToolBar();
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
    //! prop
    connect( m_pDeviceMgr,
             SIGNAL(itemXActivated( mcModelObj *)),
             this,
             SLOT(on_itemXActivated( mcModelObj *)));

    connect( m_pRoboMgr,
             SIGNAL(itemXActivated(mcModelObj*)),
             this,
             SLOT(on_itemXActivated(mcModelObj*)));

    connect( m_pScriptMgr,
             SIGNAL(itemXActivated(mcModelObj*)),
             this,
             SLOT(on_itemXActivated(mcModelObj*)));

    connect( m_pScriptMgr, SIGNAL(signal_scriptmgr_changed()),
             this, SLOT(slot_scriptmgr_changed()));

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

    //! event viewer
    connect( mMcModel.m_pInstMgr->getInterruptSource(),
             SIGNAL(sig_event(eventId,frameData)),
             m_pEventViewer,
             SLOT(slot_event(eventId,frameData)));
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


    //! robo conn
    connect( m_pRoboConnTool->getCombName(),
             SIGNAL(currentTextChanged(const QString &)),
             this,
             SLOT(slot_robo_name_changed(const QString&))
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
}

void MainWindow::loadSetup()
{
    mMcModel.mSysPref.load( pref_file_name );

    mRoboModelMgr.load(  robo_mgr_name );
}

void MainWindow::setupData()
{
    //! model
    mMcModel.preload();

    mMcModel.m_pInstMgr->setMainModel( &mMcModel );

    m_pDeviceMgr->setInstMgr( mMcModel.m_pInstMgr );
    m_pDeviceMgr->setDeviceDb( &mMcModel.mDeviceDbs );
    m_pDeviceMgr->setSysPref( &mMcModel.mSysPref );

    m_pDeviceMgr->setMcModel( &mMcModel );

    m_pRoboMgr->setModel( mRoboModelMgr );

}

void MainWindow::setupService()
{
    //! load
    mMcModel.postload();

    //! thread
    m_pInterruptThread = new interruptThread();
    Q_ASSERT( NULL != m_pInterruptThread );
    m_pInterruptThread->connectInterrupt( mMcModel.m_pInstMgr->getInterruptSource() );
    m_pInterruptThread->start( QThread::TimeCriticalPriority );
    m_pInterruptThread->setInstMgr( mMcModel.m_pInstMgr );

    //! interrupt event
    frameEvent event;
    event.setEnable( true );
    event.setId( event_status );
    event.setMainSubCode( mc_MOTION, sc_MOTION_STATE_Q );
    mMcModel.m_pInstMgr->getInterruptSource()->setFrameEventEnable( event, true );

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
    m_pSampleThread->terminate();
    m_pSampleThread->wait();
    delete m_pSampleThread;
    m_pSampleThread = NULL;

    //! robo net
    m_pRoboNetThread->terminate();
    m_pRoboNetThread->wait();
    delete m_pRoboNetThread;
    m_pRoboNetThread = NULL;

    //! timer
//    m_pSysTimerThread->quit();
//    m_pSysTimerThread->wait();
    m_pSysTimerThread->sigExit();
    m_pSysTimerThread->wait();
    delete m_pSysTimerThread;
    m_pSysTimerThread = NULL;
}

void MainWindow::regSysVar()
{
    attachSysLog( m_pLogout );
    attachSysQueue( m_pRoboNetThread );
}

void MainWindow::statusShow( const QString &str )
{
    m_pStateBar->showState( str );
}

void MainWindow::cfgTab_tabCloseRequested( int index )
{
    ui->widget->widget(index)->close();
    ui->widget->removeTab( index );
}

//! show model
void MainWindow::on_itemXActivated( mcModelObj *pObj )
{
    if ( NULL == pObj )
    { return; }

    logDbg()<<QString::number( (quint32)pObj, 16 );

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
    {logDbg();
        pvtEdit *pEdit;
        pEdit = new pvtEdit();
        Q_ASSERT( NULL != pEdit );

        createModelView( pEdit, pObj );
    }

    //! motion
    else if ( pObj->getType() == mcModelObj::model_motion_file )
    {logDbg();
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
    {logDbg();
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
              )
    {logDbg();
        createRoboProp( pObj );
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
    }

    //! py file
    else if ( pObj->getType() == mcModelObj::model_py_file )
    {logDbg();
        scriptEditor *pScriptEditor;
        pScriptEditor = new scriptEditor();
        Q_ASSERT( NULL != pScriptEditor );

        createModelView( pScriptEditor, pObj );
    }
    else
    {}
    logDbg();
}

void MainWindow::on_signalReport( int err, const QString &str )
{
    m_pStateBar->showState( QString("%1:%2").arg(err).arg(str) );
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
    if ( index >= 0 )
    {
        ui->widget->widget( index )->close();
        ui->widget->removeTab( index );
    }
}

//! sample thread do not use the direct pointer
void MainWindow::slot_instmgr_changed( bool bEnd, MegaDevice::InstMgr *pMgr )
{
    logDbg()<<bEnd;
    Q_ASSERT( NULL != pMgr );

    if ( !bEnd )
    {
        m_pSampleThread->clear();
        sysLog("samples cleared!");
    }
    //! add again
    else
    {
        //! \todo for debug only
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
    }
}

void MainWindow::slot_net_event( const QString &name,
                     int axes,
                     RoboMsg msg )
{
    //! event id, frame id, byte array
    if ( msg.getMsg() == e_interrupt_occuring )
    {
        logDbg()<<name;

        int eId;
        eId = msg.at(0).toInt();
        if ( eId >= event_exception_min
             && eId < event_exception_max )
        {
            exceptionProc( name, eId, msg );
        }
    }
}

void MainWindow::slot_robo_name_changed( const QString &name )
{
    mMcModel.mConn.setRoboName( name );
    logDbg()<<name;

    emit sig_robo_name_changed( name );
}
void MainWindow::slot_device_name_changed( const QString &name )
{
    mMcModel.mConn.setDeviceName( name );
    logDbg()<<name;
}

void MainWindow::slot_device_ch_index_changed( int id )
{
    mMcModel.mConn.setDeviceCH( id );
    logDbg()<<id;
}

void MainWindow::slot_tabwidget_currentChanged(int index)
{
    //! disable
    m_pToolbarAxesConn->setVisible( false );
    m_pToolbarRoboConn->setVisible( false );

    m_pRoboMgr->setEnabled( false );

    //! to enable
    modelView *pViewModel;
    pViewModel = (modelView*)ui->widget->widget(index);
    if ( NULL == pViewModel )
    { return; }

    Q_ASSERT( pViewModel->getModelObj() != NULL );
    mcModelObj::obj_type objType = pViewModel->getModelObj()->getType();
    if (  objType == mcModelObj::model_tpv
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
        m_pRoboMgr->setEnabled( true );
    }
    else
    {}
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

    pView->setMcModel( &mMcModel );logDbg();
    pView->setModelObj( pObj );logDbg();

    ui->widget->addTab( pView, comAssist::pureFileName(pObj->getName()) );logDbg();
    ui->widget->setCurrentWidget( pView );logDbg();
    //! wnd manage
    connect( pView,
             SIGNAL(destroyed(QObject*)),
             this,
             SLOT(modelview_destroyed(QObject*)) );logDbg();

    connect( pView,
             SIGNAL(sigClose(QWidget *)),
             this,
             SLOT(modelview_closed(QWidget*)) );logDbg();

    //! state manage
    Q_ASSERT( NULL != m_pRoboNetThread );
    connect( m_pRoboNetThread,
             SIGNAL(signal_net(const QString &,int,RoboMsg)),
             pView,
             SLOT(slot_net_event(const QString &,int,RoboMsg)));logDbg();

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

    //! devices
    if ( robot_is_mrq( pBase->getId() ) )
    {
        mrqProperty *pProp;
        pProp = new mrqProperty( pBase->getAxes() );logDbg();
        Q_ASSERT( NULL != pProp );
        createModelView( pProp, pBase );        //! device is a model

        return pProp;
    }
    //! robot
    else if ( robot_is_robot( pBase->getId() ) )
    {
        roboProp *pProp;
        pProp = new roboProp();
        Q_ASSERT( NULL != pProp );

        //! set instmgr
        ((VRobot*)pObj)->setInstMgr( mMcModel.m_pInstMgr );

        createModelView( pProp, pObj );logDbg();

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
//        m_pSpyCfgModel->fillDbgData();

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
//    for( int i = 0; i < ui->widget->count(); i++ )
//    {
////        if ( ui->widget->tabText(i) == str )
////        {
////            ui->widget->setCurrentWidget( ui->widget->widget(i) );
////        }
//        ui->widget->widget(i)->close();
//    }
    ui->widget->clear();
    mModelViews.clear();
}
//! active window
void MainWindow::slot_wndActive( QString str )
{
    for( int i = 0; i < ui->widget->count(); i++ )
    {
        if ( ui->widget->tabText(i) == str )
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

//void MainWindow::on_actionSave_Scene_triggered()
//{
//    //! get current scenne
//    roboScene *pRoboScene = currentRoboScene();
//    if ( NULL == pRoboScene )
//    { return; }

//    //! have path
//    if ( pRoboScene->getModelObj()->getPath().length() > 0 )
//    {
//        pRoboScene->save();
//    }
//    else
//    { on_actionSave_Scene_As_triggered(); }
//}

//void MainWindow::on_actionSave_Scene_As_triggered()
//{
//    //! get current scenne
//    roboScene *pRoboScene = currentRoboScene();
//    if ( NULL == pRoboScene )
//    { return; }

//    //! dialog
//    QFileDialog fDlg;

//    fDlg.setAcceptMode( QFileDialog::AcceptSave );
//    fDlg.setNameFilter( tr("scene file (*.sce)") );
//    if ( QDialog::Accepted != fDlg.exec() )
//    { return; }

//    pRoboScene->getModelObj()->setPath( fDlg.directory().absolutePath() );
//    pRoboScene->getModelObj()->setName( fDlg.selectedFiles().first() );
//    pRoboScene->save( fDlg.selectedFiles().first() );

//    //! change the tab title
//    int curIndex = ui->widget->currentIndex();
//    ui->widget->setTabText( curIndex,
//                            comAssist::pureFileName( fDlg.selectedFiles().first() )
//                            );
//}

//void MainWindow::on_actionOpen_Scene_triggered()
//{
//    QFileDialog fDlg;

//    fDlg.setAcceptMode( QFileDialog::AcceptOpen );
//    fDlg.setNameFilter( tr("scene file (*.sce)") );
//    if ( QDialog::Accepted != fDlg.exec() )
//    { return; }

//    m_pScriptMgr->openScene( fDlg.directory().absolutePath(),
//                             fDlg.selectedFiles().first() );
//}


void MainWindow::slot_scriptmgr_changed()
{
//    Q_ASSERT( NULL != m_pScriptMgr );

//    QStringList sceneList, pathList;

//    sceneList = m_pScriptMgr->sceneList( pathList );
//    logDbg()<<sceneList<<pathList;

//    m_pRoboConnTool->setScenes( sceneList, pathList );

    slot_scene_changed();
}

void MainWindow::on_actionForceStop_triggered()
{
    if ( NULL != m_pDeviceMgr->m_pMgr )
    { m_pDeviceMgr->m_pMgr->emergencyStop(); }
}

void MainWindow::on_actionReset_triggered()
{
    if ( NULL != m_pDeviceMgr->m_pMgr )
    { m_pDeviceMgr->m_pMgr->hardReset(); }
}

void MainWindow::on_actionAngle_A_triggered()
{
    if ( NULL == m_pAngleMonitor )
    {
        m_pAngleMonitor = new AngleMonitor(this);
    }

    //! fail to open window
    if ( NULL == m_pAngleMonitor )
    { return; }

    m_pAngleMonitor->show();
    //! set connection

    QStringList strList = mMcModel.m_pInstMgr->getChans();

    m_pAngleMonitor->setModel( &mMcModel );
    m_pAngleMonitor->setConnections( strList );

}
