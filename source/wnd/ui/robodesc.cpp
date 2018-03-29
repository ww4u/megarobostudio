#include "robodesc.h"
#include "ui_robodesc.h"

RoboDesc::RoboDesc(QWidget *parent) :
    modelView(parent),
    ui(new Ui::RoboDesc)
{
    ui->setupUi(this);

    SET_INFO_ATTR();
}

RoboDesc::~RoboDesc()
{
    delete ui;
}

void RoboDesc::setModelObj( mcModelObj *pObj )
{
    modelView::setModelObj( pObj );

    updateUi();
}

void RoboDesc::updateUi()
{
    Q_ASSERT( NULL != m_pModelObj );

    VRobot *pRobot = (VRobot*)m_pModelObj->getObj();
    Q_ASSERT( NULL != pRobot );

    ui->edtDesc->setText( pRobot->getDetail() );
}

