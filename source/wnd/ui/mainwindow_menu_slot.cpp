#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "../../com/comassist.h"
#include "motionwizard.h"

#include "dlgapp.h"
#include "deviceconsole.h"

#include "../model/mrggraphmodel.h"

#include "robograph.h"

void MainWindow::doLoadPrj( const QString &path,
                             const QString &name )
{
    m_pScriptMgr->load( path, name );

    //! close all
    slot_wndcloseAll();
    if ( mMcModel.mSysPref.mAutoExpand )
    {
        m_pScriptMgr->setExpand( true );
    }

    setWindowTitle( m_pScriptMgr->getFullName() );
}

bool MainWindow::asurePrjValid()
{
    //! not saved
    if ( m_pScriptMgr->getPath().length() < 1 )
    {
        QString path, name;

        if ( savePrjAs( path, name ) )
        {}
        else
        { return false; }

        //! load again
        doLoadPrj( path, name );

        //! save latest prj
        mMcModel.mSysPref.setLatestPrj( path,
                                        comAssist::pureFileName( name ) );

        //! request save
        emit sig_pref_request_save();

        return true;
    }
    else
    { return true; }
}

bool MainWindow::savePrjAs( QString &path, QString &name )
{
    QFileDialog fDlg;

    fDlg.setAcceptMode( QFileDialog::AcceptSave );
    fDlg.setNameFilter( tr("prj file (*.prj)") );
    if ( QDialog::Accepted != fDlg.exec() )
    { return false; }

    m_pScriptMgr->save( fDlg.directory().absolutePath(),
                        fDlg.selectedFiles().first() );

    setWindowTitle( m_pScriptMgr->getFullName() );

    //! out name
    path = fDlg.directory().absolutePath();
    name = fDlg.selectedFiles().first();

    return true;
}

void MainWindow::newMrp( QString className )
{
    //! file name
    QFileDialog fDlg;
    fDlg.setAcceptMode( QFileDialog::AcceptSave );
    fDlg.setNameFilter( tr("mrp file (*.mrp)") );
    if ( fDlg.exec() != QFileDialog::Accepted )
    { return; }

    //! create the type
    MegaTableModel *pNewModelObj;
    pNewModelObj = scriptMgr::newMotion( className );
    if ( NULL == pNewModelObj )
    {
        sysError( tr("fail"), className );
        return;
    }

    //! auto add
    if ( mMcModel.mSysPref.mbNewAutoAdd )
    { m_pScriptMgr->slot_file_import( fDlg.selectedFiles().first() ); }

    pNewModelObj->save( fDlg.selectedFiles().first() );

    //! init obj
    pNewModelObj->setName( comAssist::pureFileName( fDlg.selectedFiles().first() ) );
    pNewModelObj->setPath( fDlg.directory().absolutePath() );
    pNewModelObj->setFullName( fDlg.selectedFiles().first() );

    pNewModelObj->setShadow( true );
    pNewModelObj->setGc( true );

    pNewModelObj->set( mcModelObj::model_motion_file, pNewModelObj );

    on_itemXActivated( pNewModelObj );
}

void MainWindow::on_itemx_active( mcModelObj* pObj )
{
    if ( NULL == pObj )
    { return; }

    //! find view
    modelView *pView = findView( pObj );
    int index;
    if ( NULL != pView )
    {
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

    //! close precedent
    pView = findView( pObj->fullName() );
    if ( pView != NULL )
    {logDbg()<<pObj->fullName();
        index = ui->widget->indexOf( pView );
        if ( index >= 0 )
        { ui->widget->removeTab( index ); }
        else
        { Q_ASSERT(false); }

        pView->close();

        mModelViews.removeAll( pView );
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
    {
        roboScene *pWorkScene;
        pWorkScene = new roboScene();
        Q_ASSERT( NULL != pWorkScene );

        createModelView( pWorkScene, pObj );

        connect( pWorkScene,
                 SIGNAL(itemXActivated(mcModelObj*, mcModelObj_Op)),
                 this,
                 SLOT(on_itemXActivated(mcModelObj*, mcModelObj_Op)));

        connect( pWorkScene,
                 SIGNAL(signalSceneChanged()),
                 this,
                 SLOT(slot_scene_changed()) );
    }

    //! graph file
    else if ( pObj->getType() == mcModelObj::model_graph_file )
    {
        RoboGraph *pRoboGraph;
        pRoboGraph = new RoboGraph();
        Q_ASSERT( NULL != pRoboGraph );

        //! set the data
        createModelView( pRoboGraph, pObj );

        //! load graph
        ((MrgGraphModel*)pObj)->load( pObj->fullName(), pRoboGraph );

        //! \todo
//        connect( pRoboGraph,
//                 SIGNAL(itemXActivated(mcModelObj*, mcModelObj_Op)),
//                 this,
//                 SLOT(on_itemXActivated(mcModelObj*, mcModelObj_Op)));

//        connect( pRoboGraph,
//                 SIGNAL(signalSceneChanged()),
//                 this,
//                 SLOT(slot_scene_changed()) );
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

void MainWindow::on_itemx_new_mrp( mcModelObj* pObj )
{
    if ( NULL == pObj )
    { return; }

    //! new mrp
    VRobot *pRobo = (VRobot*)pObj;
    if ( NULL == pRobo )
    { return; }

    newMrp( pRobo->getClass() );
}

void MainWindow::changeLang( int langIndex )
{
    if ( langIndex == 0 )
    {
        ui->actionEnglish->setChecked( true );
        ui->actionChinese->setChecked( false );
        ui->actionTraditional_Chinese->setChecked( false );
    }
    else if ( langIndex == 1 )
    {
        ui->actionEnglish->setChecked( false );
        ui->actionChinese->setChecked( true );
        ui->actionTraditional_Chinese->setChecked( false );
    }
    else if ( langIndex == 2 )
    {
        ui->actionEnglish->setChecked( false );
        ui->actionChinese->setChecked( false );
        ui->actionTraditional_Chinese->setChecked( true );
    }
    else
    { return; }

    //! set lang
    do
    {
        QLocale::Language lang;
        QLocale::Country area;
        if ( langIndex == 0 )
        {
            lang = QLocale::English;
            area = QLocale::AnyCountry;
        }
        else if ( langIndex == 1 )
        {
            lang = QLocale::Chinese;
            area = QLocale::China;
        }
        else if ( langIndex == 2 )
        {
            lang = QLocale::Chinese;
            area = QLocale::Taiwan;
        }
        else
        {
            lang = QLocale::AnyLanguage;
            area = QLocale::AnyCountry;

            return;
        }

        if ( mTranslator.isEmpty() )
        {}
        else
        { qApp->removeTranslator( &mTranslator ); }

        //! local
        QLocale local( lang, area );
        local.setDefault( QLocale(QLocale::English) );

        logDbg()<<local.uiLanguages();

        if ( mTranslator.load( local,
                              QLatin1String("megarobostudio"),
                              QLatin1String("_"),
                              qApp->applicationDirPath() + "/translate"
                              )
             && qApp->installTranslator(&mTranslator) )
        {
        }
        else
        {
            QMessageBox::information( NULL,
                                    QObject::tr("Info"),
                                    QObject::tr("language loss"));
        }
    }while( 0 );

}

//! new prj
void MainWindow::on_actionProject_triggered()
{
    QFileDialog fDlg;

    fDlg.setAcceptMode( QFileDialog::AcceptSave );
    fDlg.setNameFilter( tr("prj file (*.prj)") );
    if ( QDialog::Accepted != fDlg.exec() )
    { return; }

    m_pScriptMgr->reNew(
                         fDlg.directory().absolutePath(),
                         fDlg.selectedFiles().first() );

    m_pScriptMgr->save( fDlg.directory().absolutePath(),
                        fDlg.selectedFiles().first() );

    //! save latest prj
    mMcModel.mSysPref.setLatestPrj( fDlg.directory().absolutePath(),
                                    comAssist::pureFileName( fDlg.selectedFiles().first() ) );

    //! request save
    emit sig_pref_request_save();
}
void MainWindow::on_actionOpen_Prj_triggered()
{
    QFileDialog fDlg;

    fDlg.setAcceptMode( QFileDialog::AcceptOpen );
    fDlg.setNameFilter( tr("prj file (*.prj)") );
    if ( QDialog::Accepted != fDlg.exec() )
    { return; }

    //! do load
    doLoadPrj( fDlg.directory().absolutePath(),
               fDlg.selectedFiles().first() );

    //! save latest prj
    mMcModel.mSysPref.setLatestPrj( fDlg.directory().absolutePath(),
                                    comAssist::pureFileName( fDlg.selectedFiles().first() ) );

    //! request save
    emit sig_pref_request_save();
}

void MainWindow::on_actionSave_Prj_triggered()
{
    //! not saved
    if ( m_pScriptMgr->getPath().length() < 1 )
    {
        return on_actionSave_Prj_As_triggered();
    }

    QString strFileName;

    //! save file name
    strFileName = m_pScriptMgr->getPath() + QDir::separator() + m_pScriptMgr->getName();
    strFileName = QDir::toNativeSeparators( strFileName );
    if ( strFileName.contains(".prj") )
    {}
    else
    { strFileName += ".prj"; }

    //! have saved
    m_pScriptMgr->save( m_pScriptMgr->getPath(),
                        strFileName );
}

void MainWindow::on_actionSave_Prj_As_triggered()
{
    QString path, name;

    savePrjAs( path, name );
}

void MainWindow::on_actionClose_Prj_triggered()
{
    slot_wndcloseAll();

    m_pScriptMgr->reNew( "", "" );

    //! save latest prj
    mMcModel.mSysPref.setLatestPrj( "",
                                    "" );

    setWindowTitle( "" );

    //! request save
    emit sig_pref_request_save();
}

//! save the current file
void MainWindow::on_actionSave_triggered()
{
    if ( ui->widget->currentWidget() != NULL )
    {
        modelView *pView = (modelView*)ui->widget->currentWidget();
        Q_ASSERT( NULL != pView );

        QString str;
        logDbg();
        int ret = pView->save(str);logDbg()<<ret;
        on_signalReport( ret, str );
        if ( ret == 0 )
        { sysLog( str, tr("save complted") ); }
        else
        { sysError( str, tr("save failed") ); }
    }
}

void MainWindow::on_actionSave_As_triggered()
{
    if ( ui->widget->currentWidget() != NULL )
    {
        modelView *pView = (modelView*)ui->widget->currentWidget();
        Q_ASSERT( NULL != pView );

        QString nameFilter;

        QStringList strList = pView->filePattern();
        if ( strList.size() >=2 )
        {
            nameFilter = QString("%1 (*.%2)").arg( strList[0] ).arg( strList[1] );
        }
        else
        {
            nameFilter = QString("Any file (*.*)");
        }

        QFileDialog fDlg;
        fDlg.setAcceptMode( QFileDialog::AcceptSave );
        fDlg.setNameFilter( nameFilter );
        if ( QDialog::Accepted != fDlg.exec() )
        { return; }

        QString str;
        str = fDlg.selectedFiles().first();
        int ret = pView->saveAs( str );

        on_signalReport( ret, str );
    }
}

//! save all views
void MainWindow::on_actionSave_All_A_triggered()
{
    modelView *pView;
    for ( int i = 0; i < ui->widget->count(); i++ )
    {
        pView = (modelView*)ui->widget->widget( i );
        Q_ASSERT( NULL != pView );

        QString str;
        int ret;
        ret = pView->save( str );

        sysLog( QString("%1:%2").arg(str).arg(ret) );
    }
}

void MainWindow::on_actionOpen_triggered()
{
    QFileDialog fDlg;

    fDlg.setAcceptMode( QFileDialog::AcceptOpen );
    QStringList nameFilters;

    nameFilters<<tr("mrp file (*.mrp)")
               <<tr("pvt file (*.pvt)")
               <<tr("pt file (*.pt)")
               <<tr("scene (*.sce)")
               <<tr("python (*.py)")
               <<tr("mrg (*.mrg)")
               ;
    fDlg.setNameFilters( nameFilters );
    if ( QDialog::Accepted != fDlg.exec() )
    { return; }

    m_pScriptMgr->openFile( fDlg.directory().path(),
                            fDlg.selectedFiles().first() );
}

//! news
void MainWindow::on_actionNewMotion_triggered()
{
    //! type
    MotionWizard motionWizard;
    if ( QDialog::Accepted == motionWizard.exec() )
    {}
    else
    { return; }

    //! check prj
    if ( asurePrjValid() )
    {}
    else
    { return; }

    newMrp( motionWizard.motionName() );
}

void MainWindow::on_actionNewPVT_triggered()
{
    //! check prj
    if ( asurePrjValid() )
    {}
    else
    { return; }

    QFileDialog fDlg;
    fDlg.setAcceptMode( QFileDialog::AcceptSave );
    fDlg.setNameFilter( tr("pvt file (*.pvt)") );
    if ( fDlg.exec() != QDialog::Accepted )
    { return; }

    tpvGroup * pNewModelObj = new tpvGroup();
    Q_ASSERT( NULL != pNewModelObj );

    //! auto add
    if ( mMcModel.mSysPref.mbNewAutoAdd )
    { m_pScriptMgr->slot_file_import( fDlg.selectedFiles().first() ); }

    pNewModelObj->save( fDlg.selectedFiles().first() );

    pNewModelObj->setName( comAssist::pureFileName( fDlg.selectedFiles().first() ) );
    pNewModelObj->setPath( fDlg.directory().absolutePath() );
    pNewModelObj->setFullName( fDlg.selectedFiles().first() );

    pNewModelObj->setGc( true );
    pNewModelObj->setShadow( true );

    pNewModelObj->set( mcModelObj::model_tpv, pNewModelObj );

    on_itemXActivated( pNewModelObj );
}

void MainWindow::on_actionPT_triggered()
{
    //! check prj
    if ( asurePrjValid() )
    {}
    else
    { return; }

    QFileDialog fDlg;
    fDlg.setAcceptMode( QFileDialog::AcceptSave );
    fDlg.setNameFilter( tr("pt file (*.pt)") );
    if ( fDlg.exec() != QDialog::Accepted )
    { return; }

    TpGroup * pNewModelObj = new TpGroup();
    Q_ASSERT( NULL != pNewModelObj );

    //! auto add
    if ( mMcModel.mSysPref.mbNewAutoAdd )
    { m_pScriptMgr->slot_file_import( fDlg.selectedFiles().first() ); }

    pNewModelObj->save( fDlg.selectedFiles().first() );

    pNewModelObj->setName( comAssist::pureFileName( fDlg.selectedFiles().first() ) );
    pNewModelObj->setPath( fDlg.directory().absolutePath() );
    pNewModelObj->setFullName( fDlg.selectedFiles().first() );

    pNewModelObj->setGc( true );
    pNewModelObj->setShadow( true );

    pNewModelObj->set( mcModelObj::model_tp, pNewModelObj );

    on_itemXActivated( pNewModelObj );
}

void MainWindow::on_actionScene_triggered()
{
    //! check prj
    if ( asurePrjValid() )
    {}
    else
    { return; }

    QFileDialog fDlg;

    fDlg.setAcceptMode( QFileDialog::AcceptSave );
    fDlg.setNameFilter( tr("scene file (*.sce)") );
    if ( QDialog::Accepted != fDlg.exec() )
    { return; }

    roboSceneModel *pScene = new roboSceneModel();
    mcModelObj *pNewModelObj = pScene;
    Q_ASSERT( NULL != pNewModelObj );

    //! auto add
    if ( mMcModel.mSysPref.mbNewAutoAdd )
    { m_pScriptMgr->slot_file_import( fDlg.selectedFiles().first() ); }

    //! save
    pScene->save( fDlg.selectedFiles().first() );

    pNewModelObj->setPath( fDlg.directory().absolutePath() );
    pNewModelObj->setName( comAssist::pureFileName( fDlg.selectedFiles().first() ) );
    pNewModelObj->setFullName( fDlg.selectedFiles().first() );

    pNewModelObj->setGc( true );
    pNewModelObj->setShadow( true );
    pNewModelObj->set( mcModelObj::model_scene_file, pNewModelObj );

    Q_ASSERT( NULL != pNewModelObj );

    emit itemXActivated( pNewModelObj );
}

void MainWindow::on_actionGraph_triggered()
{
    //! check prj
    if ( asurePrjValid() )
    {}
    else
    { return; }

    QFileDialog fDlg;

    fDlg.setAcceptMode( QFileDialog::AcceptSave );
    fDlg.setNameFilter( tr("graph file (*.mrg)") );
    if ( QDialog::Accepted != fDlg.exec() )
    { return; }

    MrgGraphModel *pGraph = new MrgGraphModel();
    mcModelObj *pNewModelObj = pGraph;
    Q_ASSERT( NULL != pNewModelObj );

    //! auto add
    if ( mMcModel.mSysPref.mbNewAutoAdd )
    { m_pScriptMgr->slot_file_import( fDlg.selectedFiles().first() ); }

    //! save
    pGraph->save( fDlg.selectedFiles().first() );

    pNewModelObj->setPath( fDlg.directory().absolutePath() );
    pNewModelObj->setName( comAssist::pureFileName( fDlg.selectedFiles().first() ) );
    pNewModelObj->setFullName( fDlg.selectedFiles().first() );

    pNewModelObj->setGc( true );
    pNewModelObj->setShadow( true );
    pNewModelObj->set( mcModelObj::model_graph_file, pNewModelObj );

    Q_ASSERT( NULL != pNewModelObj );

    emit itemXActivated( pNewModelObj );
}

//! about
void MainWindow::on_actionAbout_triggered()
{
    //! pref
    aboutDlg dlg( &mMcModel.mSysPref );

    dlg.exec();
}

void MainWindow::on_actionDocs_triggered()
{
    QStringList args;
    QString str;
    str = QCoreApplication::applicationDirPath() + QDir::separator() + QStringLiteral("doc");
    str.replace("/", QDir::separator() );
    args<<str;
    //! \todo linux

    QProcess::execute( "explorer.exe", args );
}

void MainWindow::on_actionPackage_triggered()
{
    QStringList args;
    QString str;
    str = QCoreApplication::applicationDirPath()
            + QDir::separator() + QStringLiteral("package")
            + QDir::separator() + QStringLiteral("mrq");
    str.replace("/", QDir::separator() );
    args<<str;
    //! \todo linux

    QProcess::execute( "explorer.exe", args );
}

void MainWindow::on_actionExample_triggered()
{
    QStringList args;
    QString str;
    str = QCoreApplication::applicationDirPath()
            + QDir::separator() + QStringLiteral("package")
            + QDir::separator() + QStringLiteral("example");
    str.replace("/", QDir::separator() );
    args<<str;
    //! \todo linux

    QProcess::execute( "explorer.exe", args );
}

void MainWindow::on_actionDataSet_triggered()
{
    QStringList args;
    QString str;
    str = QCoreApplication::applicationDirPath()
            + QDir::separator() + QStringLiteral("package")
            + QDir::separator() + QStringLiteral("dataset");
    str.replace("/", QDir::separator() );
    args<<str;
    //! \todo linux

    QProcess::execute( "explorer.exe", args );
}

void MainWindow::on_actionReadMe_triggered()
{
    QStringList args;
    QString str;
    str = QCoreApplication::applicationDirPath()
            + QDir::separator()
            + QStringLiteral("doc")
            + QDir::separator()
            + QStringLiteral("readme.txt");
    str.replace("/","\\");
    args<<str;
    //! \todo linux

    QProcess::execute( "explorer.exe", args );
}


void MainWindow::on_actionErrant_triggered()
{
    QStringList args;
    QString str;
    str = QCoreApplication::applicationDirPath()
            + QDir::separator()
            + QStringLiteral("doc")
            + QDir::separator()
            + QStringLiteral("errant.txt");
    str.replace("/","\\");
    args<<str;
    //! \todo linux

    QProcess::execute( "explorer.exe", args );
}

//! pref
void MainWindow::on_actionpref_triggered( )
{
    sysPref dlg;

    dlg.setPref( &mMcModel.mSysPref );

    if ( QDialog::Accepted == dlg.exec() )
    {
        mMcModel.mSysPref.save( user_pref_file_name );

        applyConfigs();

        mMcModel.postload();

        slot_pref_changed();
    }
}

void MainWindow::on_actionRun_Script_triggered()
{
    if ( NULL == m_pProcess )
    {
        m_pProcess = new QProcess();
        connect( m_pProcess, SIGNAL(readyReadStandardOutput()),
                 this, SLOT(slot_process_output()) );

        connect( m_pProcess, SIGNAL(readyRead()),
                 this, SLOT(slot_process_output()) );


        connect( m_pProcess, SIGNAL(readyReadStandardError()),
                 this, SLOT(slot_process_output()) );

        connect( m_pProcess, SIGNAL(finished(int,QProcess::ExitStatus)),
                 this, SLOT(slot_process_exit(int,QProcess::ExitStatus)) );
    }

    Q_ASSERT( NULL != m_pProcess );
    //! run the script
    QStringList args;
    args<<mCurrentScript;
    m_pProcess->start( "python", args );

    //! run
    if ( m_pProcess->waitForStarted() )
    {
        sysLog( mCurrentScript, tr("started") );
        ui->actionRun_Script->setEnabled( false );
        ui->actionTerminate->setEnabled( true );
    }
    else
    {}
}

void MainWindow::on_actionTerminate_triggered()
{
    if ( NULL != m_pProcess )
    {
        m_pProcess->kill();
    }
}

void MainWindow::on_actionConsole_triggered()
{
    deviceConsole *pConsole;

    pConsole = new deviceConsole(this);
    if ( NULL == pConsole )
    { return; }

    connect( m_pDeviceMgr, SIGNAL(signal_instmgr_changed(bool,MegaDevice::InstMgr*)),
             pConsole, SLOT(slot_device_changed()));

    pConsole->setMrq( true );
    pConsole->setClassName( "local" );

    //! title name
    pConsole->setWindowTitle( "localhost" );
    pConsole->setShell( this );

    pConsole->show();
}

void MainWindow::on_actionApp_triggered()
{
    DlgApp appWnd( &mMcModel.mSysPref, &mAppMgr, this );

    if ( QDialog::Accepted == appWnd.exec() )
    {
        mMcModel.mSysPref.mAppStartDelay = appWnd.getDelay();
        emit sig_pref_request_save();
    }
}
