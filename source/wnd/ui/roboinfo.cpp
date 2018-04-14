#include "roboinfo.h"
#include "ui_roboinfo.h"

#include "../../robot/robotfact.h"

roboInfo::roboInfo(QWidget *parent) :
    modelView(parent),
    ui(new Ui::roboInfo)
{
    ui->setupUi(this);

    initModel();

    SET_INFO_ATTR();
}

roboInfo::~roboInfo()
{
    deInitModel();

    delete ui;
}

void roboInfo::setModelObj( mcModelObj *pObj )
{
    modelView::setModelObj( pObj );

    updateUi();
}

void roboInfo::updateScreen()
{
    updateUi();
}

void roboInfo::initModel()
{
    for( int i = 0; i < ui->tableWidget->rowCount(); i++ )
    {
        for ( int j = 0; j < ui->tableWidget->columnCount(); j++ )
        {
            mItems.append( new QTableWidgetItem() );

            ui->tableWidget->setItem( i, j, mItems.last() );
        }
    }

    foreach( QTableWidgetItem *pItem, mItems )
    {
        pItem->setFlags( Qt::ItemIsSelectable );
    }

    mItems[0]->setText( tr("Model") );
    mItems[2]->setText( tr("Axes") );
    mItems[4]->setText( tr("Id") );
}

void roboInfo::deInitModel()
{
//    foreach( QTableWidgetItem *pItem, mItems )
//    {
//       delete pItem;
//    }
}

void roboInfo::updateUi()
{
    Q_ASSERT( NULL != m_pModelObj );
//    if ( m_pModelObj->getType() != mcModelObj::model_scene_variable )
//    { return; }

    VRobot *pRobot = (VRobot*)m_pModelObj->getObj();
    Q_ASSERT( NULL != pRobot );

    mItems[1]->setText( pRobot->getClass() );
    mItems[3]->setData( Qt::DisplayRole, QVariant( pRobot->axes() ) );
    mItems[5]->setData( Qt::DisplayRole, QVariant( pRobot->getId() ) );

    ui->label->setPixmap( QPixmap::fromImage( pRobot->getImage() ) );
}
