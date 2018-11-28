#include "robomgr.h"
//#include "ui_robomgr.h"

#include "../../robot/robotfact.h"

roboMgr::roboMgr(QListWidget *parent) :
    QListWidget(parent)
//  ,
//    ui(new Ui::roboMgr)
{
//    ui->setupUi(this);

    mbOperable = false;

    setViewMode( IconMode );
    setIconSize( QSize(48,48) );

    setDragDropMode( NoDragDrop );
    setEditTriggers( NoEditTriggers );

    setResizeMode( Adjust );
    setMovement( Free );

    //! activated
    connect( this, SIGNAL(itemActivated(QListWidgetItem*)),
             this, SLOT(slot_itemActivated(QListWidgetItem*)) );
}

roboMgr::~roboMgr()
{
    delete_all( mRobots );
}

QMimeData *roboMgr::mimeData(const QList<QListWidgetItem *> items) const
{
    if ( items.size() == 1 )
    {}
    else
    { return NULL; }

    QListWidgetItem *pItem = items.at(0);

    QMimeData *pMime = new QMimeData();
    if ( NULL == pMime )
    { return NULL; }

    //! get role
    QVariant var = pItem->data( Qt::UserRole );
    VRobot *pBase = var.value<VRobot *>();

    pMime->setData( "robomgr/robot", pBase->getClass().toLatin1() );
    return pMime;
}

QStringList roboMgr::mimeTypes() const
{
    QStringList mimeList;
    return mimeList<<"robomgr/robot";
}

//QSize roboMgr::sizeHint() const
//{ return QSize(180,0);}

void roboMgr::setModel( const modelRobotMgr &mgr )
{
    foreach( QString str, mgr.mRobots )
    {
        mRobots.append( robotFact::createRobot(str) );
    }

    setupData();
}

void roboMgr::setupData()
{
    QListWidgetItem *pItem;
    foreach( VRobot *pRobo, mRobots )
    {
        Q_ASSERT( NULL != pRobo );
        pItem = new QListWidgetItem( QIcon( QPixmap::fromImage( pRobo->getImage() ) ),
                                     pRobo->getClass());
        Q_ASSERT( NULL != pItem );

        pRobo->set( mcModelObj::model_robot, pRobo );

        pRobo->setName( pRobo->getClass() );

        pItem->setData( Qt::UserRole, QVariant::fromValue( pRobo ) );
        pItem->setToolTip( pRobo->getClass() );

//        ui->listWidget->addItem( pItem );
        addItem( pItem );
    }
}

void roboMgr::setOperable( bool b )
{ mbOperable = b; }
bool roboMgr::operAble()
{ return mbOperable; }

//! double click
void roboMgr::slot_itemActivated(QListWidgetItem *item)
{
    if ( NULL == item )
    { return; }
logDbg();
    if ( mbOperable )
    {}
    else
    { return; }
logDbg();
    QVariant var = item->data( Qt::UserRole );

    VRobot *pBase = var.value<VRobot *>();
    if ( NULL != pBase )
    {
        //! create a new robot
        //! delete on the view
        VRobot *pNewRobot = robotFact::createRobot( pBase->getClass() );
        Q_ASSERT( NULL != pNewRobot );

        pNewRobot->setName( pBase->getClass() );
        pNewRobot->set( mcModelObj::model_robot, pNewRobot );

        Q_ASSERT( NULL != pNewRobot );

        emit itemXActivated( pNewRobot );

        emit signal_itemXHelp( e_help_new_robot );
    }
}
