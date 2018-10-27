#include "roboscene.h"
#include "ui_roboscene.h"

#include "../../widget/flowlayout.h"
#include "../widget/megamessagebox.h"
roboScene::roboScene(QWidget *parent) :
    modelView(parent),
    ui(new Ui::roboScene)
{
    ui->setupUi(this);

    mFilePattern<<"Scene file"<<"sce";

    init();

    setupUi();

    buildConnection();

    m_pLayout = new FlowLayout( this );
    Q_ASSERT( NULL != m_pLayout );
}

roboScene::~roboScene()
{
    delete ui;
}

void roboScene::mouseMoveEvent(QMouseEvent *event)
{
    //! left/top
    int x, y;
    x = event->pos().x() - mRefPoint.x();
    y = event->pos().y() - mRefPoint.y();
    if ( x < 0 )
    { x = 0; }
    if ( y < 0 )
    { y = 0; }

    foreach( sceneWidget*pItem, mItemList )
    {
        if ( pItem->getSelected() )
        {
            pItem->move( x, y );
        }
    }
}

void roboScene::mousePressEvent(QMouseEvent *event)
{
    selectAll( false );

    //! dynamic cast
    sceneWidget *pWidget = dynamic_cast<sceneWidget*>( childAt( event->pos() ) );

    if ( NULL != pWidget )
    {
        pWidget->setFocus();

        pWidget->setSelected(true);

        pWidget->raise();
        pWidget->setFrameShape( QFrame::StyledPanel );

        mRefPoint = pWidget->mapFromParent( event->pos() );
    }
}

void roboScene::contextMenuEvent(QContextMenuEvent *event)
{
    if ( selectCount()>0 )
    {
        sceneWidget *pWig = theSelectedFirst();
        if ( NULL== pWig )
        { return; }

        //! get model
        VRobot *pRobo;
        pRobo = (VRobot*)pWig->getModelObj();
        if ( pRobo == NULL )
        { return; }

        m_pNewMrp->setVisible( pRobo->mrpAble() );

        m_pContextMenu->popup( mapToGlobal( event->pos() ) );
    }
}

void roboScene::keyReleaseEvent(QKeyEvent *event)
{
    if ( event->key()==Qt::Key_Delete )
    {
        context_delete();
    }
}

void roboScene::mouseDoubleClickEvent(QMouseEvent *event)
{
    sceneWidget *pWidget = (sceneWidget*)childAt( event->pos() );
    if ( NULL == pWidget )
    { return; }

//    VRobot *pRobo = (VRobot*)pWidget->getModelObj();
//    if ( NULL == pRobo )
//    { return; }

//    if ( )
    if ( NULL != pWidget )
    { emit itemXActivated( pWidget->getModelObj() ); }
}

void roboScene::context_delete()
{
    QList< sceneWidget *> delList;

    //! collect
    foreach( sceneWidget *pItem, mItemList )
    {
        if ( pItem->getSelected() )
        { delList.append( pItem ); }
    }

    //! delete
    foreach( sceneWidget *pItem, delList )
    {
        mItemList.removeAll( pItem );
    }

    //! delete resource
    delete_all( delList );

    emit sigSaveRequest( this );
    emit signalSceneChanged();
}
void roboScene::context_option()
{
    //! find the select one
    foreach( sceneWidget *pItem, mItemList )
    {
        if ( NULL != pItem && pItem->getSelected() )
        {
            emit itemXActivated( pItem->getModelObj() );
            return;
        }
    }
}

void roboScene::context_new_mrp()
{
    //! find the select one
    foreach( sceneWidget *pItem, mItemList )
    {
        if ( NULL != pItem && pItem->getSelected() )
        {
            emit itemXActivated( pItem->getModelObj(), model_obj_op_new_mrp );
            return;
        }
    }
}

void roboScene::slot_editingFinished( sceneWidget *pWig, const QString &str )
{
    if( checkName( str, pWig ) )
    {
        emit sigSaveRequest( this );
        emit signalSceneChanged();
    }
    else
    {
        pWig->setName( tr("untitled") );
        MegaMessageBox::warning( this, tr("Warning"), tr("Name Inavlid") );
        pWig->focusOnName();
    }
}

void roboScene::setModelObj( mcModelObj *pObj )
{
    modelView::setModelObj( pObj );

    //! get roboscene model
    Q_ASSERT( NULL != pObj->getObj() );
    roboSceneModel *pModel = (roboSceneModel*)pObj->getObj();
    Q_ASSERT( NULL != pModel );

    fromSceneModel( pModel, pModel->getPath() );
}

int roboScene::save( QString &outFileName )
{
    roboSceneModel lSceneModel;

    toSceneModel( &lSceneModel );

    outFileName = m_pModelObj->getPath() + QDir::separator() + m_pModelObj->getName();
    outFileName = QDir::toNativeSeparators( outFileName );
    return lSceneModel.save( outFileName );
}

int roboScene::saveAs( QString &name )
{
    roboSceneModel lSceneModel;

    toSceneModel( &lSceneModel );

    return lSceneModel.save( name );
}

sceneWidget * roboScene::addRobot( mcModelObj *pBase )
{
    Q_ASSERT( NULL != pBase );

    //! change type
    pBase->setType( mcModelObj::model_scene_variable );
    sceneWidget *pItem = new sceneWidget( ui->scrollArea );   //! has parent, no need to delete
    Q_ASSERT( NULL != pItem );


    pItem->setModelObj( pBase );                    //! attach robot

    pItem->setVisible( true );
                                                    //! connection
    connect( pItem, SIGNAL(editingFinished(sceneWidget*, const QString &)),
             this, SLOT(slot_editingFinished(sceneWidget*,const QString &)));

    mItemList.append( pItem );

    emit sigSaveRequest( this );
    emit signalSceneChanged();

    return pItem;
}

sceneWidget * roboScene::addRobot( mcModelObj *pBase, sceneModel *pModel )
{
    Q_ASSERT( NULL != pBase );

    sceneWidget *pWig;
    pWig = addRobot( pBase );
    pWig->setGeometry( pModel->mX, pModel->mY,
                       pModel->mW, pModel->mH );
    return pWig;
}

int roboScene::load( const QString &name, const QString &path )
{
    roboSceneModel lSceneModel;

    lSceneModel.load( name );

    fromSceneModel( &lSceneModel, path );

    //! all to visible
    return 0;
}

QList<VRobot *> roboScene::complexRobotList()
{
    QList<VRobot *> roboList;

    VRobot *pRobo;
    foreach (sceneWidget *pWig, mItemList)
    {
        Q_ASSERT( NULL != pWig );
        pRobo = pWig->getRobot();

        Q_ASSERT( NULL != pRobo );
        if ( robot_is_robot( pRobo->getId()) )
        {
            roboList.append( pRobo );
        }
    }

    return roboList;
}

VRobot * roboScene::findRobot( const QString &name )
{
    VRobot *pRobo;
    foreach (sceneWidget *pWig, mItemList)
    {
        Q_ASSERT( NULL != pWig );
        pRobo = pWig->getRobot();

        Q_ASSERT( NULL != pRobo );
        if ( pRobo->name() == name )
        {
            return pRobo;
        }
    }

    return NULL;
}

//! to model
void roboScene::toSceneModel( roboSceneModel *pSceneModel )
{
    Q_ASSERT( NULL != pSceneModel );
    sceneModel *pModel;

    foreach( sceneWidget *pWidget, mItemList )
    {
        pModel = new sceneModel();

        pModel->mClass = pWidget->getRobot()->getClass();
        pModel->mName = pWidget->getName();

        pModel->mX = pWidget->geometry().x();
        pModel->mY = pWidget->geometry().y();
        pModel->mW = pWidget->geometry().width();
        pModel->mH = pWidget->geometry().height();

        pSceneModel->mSceneItems.append( pModel );
    }
}

void roboScene::fromSceneModel( roboSceneModel *pSceneModel,
                                const QString &path )       //! ref path
{
    //! delete current
    delete_all( mItemList );

    QString fullName;
    foreach( sceneModel *pModel, pSceneModel->mSceneItems )
    {
        //! robot
        VRobot *pNewRobot = robotFact::createRobot(pModel->mClass);
        Q_ASSERT( NULL != pNewRobot );

        pNewRobot->set( mcModelObj::model_scene_variable, pNewRobot );
        pNewRobot->setName( pModel->mName );

        //! try to load the stp from the scene path
        fullName = path + QDir::separator() + pModel->mName +  setup_d_ext;
        fullName = QDir::toNativeSeparators( fullName );
        QFile file( fullName );
        if ( file.exists() )
        {
            pNewRobot->load( fullName );
            sysLog( fullName, tr("loaded!") );
        }

        pNewRobot->setPath( path );

        //! add to scene
        addRobot( pNewRobot, pModel );
    }
}

bool roboScene::checkName( const QString &name,
                           sceneWidget *pSelf )
{
    if ( name.length() < 1 )
    { return false; }

    //! check repeat name
    foreach( sceneWidget *pWig, mItemList )
    {
        Q_ASSERT( NULL != pWig );

        if ( pWig == pSelf )
        { continue; }

        if ( name == pWig->getName() )
        { return false; }
    }

    return true;
}

void roboScene::init()
{
    m_pContextMenu = NULL;
}
void roboScene::deInit()
{
}

void roboScene::setupUi()
{
    m_pContextMenu = new QMenu(this);

    m_pDelete = m_pContextMenu->addAction( tr("Delete"),
                                           this,
                                           SLOT(context_delete()) );
    m_pOption = m_pContextMenu->addAction( tr("Option..."),
                                           this,
                                           SLOT(context_option()));

    m_pNewMrp = m_pContextMenu->addAction( tr("New mrp"),
                                           this,
                                           SLOT(context_new_mrp()));

}
void roboScene::buildConnection()
{

}

void roboScene::selectAll( bool bSel )
{
    foreach( sceneWidget*pItem, mItemList )
    {
        Q_ASSERT( NULL != pItem );
        pItem->setSelected( bSel );
    }

    updateSelecte();
}

void roboScene::updateSelecte()
{
    foreach( sceneWidget*pItem, mItemList )
    {
        Q_ASSERT( NULL != pItem );
        if ( pItem->getSelected() )
        {
            pItem->setFrameShape( QFrame::StyledPanel );
        }
        else
        {
            pItem->setFrameShape( QFrame::NoFrame );
        }
    }
}

int roboScene::selectCount()
{
    int count = 0;
    foreach( sceneWidget*pItem, mItemList )
    {
        count += pItem->getSelected()? 1:0;
    }
    return count;
}

sceneWidget *roboScene::theSelectedFirst()
{
    foreach( sceneWidget*pItem, mItemList )
    {
        if ( pItem->getSelected() )
        { return pItem; }
    }

    return NULL;
}
