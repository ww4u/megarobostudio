#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "../../com/comassist.h"
#include "motionwizard.h"

//! file
//void MainWindow::on_actionNew_triggered()
//{
//    QFileDialog fDlg;

//    fDlg.setAcceptMode( QFileDialog::AcceptSave );
//    fDlg.setNameFilter( tr("motion file (*.xml)") );
//    if ( QDialog::Accepted != fDlg.exec() )
//    { return; }

//    m_pScriptMgr->newFile( fDlg.directory().absolutePath(), fDlg.selectedFiles().first() );
//}

void  MainWindow::doLoadPrj( const QString &path,
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

//! project
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
    mMcModel.mSysPref.save( pref_file_name );
}

void MainWindow::on_actionSave_Prj_triggered()
{
    //! not saved
    if ( m_pScriptMgr->getPath().length() < 1 )
    {
        return on_actionSave_Prj_As_triggered();
    }

    QString strFileName;

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
    QFileDialog fDlg;

    fDlg.setAcceptMode( QFileDialog::AcceptSave );
    fDlg.setNameFilter( tr("prj file (*.prj)") );
    if ( QDialog::Accepted != fDlg.exec() )
    { return; }

    m_pScriptMgr->save( fDlg.directory().absolutePath(),
                        fDlg.selectedFiles().first() );

    setWindowTitle( m_pScriptMgr->getFullName() );
}

//! save the current file
void MainWindow::on_actionSave_triggered()
{
    if ( ui->widget->currentWidget() != NULL )
    {
        modelView *pView = (modelView*)ui->widget->currentWidget();
        Q_ASSERT( NULL != pView );

        QString str;
        m_pStateBar->showState( tr("Saving...") );
        int ret = pView->save(str);
        on_signalReport( ret, str );
        if ( ret == 0 )
        { sysLog( str, "save complted"); }
        else
        { sysError( str, "save failed"); }
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
    fDlg.setNameFilter( tr("motion file (*.mc);"
                           ";pvt file (*.pvt);"
                           ";pt file (*.pt);"
                           ";setup file (*.stp);"
                           ";scene file (*.sce)") );
    if ( QDialog::Accepted != fDlg.exec() )
    { return; }

    m_pScriptMgr->openFile( fDlg.directory().path(),
                            fDlg.selectedFiles().first() );
}

//! news
void MainWindow::on_actionNewMotion_triggered()
{
    MotionWizard motionWizard;

    if ( QDialog::Accepted == motionWizard.exec() )
    {}
    else
    { return; }

    QFileDialog fDlg;
    fDlg.setAcceptMode( QFileDialog::AcceptSave );
    fDlg.setNameFilter( tr("motion file (*.mc)") );
    if ( fDlg.exec() != QFileDialog::Accepted )
    { return; }

    //! create the type
    MegaTableModel *pNewModelObj;
    pNewModelObj = scriptMgr::newMotion( motionWizard.motionName() );
    if ( NULL == pNewModelObj )
    {
        sysError( tr("fail"), motionWizard.motionName() );
        return;
    }

    //! init obj
    pNewModelObj->setName( comAssist::pureFileName( fDlg.selectedFiles().first() ) );
    pNewModelObj->setPath( fDlg.directory().absolutePath() );

    pNewModelObj->setFile( true );
    pNewModelObj->setGc( true );

    pNewModelObj->set( mcModelObj::model_motion_file, pNewModelObj );

    on_itemXActivated( pNewModelObj );
}

void MainWindow::on_actionNewPVT_triggered()
{
    QFileDialog fDlg;
    fDlg.setAcceptMode( QFileDialog::AcceptSave );
    fDlg.setNameFilter( tr("pvt file (*.pvt)") );
    if ( fDlg.exec() != QDialog::Accepted )
    { return; }

    tpvGroup * pNewModelObj = new tpvGroup();
    Q_ASSERT( NULL != pNewModelObj );

    pNewModelObj->setName( comAssist::pureFileName( fDlg.selectedFiles().first() ) );
    pNewModelObj->setPath( fDlg.directory().absolutePath() );

    pNewModelObj->setGc( true );
    pNewModelObj->setFile( true );

    pNewModelObj->set( mcModelObj::model_tpv, pNewModelObj );

    on_itemXActivated( pNewModelObj );
}

void MainWindow::on_actionPT_triggered()
{
    QFileDialog fDlg;
    fDlg.setAcceptMode( QFileDialog::AcceptSave );
    fDlg.setNameFilter( tr("pt file (*.pt)") );
    if ( fDlg.exec() != QDialog::Accepted )
    { return; }

    TpGroup * pNewModelObj = new TpGroup();
    Q_ASSERT( NULL != pNewModelObj );

    pNewModelObj->setName( comAssist::pureFileName( fDlg.selectedFiles().first() ) );
    pNewModelObj->setPath( fDlg.directory().absolutePath() );

    pNewModelObj->setGc( true );
    pNewModelObj->setFile( true );

    pNewModelObj->set( mcModelObj::model_tp, pNewModelObj );

    on_itemXActivated( pNewModelObj );
}

void MainWindow::on_actionScene_triggered()
{
    QFileDialog fDlg;

    fDlg.setAcceptMode( QFileDialog::AcceptSave );
    fDlg.setNameFilter( tr("scene file (*.sce)") );
    if ( QDialog::Accepted != fDlg.exec() )
    { return; }

    mcModelObj *pNewModelObj = new roboSceneModel();
    Q_ASSERT( NULL != pNewModelObj );

    pNewModelObj->setPath( fDlg.directory().absolutePath() );
    pNewModelObj->setName( comAssist::pureFileName( fDlg.selectedFiles().first() ) );

    pNewModelObj->setGc( true );
    pNewModelObj->setFile( true );
    pNewModelObj->set( mcModelObj::model_scene_file, pNewModelObj );

    Q_ASSERT( NULL != pNewModelObj );

    emit itemXActivated( pNewModelObj );
}

//! about
void MainWindow::on_actionAbout_triggered()
{
    aboutDlg dlg;

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
    logDbg()<<args;
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
    logDbg()<<args;
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
    logDbg()<<args;
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
    logDbg()<<args;
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
    logDbg()<<str;
    QProcess::execute( "explorer.exe", args );
}

//! pref
void MainWindow::on_actionpref_triggered( )
{
    sysPref dlg;

    dlg.setPref( &mMcModel.mSysPref );

    if ( QDialog::Accepted == dlg.exec() )
    {
        mMcModel.mSysPref.save( pref_file_name );

        applyConfigs();

        mMcModel.postload();

        slot_pref_changed();
    }
}



