#include "robopref.h"
#include "ui_robopref.h"
#include "../../robot/robotfact.h"
roboPref::roboPref(QWidget *parent) :
    modelView(parent),
    ui(new Ui::roboPref)
{
    ui->setupUi(this);

    //! fill table
    int i = 0;
    mLabels[i] = ui->label_3;i++;
    mLabels[i] = ui->label_4;i++;
    mLabels[i] = ui->label_5;i++;
    mLabels[i] = ui->label_6;i++;
    mLabels[i] = ui->label_7;i++;

    i = 0;
    mEdits[i] = ui->lineEdit_2;i++;
    mEdits[i] = ui->lineEdit_3;i++;
    mEdits[i] = ui->lineEdit_4;i++;
    mEdits[i] = ui->lineEdit_5;i++;
    mEdits[i] = ui->lineEdit_6;i++;

    //! config validator
    m_pGroupIdValidaor = new QIntValidator( this );
    ui->edtGroupCanID->setValidator( m_pGroupIdValidaor );

    m_pGroupSubIdValidator = new QIntValidator( this );
    ui->cmbGpSubId->setValidator( m_pGroupSubIdValidator );
    m_pGroupSubIdValidator->setRange( sub_group_id_from, sub_group_id_to );
}

roboPref::~roboPref()
{
    delete ui;
}

void roboPref::setModelObj( mcModelObj *pObj )
{
    modelView::setModelObj( pObj );

    initModel();

    updateUi();
}

//! set group id
int roboPref::setApply()
{
    updateData();
logDbg();
    return applyGroupId();
}

void roboPref::updateData()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );
logDbg();
    pBase->mAxesConnectionName.clear();
    for ( int i = 0; i < pBase->axes(); i++ )
    {
        pBase->mAxesConnectionName<<mEdits[i]->text();
    }
logDbg();
    pBase->setName( ui->edtAlias->text() );
    pBase->setCanGroupId( ui->edtGroupCanID->text().toInt() );
    pBase->setSubGroup( ui->cmbGroup->currentIndex() );
    pBase->setSubGroupId( ui->cmbGpSubId->currentText().toInt() );
}

void roboPref::updateUi()
{

}

void roboPref::initModel()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    int i;
    for( i = 0; i < pBase->getAxes(); i++ )
    {
        mLabels[i]->setVisible( true );
        mEdits[i]->setVisible( true );

        mLabels[i]->setText( (pBase->mJointName.at(i)) );
        mEdits[i]->setText( (pBase->mAxesConnectionName.at(i)) );
    }

    //! other to false
    for ( ; i < 5; i++ )
    {
        mLabels[i]->setVisible( false );
        mEdits[i]->setVisible( false );
    }
    ui->edtAlias->setText( pBase->getName() );

    Q_ASSERT( NULL != m_pmcModel );
    ui->edtGroupCanID->setText( QString::number( pBase->canGroupId() ) );
    ui->cmbGroup->setCurrentIndex( pBase->subGroup() );
    ui->cmbGpSubId->setCurrentText( QString::number( pBase->subGroupId() ) );

    //! config range
    m_pGroupIdValidaor->setRange( m_pmcModel->mSysPref.mGroupIdFrom,
                                  m_pmcModel->mSysPref.mGroupIdTo );
}

int roboPref::applyGroupId()
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

