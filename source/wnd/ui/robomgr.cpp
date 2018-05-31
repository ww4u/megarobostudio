#include "robomgr.h"
#include "ui_robomgr.h"

#include "../../robot/robotfact.h"
//#include "../../robot/megatron/megatron.h"


roboMgr::roboMgr(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::roboMgr)
{
    ui->setupUi(this);
}

roboMgr::~roboMgr()
{
    delete_all( mRobots );

    delete ui;
}

QSize roboMgr::sizeHint() const
{ return QSize(180,0);}

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

        ui->listWidget->addItem( pItem );
    }
}

void roboMgr::on_listWidget_itemActivated(QListWidgetItem *item)
{
    if ( NULL == item )
    { return; }

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
    }
}
