#include "robocompref.h"
#include "ui_robocompref.h"

RoboComPref::RoboComPref(QWidget *parent) :
    modelView(parent),
    ui(new Ui::RoboComPref)
{
    ui->setupUi(this);

    //! config validator
    m_pGroupIdValidaor = new QIntValidator( this );
    ui->edtGroupCanID->setValidator( m_pGroupIdValidaor );

    m_pGroupSubIdValidator = new QIntValidator( this );
    ui->cmbGpSubId->setValidator( m_pGroupSubIdValidator );
    m_pGroupSubIdValidator->setRange( sub_group_id_from, sub_group_id_to );
}

RoboComPref::~RoboComPref()
{
    delete ui;
}

void RoboComPref::setModelObj( mcModelObj *pObj )
{
    modelView::setModelObj( pObj );

    updateUi();
}

int RoboComPref::setApply()
{
    applyGroupId();
}

void RoboComPref::updateUi()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    ui->edtAlias->setText( pBase->getName() );

    Q_ASSERT( NULL != m_pmcModel );
    ui->edtGroupCanID->setText( QString::number( pBase->canGroupId() ) );
    ui->cmbGroup->setCurrentIndex( pBase->subGroup() );
    ui->cmbGpSubId->setCurrentText( QString::number( pBase->subGroupId() ) );
}

void RoboComPref::updateData()
{

}

int RoboComPref::applyGroupId()
{
    VRobot *pRobo;

    //! set to self
    pRobo = (VRobot*)m_pModelObj;
    if ( NULL == pRobo )
    { return ERR_INVALID_DEVICE_NAME; }
logDbg();
    //! foreach sub device
    MegaDevice::deviceMRQ *pMRQ;
    int ret;
    int gpId, subGroup, axesId;
    foreach( QString str, pRobo->mAxesConnectionName )
    {
        axesId = 0;
logDbg()<<str;
        //! find device by connection name
        pMRQ = pRobo->findDevice( str, &axesId );
        if ( NULL == pMRQ )
        {
            logDbg()<<str;
            return ERR_INVALID_DEVICE_NAME;
        }
logDbg();
        //! set group id
        gpId = ui->edtGroupCanID->text().toInt();
        ret = pMRQ->setCAN_GROUPID1( gpId );
        if ( ret != 0 )
        {
            logDbg();
            return ERR_DEVICE_WRITE_FAIL;
        }

        //! set sub group
        subGroup = ui->cmbGpSubId->currentText().toInt();
        pMRQ->setIDENTITY_GROUP( axesId, (MRQ_IDENTITY_GROUP)ui->cmbGroup->currentIndex(), subGroup );

        pMRQ->setCAN_APPLYPARA();
    }
logDbg();
//    //! for self id
//    pRobo->setGroupCanId( gpId );

//    //! set sub id
//    pRobo->setSubGroupId( subGroup );

    return 0;
}
