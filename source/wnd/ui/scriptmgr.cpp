#include "scriptmgr.h"
#include "ui_scriptmgr.h"
#include "../../com/comassist.h"

#include "../../model/tpvgroup.h"
#include "../../model/tpgroup.h"

#include "../../model/roboscenemodel.h"

#include "../../robot/robotfact.h"

#include "../model/sinanjumotiongroup.h"
#include "../model/deltamotiongroup.h"
#include "../model/megatronmotiongroup.h"
#include "../model/h2motiongroup.h"
#include "../model/h2zmotiongroup.h"
#include "../model/ipmotiongroup.h"

MegaTableModel *scriptMgr::newMotion( const QString &clsName )
{
    MegaTableModel *pNewModelObj;
    if ( clsName.compare( robo_t4, Qt::CaseInsensitive) == 0 )
    { pNewModelObj = new SinanjuMotionGroup( clsName ); }

    else if ( clsName.compare(robo_as, Qt::CaseInsensitive) == 0 )
    { pNewModelObj = new MegatronMotionGroup( clsName ); }

    else if ( clsName.compare( robo_dt, Qt::CaseInsensitive) == 0 )
    { pNewModelObj = new DeltaMotionGroup( clsName ); }

    else if ( clsName.compare( robo_h2, Qt::CaseInsensitive) == 0 )
    { pNewModelObj = new H2MotionGroup( clsName ); }

    else if ( clsName.compare( robo_h2m, Qt::CaseInsensitive) == 0 )
    { pNewModelObj = new H2MotionGroup( clsName ); }

    else if ( clsName.compare( robo_h2z, Qt::CaseInsensitive) == 0 )
    { pNewModelObj = new H2ZMotionGroup( clsName ); }

    else if ( clsName.compare( robo_ip, Qt::CaseInsensitive) == 0 )
    { pNewModelObj = new IPMotionGroup( clsName ); }
    else
    { return NULL; }

    return pNewModelObj;
}

scriptMgr::scriptMgr(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::scriptMgr)
{
    m_pContextMenu = NULL;
    mUntitleId = 0;

    m_pRootModel = NULL;
    m_pRootModel = new scriptModel();
    Q_ASSERT( NULL != m_pRootModel );

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
        QModelIndex index = ui->scriptView->indexAt( event->pos() );

        if ( index.isValid() )
        {}
        else
        { return; }

        //! check root
        if ( index.internalPointer() == NULL )
        { return; }

        //! is root?
//        if ( )
//             == m_pRootModel->rootNode() )
//        { m_pRemoveAction->setEnabled( false ); }
//        else

        scriptNode *pNode = static_cast<scriptNode*>(index.internalPointer());
        Q_ASSERT( NULL != pNode );
        m_pRemoveAction->setVisible( pNode->getParent() != 0 );

        m_pImportAction->setEnabled( getPath().length() > 0 );
        m_pNewGroupAction->setEnabled( getPath().length() > 0 );

        //! popup
        m_pContextMenu->popup( mapToGlobal( event->pos() ) );
    }
    //! file mgr
    else
    {}
}

QSize scriptMgr::sizeHint() const
{ return QSize(180,0);}

void scriptMgr::setExpand( bool b )
{
    if ( b )
    { ui->scriptView->expandAll(); }
    else
    { ui->scriptView->collapseAll(); }
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

//! full name
QString scriptMgr::getFullName()
{
    return QDir::fromNativeSeparators( m_pRootModel->getPath() + QDir::separator() + m_pRootModel->getName() );
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

    //! \todo
    SinanjuMotionGroup *pGroup = dynamic_cast<SinanjuMotionGroup*>( pNode );
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

    if ( comAssist::fileSuffixMatch( ext, motion_ext) )
    {
        return openMotionGroup( path, file );
    }
    else if ( comAssist::fileSuffixMatch( ext, pvt_ext) )
    {
        return openTpv( path, file );
    }
    else if ( comAssist::fileSuffixMatch( ext, pt_ext) )
    {logDbg();
        return openTp( path, file );
    }
    else if ( comAssist::fileSuffixMatch( ext, setup_ext) )
    {
        return openSetup( path, file );
    }
    else if ( comAssist::fileSuffixMatch( ext, scene_ext) )
    {
        return openScene( path, file );
    }
    else if ( comAssist::fileSuffixMatch( ext, script_ext) )
    {
        return openPy( path, file );
    }
    else
    {  }

    return ERR_FILE_DO_NOT_SUPPORT;
}

int scriptMgr::openMotionGroup( const QString &path, const QString &file )
{
    //! read the first line
    QFile fileRaw( file );
    if ( fileRaw.open(QIODevice::ReadOnly) )
    { }
    else
    { return ERR_FILE_OPEN_FAIL; }

    //! read model
    QByteArray ary;
    ary = fileRaw.readLine( 128 );
    fileRaw.close();
    QString lineStr(ary);
    if ( lineStr.startsWith("#!") )
    {}
    else
    { return ERR_FILE_DO_NOT_SUPPORT; }

    lineStr.remove("#!");
    lineStr = lineStr.trimmed().toLower();

    MegaTableModel *pModel;
    pModel = scriptMgr::newMotion( lineStr );
    if ( NULL == pModel )
    { return ERR_FILE_OPEN_FAIL; }

    int ret = pModel->load( file );
    if ( ret != 0 )
    {
        sysError( file, "do not exist" );
        delete pModel;
        return ret;
    }

//    logDbg()<<pModel->mItems.size();
    pModel->setPath( path );
    pModel->setName( comAssist::pureFileName(file) );
    pModel->setFile( true );
    pModel->setGc( true );

    pModel->set( mcModelObj::model_motion_file,
                 pModel );

    emit itemXActivated( pModel );

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
        sysError( file, "do not exist" );
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

int scriptMgr::openTp( const QString &path, const QString &file )
{
    TpGroup *pGroup;

    pGroup = new TpGroup();
    Q_ASSERT( NULL != pGroup );
    pGroup->setGc( true );
logDbg()<<file;
    int ret = pGroup->load( file );
    if ( ret != 0 )
    {
        sysError( file, "do not exist" );
        delete pGroup;
        return ret;
    }

    logDbg()<<pGroup->mItems.size();

    pGroup->setPath( path );
    pGroup->setName( comAssist::pureFileName(file) );
    pGroup->setFile( true );
    pGroup->setGc( true );

    pGroup->set( mcModelObj::model_tp,
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

//! \todo
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
        sysError( file, "do not exist" );
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

    MegaTableModel *pGroup = dynamic_cast<MegaTableModel*>( pNode );
    if ( NULL == pGroup )
    { return ERR_INVALID_SCRIPT_FILE_NODE; }

    QString fullName = getPath() + QDir::separator() + pGroup->getPath() + QDir::separator() + pGroup->getName();
    fullName = QDir::fromNativeSeparators( fullName );
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

void scriptMgr::removeCurrent()
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
            if ( pGroup->child(i)->getName().contains( scene_d_ext ) )
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

//! full name list
QStringList scriptMgr::fileList()
{
    //! get all file
    QStringList nameList;
    QStringList pathList;
    nameList = fileList( pathList,
                         (scriptGroup*)m_pRootModel->getItem( QModelIndex() ) );

    //! to abs path
    QStringList absFullList;
    QString absPath;
    Q_ASSERT( pathList.size() == nameList.size() );

    QDir prjDir( getPath() );
    QString refFile;
    for ( int i = 0; i < pathList.size(); i++ )
    {
        refFile = pathList[i] + nameList[i];

        absPath = prjDir.absoluteFilePath( refFile );
        absPath = QDir::fromNativeSeparators( absPath );

        absFullList<<absPath;
    }

    return absFullList;
}

QStringList scriptMgr::fileList( QStringList &pathList, scriptGroup *pGroup )
{
    Q_ASSERT( NULL != pGroup );

    QStringList fileNameList;
    for ( int i = 0; i < pGroup->childCount(); i++ )
    {
        Q_ASSERT( pGroup->child(i) != NULL );

        //! a group
        if ( pGroup->child(i)->getNodeType() == scriptNode::node_group )
        {
            QStringList subFileList;
            QStringList subPathList;

            subFileList = fileList( subPathList, (scriptGroup*)pGroup->child(i) );

            fileNameList<<subFileList;
            pathList<<subPathList;
        }

        //! a file
        if ( pGroup->child(i)->getNodeType() == scriptNode::node_file )
        {
            //! path contains seperator
            fileNameList<<pGroup->child(i)->getName();
            pathList<<pGroup->child(i)->getPath();
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
    m_pRemoveAction = m_pContextMenu->addAction( tr("Remove"), this, SLOT(slot_context_remove()) );

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

bool scriptMgr::isExist( const QString &fullName )
{
    QStringList fullFileList = fileList();
logDbg()<<fullFileList;
    if( fullFileList.contains( fullName, Qt::CaseInsensitive ) )
    { return true; }
    else
    { return false; }
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

    path = QDir::fromNativeSeparators( path );
    fullName = QDir::fromNativeSeparators( fullName );

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
    {
        newGroup( text );

        emit signal_prj_edited();
    }
}
void scriptMgr::slot_context_remove()
{
    removeCurrent( );logDbg();

    emit signal_prj_edited();
}

void scriptMgr::slot_context_import()
{
    QFileDialog fDlg;

    fDlg.setAcceptMode( QFileDialog::AcceptOpen );
    fDlg.setFileMode( QFileDialog::ExistingFiles );
    QStringList nameFilters;
    nameFilters<<tr("pvt (*.pvt)")
               <<tr("pt (*.pt)")
               <<tr("motion (*.mc)")
               <<tr("scene (*.sce)")
               <<tr("setup (*.stp)")
               <<tr("python (*.py)");
    fDlg.setNameFilters( nameFilters );

    if ( QDialog::Accepted != fDlg.exec() )
    { return; }

    //! save filter
    mSelectedNameFilter = fDlg.selectedNameFilter();

    foreach ( QString fileName, fDlg.selectedFiles() )
    {
        //! check repeat
        if ( isExist( QDir::fromNativeSeparators(fileName) ) )
        {
            sysPrompt( tr("File is alreay in project, ") + fileName );
            continue;
        }
        else
        { }

        //! new one
        scriptFile *pFile = new scriptFile( comAssist::pureFileName( fileName ) );

        //! extract filename
        QString pureFileName = comAssist::pureFileName( fileName );

        //! find repeat

        //! base path
        QDir dir( m_pRootModel->getPath() );
        QString refPath = dir.relativeFilePath( fileName ).remove( pureFileName);
        //! remove the last
        refPath.remove( refPath.size() - 1, 1);
        pFile->setPath( refPath );

        m_pRootModel->appendNode( ui->scriptView->currentIndex(), pFile );
    }

    emit signal_prj_edited();
}


