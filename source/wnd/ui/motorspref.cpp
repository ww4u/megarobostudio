#include "motorspref.h"
#include "ui_motorspref.h"
#include "megamessagebox.h"
#include "../robot/motors/motors.h"

MotorsPref::MotorsPref(QWidget *parent) :
    modelView(parent),
    ui(new Ui::MotorsPref)
{
    ui->setupUi(this);

    //! connect
    connect( ui->tableBar, SIGNAL(signal_add_clicked()),
             this, SLOT(slot_add_clicked()));
    connect( ui->tableBar, SIGNAL(signal_del_clicked()),
             this, SLOT(slot_del_clicked()));
    connect( ui->tableBar, SIGNAL(signal_clr_clicked()),
             this, SLOT(slot_clr_clicked()));

}

MotorsPref::~MotorsPref()
{
    delete ui;
}

#define model_      ((robotMotors*)m_pModelObj)->getModel()
#define base_       ( (VRobot *)m_pModelObj )

void MotorsPref::setModelObj( mcModelObj *pObj )
{
    modelView::setModelObj( pObj );

    updateUi();

    robotMotors *pMotor = (robotMotors*)pObj;
    ui->tableView->setModel( pMotor->getModel() );
}
int MotorsPref::setApply()
{
    //! addr
    base_->setGroupSel( ui->widget->groupSel() );
    base_->setCanGroupId( ui->widget->groupId() );
    base_->setSubGroup( ui->widget->subGroupIndex() );
    base_->setSubGroupId( ( ui->widget->subGroupId() ) );

    //! connection name
    QStringList connectionS;
    for ( int i = 0;
          i < ((RelationModel*) model_)->mItems.size();
          i++ )
    {
        connectionS.append( ( (RelationModel*) model_)->mItems.at(i)->value() );
    }

    //! motors
    base_->setAxes( ((RelationModel*) model_)->mItems.size() );
    base_->mAxesConnectionName = connectionS;

    //! apply for each item
    VRobot *pRobo;

    //! set to self
    pRobo = (VRobot*)m_pModelObj;
    if ( NULL == pRobo )
    { return ERR_INVALID_DEVICE_NAME; }

    //! foreach sub device
    MegaDevice::deviceMRQ *pMRQ;
    int ret;
    int gpId, axesId;
    foreach( QString str, connectionS )
    {
        axesId = 0;

        //! find device by connection name
        pMRQ = pRobo->findDevice( str, &axesId );
        if ( NULL == pMRQ )
        {
            return ERR_INVALID_DEVICE_NAME;
        }

        //! set group id
        gpId = ui->widget->groupId();
        if ( ui->widget->groupSel() == 0 )
        { ret = pMRQ->setCAN_GROUPID1( gpId ); }
        else
        { ret = pMRQ->setCAN_GROUPID2( gpId ); }
        if ( ret != 0 )
        {
            logDbg();
            return ERR_DEVICE_WRITE_FAIL;
        }

        //! set sub group

        pMRQ->setIDENTITY_GROUP( axesId,
                                 (MRQ_IDENTITY_GROUP)ui->widget->subGroupIndex(),
                                 ( ui->widget->subGroupId() ) );

        pMRQ->setCAN_APPLYPARA();
    }

    return 0;
}

void MotorsPref::updateScreen()
{
    updateUi();
    //! connections by model
}

void MotorsPref::updateUi()
{
    ui->widget->setAlias( base_->getName() );
    ui->widget->setGroupSel( base_->groupSel() );
    ui->widget->setGroupId( base_->canGroupId() );
    ui->widget->setSubGroupIndex( base_->subGroup() );
    ui->widget->setSubGroupId( base_->subGroupId() );
}

void MotorsPref::slot_add_clicked()
{
    if ( ui->tableView->currentIndex().isValid() )
    { model_->insertRow( ui->tableView->currentIndex().row() + 1 ); }
    else
    { model_->insertRow(0); }
}
void MotorsPref::slot_del_clicked()
{
    model_->removeRow( ui->tableView->currentIndex().row() );
}
void MotorsPref::slot_clr_clicked()
{
    MegaDeleteAffirmMessageBox msgBox;
    int ret = msgBox.exec();
    if ( ret == QMessageBox::Ok )
    {
        model_->removeRows( 0, ui->tableView->model()->rowCount() );
    }
}

