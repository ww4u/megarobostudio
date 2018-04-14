#include "scriptmgr.h"
#include "ui_scriptmgr.h"
#include "../../com/comassist.h"

#include "../../model/motiongroup.h"
#include "../../model/tpvgroup.h"
#include "../../model/roboscenemodel.h"

#include "../../robot/robotfact.h"

scriptMgr::scriptMgr(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::scriptMgr)
{
    m_pContextMenu = NULL;
    mUntitleId = 0;

    m_pRootModel = NULL;
    m_pRootModel = new scriptModel();

    //! file system
    m_pFileSysModel = new QFileSystemModel(this);
    m_pFileSysModel->setRootPath( QDir::currentPath() );

    logDbg()<<QDir::currentPath();

    ui->setupUi(this);

    setupUi();

    buildConnection();
}

scriptMgr::~scriptMgr()
{
    delete ui;

    delete m_pRootModel;
}

void scriptMgr::contextMenuEvent(QContextMenuEvent *event)
{
    //! script mgr
    if ( ui->tabWidget->currentWidget()->objectName() == "scriptView" )
    {
        m_pContextMenu->popup( mapToGlobal( event->pos() ) );

        QModelIndex index = ui->scriptView->indexAt( event->pos() );

        //! action_delete
        m_pRemoveAction->setEnabled( index.isValid() );
        m_pImportAction->setEnabled( getPath().length() > 1 );

        m_pNewGroupAction->setEnabled( getPath().length() > 1 );
    }
    //! file mgr
    else
    {}
}

void scriptMgr::setExpand( bool b )
{
    if ( b )
    {
        ui->scriptView->expandAll();
    }
    else
    {
        ui->scriptView->collapseAll();
    }
}

void scriptMgr::setName( const QString &name )
{
    m_pRootModel->setName( name );
}
QString scriptMgr::getName()
{
    return m_pRootModel->getName();
}

QString scriptMgr::getPath()
{
    return m_pRootModel->getPath();
}

QString scriptMgr::getFullName()
{
    return QDir::toNativeSeparators( m_pRootModel->getPath() + QDir::separator() + m_pRootModel->getName() );
}

int scriptMgr::save( const QString &path, const QString &name )
{
    m_pRootModel->setPath( path );
    m_pRootModel->setName( comAssist::pureFileName(name) );

    return m_pRootModel->save( name );
}
int scriptMgr::load( const QString &path, const QString &name )
{
    m_pRootModel->setPath( path );
    m_pRootModel->setName( comAssist::pureFileName(name) );

    int ret;
    ret = m_pRootModel->load( name );

    emit signal_scriptmgr_changed();

    return ret;
}
void scriptMgr::reNew( const QString &path,
                       const QString &prjName )
{
    m_pRootModel->reNew();

    //! project path
    m_pRootModel->setPath( path );
    m_pRootModel->setName( comAssist::pureFileName(prjName) );

    emit signal_scriptmgr_changed();
}

int scriptMgr::saveActiveFile( const QString &file )
{
    QModelIndex index = ui->scriptView->currentIndex();

    if ( !index.isValid() )
    { return ERR_INVALID_SCRIPT_FILE_NODE; }

    scriptNode *pNode = (scriptNode*)( index.internalPointer() );
    if ( NULL == pNode || pNode->getNodeType() != scriptNode::node_file )
    { return ERR_INVALID_SCRIPT_FILE_NODE; }

    motionGroup *pGroup = dynamic_cast<motionGroup*>( pNode );
    if ( NULL == pGroup )
    { return ERR_INVALID_SCRIPT_FILE_NODE; }

    return pGroup->save( file );
}

int scriptMgr::openFile( const QString &path,
                         const QString &file )
{
    //! find the type
    QString ext = comAssist::fileSuffix( file );
    if ( ext.length() < 1 )
    { return ERR_FILE_DO_NOT_SUPPORT; }

    if ( comAssist::fileSuffixMatch( ext, "mc") )
    {
        return openMotionGroup( path, file );
    }
    else if ( comAssist::fileSuffixMatch( ext, "pvt") )
    {
        return openTpv( path, file );
    }
    else if ( comAssist::fileSuffixMatch( ext, "stp") )
    {
        return openSetup( path, file );
    }
    else if ( comAssist::fileSuffixMatch( ext, "sce") )
    {
        return openScene( path, file );
    }
    else if ( comAssist::fileSuffixMatch( ext, "py") )
    {
        return openPy( path, file );
    }
    else
    {  }

    return ERR_FILE_DO_NOT_SUPPORT;
}

int scriptMgr::openMotionGroup( const QString &path, const QString &file )
{
    motionGroup *pGroup;

    pGroup = new motionGroup();
    Q_ASSERT( NULL != pGroup );
    int ret = pGroup->load( file );
    if ( ret != 0 )
    {
        delete pGroup;
        return ret;
    }

    logDbg()<<pGroup->mItems.size();

    pGroup->setPath( path );
    pGroup->setName( comAssist::pureFileName(file) );
    pGroup->setFile( true );
    pGroup->setGc( true );

    pGroup->set( mcModelObj::model_motion_file,
                 pGroup );

    emit itemXActivated( pGroup );

    return ERR_NONE;
}

int scriptMgr::openTpv( const QString &path, const QString &file )
{
    tpvGroup *pGroup;

    pGroup = new tpvGroup();
    Q_ASSERT( NULL != pGroup );
    pGroup->setGc( true );

    int ret = pGroup->load( file, tpvGroup::pvt_tpv );
    if ( ret != 0 )
    {
        delete pGroup;
        return ret;
    }

    logDbg()<<pGroup->mItems.size();

    pGroup->setPath( path );
    pGroup->setName( comAssist::pureFileName(file) );
    pGroup->setFile( true );
    pGroup->setGc( true );

    pGroup->set( mcModelObj::model_tpv,
                 pGroup );

    emit itemXActivated( pGroup );

    return ERR_NONE;
}

int scriptMgr::openScene( const QString &path, const QString &file )
{
    roboSceneModel *pModel;

    //! model deleted in the widget
    pModel = createScene( path, file );
    if ( NULL == pModel )
    { return ERR_FILE_READ_FAIL; }

    emit itemXActivated( pModel );

    return ERR_NONE;
}

int scriptMgr::openSetup( const QString &path, const QString &file )
{
    //! 1.load the class
    VRobot *pBase;

    int ret;
    pBase = new VRobot();
    Q_ASSERT( NULL != pBase );
    ret = pBase->load( file );
    if ( ret != 0 )
    {
        delete pBase;
        return ERR_FILE_READ_FAIL;
    }

    //! 2.load again
    VRobot *pRealRobo = robotFact::createRobot( pBase->getClass() );
    Q_ASSERT( NULL != pRealRobo );
    delete pBase;

    ret = pRealRobo->load( file );
    if ( ret != 0 )
    { return ERR_FILE_READ_FAIL; }

    pRealRobo->setPath( path );
    pRealRobo->setName( comAssist::pureFileName(file,false) );
    pRealRobo->setFile( true );
    pRealRobo->setGc( true );

    pRealRobo->set( mcModelObj::model_scene_variable, pRealRobo );

    emit itemXActivated( pRealRobo );

    return 0;
}
int scriptMgr::openPy( const QString &path, const QString &file )
{
    mcModelObj *pModel;

    pModel = new mcModelObj();
    Q_ASSERT( NULL != pModel );

    pModel->setPath( path );
    pModel->setName( comAssist::pureFileName(file) );
    pModel->setGc( true );
    pModel->setFile( true );

    pModel->set( mcModelObj::model_py_file, pModel );

    emit itemXActivated( pModel );

    return ERR_NONE;
}

int scriptMgr::saveAll()
{
    //! prj
    m_pRootModel->save( getFullName() );

    return 0;
}

int scriptMgr::saveActiveFile()
{
    QModelIndex index = ui->scriptView->currentIndex();

    if ( !index.isValid() )
    { return ERR_INVALID_SCRIPT_FILE_NODE; }

    scriptNode *pNode = (scriptNode*)( index.internalPointer() );
    if ( NULL == pNode || pNode->getNodeType() != scriptNode::node_file )
    { return ERR_INVALID_SCRIPT_FILE_NODE; }

    motionGroup *pGroup = dynamic_cast<motionGroup*>( pNode );
    if ( NULL == pGroup )
    { return ERR_INVALID_SCRIPT_FILE_NODE; }

    QString fullName = getPath() + QDir::separator() + pGroup->getPath() + QDir::separator() + pGroup->getName();
    fullName = QDir::toNativeSeparators( fullName );
    return saveActiveFile( fullName );
}

void scriptMgr::newFile( const QString &path, const QString &name )
{   
    scriptFile *pFile = new scriptFile( comAssist::pureFileName(name) );

    pFile->setPath( path );

    m_pRootModel->appendNode( ui->scriptView->currentIndex(), pFile );
}
void scriptMgr::newGroup( const QString &name )
{
    scriptGroup *pGroup = new scriptGroup( name );

    m_pRootModel->appendNode( ui->scriptView->currentIndex(), pGroup );
}

void scriptMgr::deleteCurrent()
{
    m_pRootModel->removeNode( ui->scriptView->currentIndex() );
}

QString scriptMgr::defaultName()
{
    QString interName;
    { interName = QString("Untitled_%1").arg(mUntitleId++); }

    return interName;
}

//! all scene file in the project
QStringList scriptMgr::sceneList( QStringList &pathList )
{
    //! get all scene
    QStringList nameList, refPathList;
    nameList = sceneList( refPathList,
                          (scriptGroup*)m_pRootModel->getItem( QModelIndex() ) );

    QStringList strList;
    //! add the abs path
    Q_ASSERT( nameList.size() == refPathList.size() );
    for( int i = 0; i < nameList.size(); i++ )
    {
        strList << nameList[i];
        pathList<< QDir::toNativeSeparators( getPath() + QDir::separator() + refPathList[i] );
    }

    return strList;
}

QStringList scriptMgr::sceneList( QStringList &pathList,
                       scriptGroup *pGroup )
{
    Q_ASSERT( NULL != pGroup );

    QStringList fileNameList;
    for ( int i = 0; i < pGroup->childCount(); i++ )
    {
        Q_ASSERT( pGroup->child(i) != NULL );

        //! a group
        if ( pGroup->child(i)->getNodeType() == scriptNode::node_group )
        {
            QStringList subNameList, subPathList;

            subNameList = sceneList( subPathList,
                                     (scriptGroup*)pGroup->child(i) );
            fileNameList<<subNameList;
            pathList<<subPathList;
        }

        //! a file
        if ( pGroup->child(i)->getNodeType() == scriptNode::node_file )
        {
            if ( pGroup->child(i)->getName().contains(".sce") )
            {
                fileNameList<<pGroup->child(i)->getName();
                pathList<<pGroup->child(i)->getPath();
            }
            else
            {}
        }
    }

    return fileNameList;
}

roboSceneModel *scriptMgr::createScene( const QString &path,
                           const QString &file
                            )
{
    roboSceneModel *pModel;

    pModel = new roboSceneModel();
    Q_ASSERT( NULL != pModel );
    if ( pModel->load( file ) != 0 )
    {
        delete pModel;
        return NULL;
    }

    pModel->setPath( path );
    pModel->setName( comAssist::pureFileName(file) );
    pModel->setFile( true );
    pModel->setGc( true );

    pModel->set( mcModelObj::model_scene_file, pModel );

    return pModel;

}

void scriptMgr::iterAllItems()
{
//    scriptModel *pRoot;

//    scriptModel *pRoot = m_pRootModel;
//    Q_ASSERT( NULL != pRoot );

//    QModelIndex index;

//    if ( pRoot->hasChildren() )
//    {
//        int rowCount = pRoot->rowCount();


//        index = pRoot->index( 0, 0 );

//    }
}

void scriptMgr::setupUi()
{
    //! context menu
    m_pContextMenu = new QMenu(this);

    m_pNewGroupAction = m_pContextMenu->addAction( tr("New Group"), this,  SLOT(slot_context_newgroup()) );
    m_pImportAction = m_pContextMenu->addAction( tr("Import..."), this,  SLOT(slot_context_import()) );
    m_pContextMenu->addSeparator();
    m_pRemoveAction = m_pContextMenu->addAction( tr("Remove"), this, SLOT(slot_context_delete()) );

    //! model
    ui->scriptView->setModel( m_pRootModel );

    //! file system
//    ui->fileView->setModel( m_pFileSysModel );
//    ui->fileView->setRootIndex( m_pFileSysModel->index( QDir::currentPath() ) );
}
void scriptMgr::buildConnection()
{
//    connect( ui->scriptView, SIGNAL())
}

void scriptMgr::on_scriptView_activated(const QModelIndex &index)
{
    if ( !index.isValid() )
    { return; }

    scriptNode *pNode = static_cast<scriptNode*>( index.internalPointer() );
    if ( NULL == pNode || pNode->getNodeType() == scriptNode::node_group )
    { return; }

    scriptFile *pFile = dynamic_cast<scriptFile*>( pNode );
    if ( NULL == pFile )
    { return; }
    else
    {
        logDbg()<<pFile->getName();

        QString path = m_pRootModel->getPath() + QDir::separator() + pFile->getPath() + pFile->getName();

        logDbg()<<path;
    }

    QString path = m_pRootModel->getPath() + QDir::separator() + pFile->getPath();
    QString fullName;

    fullName = path + QDir::separator() + pFile->getName();

    path = QDir::toNativeSeparators( path );
    fullName = QDir::toNativeSeparators( fullName );

    int ret = openFile( path, fullName );
    if ( ret != 0 )
    {}
    else
    { logDbg(); }
}

//void scriptMgr::slot_context_newfile()
//{
//    bool ok;
//    QString text = QInputDialog::getText(this,
//                                         tr("File name"),
//                                         tr("File name:"),
//                                         QLineEdit::Normal,
//                                         defaultName(), &ok);
//    if (ok && !text.isEmpty())
//    { newFile( getPath(), text ); }
//}
void scriptMgr::slot_context_newgroup()
{
    bool ok;
    QString text = QInputDialog::getText(this,
                                         tr("Group name"),
                                         tr("Group name:"),
                                         QLineEdit::Normal,
                                         defaultName(), &ok);
    if (ok && !text.isEmpty())
    { newGroup( text ); }
}
void scriptMgr::slot_context_import()
{
    QFileDialog fDlg;

    fDlg.setAcceptMode( QFileDialog::AcceptOpen );
    QStringList nameFilters;
    nameFilters<<tr("pvt (*.pvt)")
               <<tr("motion (*.mc)")
               <<tr("setup (*.stp)")
               <<tr("scene (*.sce)")
               <<tr("python (*.py)");
    fDlg.setNameFilters( nameFilters );
    if ( QDialog::Accepted != fDlg.exec() )
    { return; }

    QString fileName = fDlg.selectedFiles().first();
    scriptFile *pFile = new scriptFile( comAssist::pureFileName( fileName ) );

    //! extract filename
    QString pureFileName = comAssist::pureFileName( fileName );

    //! base path
    QDir dir( m_pRootModel->getPath() );
    QString refPath = dir.relativeFilePath( fileName ).remove( pureFileName);
    //! remove the last
    refPath.remove( refPath.size() - 1, 1);
    pFile->setPath( refPath );

    m_pRootModel->appendNode( ui->scriptView->currentIndex(), pFile );
}
void scriptMgr::slot_context_delete()
{ deleteCurrent( ); }

