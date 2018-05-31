#include "robocompref.h"
#include "ui_robocompref.h"

RoboComPref::RoboComPref(QWidget *parent) :
    modelView(parent),
    ui(new Ui::RoboComPref)
{
    ui->setupUi(this);

    //! connection
    mAxesLabels.append( ui->label_2 );
    mAxesLabels.append( ui->label_3 );
    mAxesLabels.append( ui->label_4 );
    mAxesLabels.append( ui->label_5 );
    mAxesLabels.append( ui->label_6 );
    mAxesLabels.append( ui->label_7 );
    mAxesLabels.append( ui->label_12 );
    mAxesLabels.append( ui->label_13 );
    mAxesLabels.append( ui->label_14 );
    mAxesLabels.append( ui->label_15 );

    mAxesEdits.append( ui->lineEdit );
    mAxesEdits.append( ui->lineEdit_2 );
    mAxesEdits.append( ui->lineEdit_3 );
    mAxesEdits.append( ui->lineEdit_4 );
    mAxesEdits.append( ui->lineEdit_5 );
    mAxesEdits.append( ui->lineEdit_6 );
    mAxesEdits.append( ui->lineEdit_7 );
    mAxesEdits.append( ui->lineEdit_8 );
    mAxesEdits.append( ui->lineEdit_9 );
    mAxesEdits.append( ui->lineEdit_10 );

    //! hide all
    foreach( QLineEdit *pEdit, mAxesEdits )
    {
        pEdit->setVisible( false );
    }

    foreach ( QLabel *pLabel, mAxesLabels )
    {
        pLabel->setVisible( false );
    }

    ui->label_10->setVisible(false);
    ui->cmbGroup->setVisible(false);

    spyEdited();
}

RoboComPref::~RoboComPref()
{
    delete ui;
}

void RoboComPref::setModelObj( mcModelObj *pObj )
{
    modelView::setModelObj( pObj );

    adaptUi();

    updateUi();
}

int RoboComPref::setApply()
{
    updateData();

    applyGroupId();

    return 0;
}

void RoboComPref::updateScreen()
{
    updateUi();
}

void RoboComPref::spyEdited()
{
    QCheckBox *checkBoxes[]=
    {
    };

    QLineEdit *edits[]={
        ui->edtAlias,
    };

    QSpinBox *spinBoxes[]={
        ui->spinBox,
        ui->spinBox_2,

        ui->spinZeroTmo,
        ui->spinZeroTick,
    };

    QDoubleSpinBox *doubleSpinBoxes[]={
        ui->spinInterpStep,
        ui->spinZeroSpeed,
    };

    QComboBox *comboxes[]={
//        ui->cmbGpSubId,
        ui->cmbGroup,
        ui->cmbInterpMode,
    };

    install_spy();

    foreach( QLineEdit *pEdit, mAxesEdits )
    {
        spy_control_edit( pEdit );
    }
}

void RoboComPref::updateUi()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    ui->edtAlias->setText( pBase->getName() );

    Q_ASSERT( NULL != m_pmcModel );
    ui->spinBox->setValue( pBase->canGroupId() );
    ui->cmbGroup->setCurrentIndex( pBase->subGroup() );
//    ui->cmbGpSubId->setCurrentText( QString::number( pBase->subGroupId() ) );

    double spd;
    int zeroTmo, zeroTick;
    pBase->zeroPref( spd, zeroTmo, zeroTick );
    ui->spinZeroSpeed->setValue( spd );
    ui->spinZeroTmo->setValue( zeroTmo );
    ui->spinZeroTick->setValue( zeroTick );

    //! connection name
    for ( int i = 0; i < pBase->axes(); i++ )
    {
        mAxesEdits[i]->setText( pBase->mAxesConnectionName.at(i) );
        mAxesEdits[i]->setVisible( true );
    }

    //! axes name
    for ( int i = 0; i < pBase->axes(); i++ )
    {
        mAxesLabels[i]->setText( pBase->mJointName.at(i) );
        mAxesLabels[i]->setVisible( true );
    }

    //! interp
    RawRobo *pRawRobo = (RawRobo*)pBase;
    ui->spinInterpStep->setValue( pRawRobo->planAttr().mStep );
    ui->cmbInterpMode->setCurrentIndex( (int)pRawRobo->planAttr().mMode );
}

void RoboComPref::updateData()
{
    VRobot *pRobo;

    //! set to self
    pRobo = (VRobot*)m_pModelObj;
    if ( NULL == pRobo )
    { return; }

    pRobo->setCanGroupId( ui->spinBox->value() );

    //! interp
//    RawRobo *pRawRobo = (RawRobo*)pRobo;
//    pRawRobo->setPlanStep( ui->spinInterpStep->value() );
//    pRawRobo->setPlanMode( (eRoboPlanMode)ui->cmbInterpMode->currentIndex() );

    pRobo->setPlanAttr( PlanAttr( (eRoboPlanMode)ui->cmbInterpMode->currentIndex(),
                                  ui->spinInterpStep->value()) );

    //! zero speed
    pRobo->setZeroPref( ui->spinZeroSpeed->value(),
                        ui->spinZeroTmo->value(),
                        ui->spinZeroTick->value()
                        );

    //! set connection
    pRobo->mAxesConnectionName.clear();
    for ( int i = 0; i < pRobo->axes(); i++ )
    { pRobo->mAxesConnectionName<<mAxesEdits.at(i)->text(); }
}

void RoboComPref::adaptUi()
{
    VRobot *pRobo;

    //! set to self
    pRobo = (VRobot*)m_pModelObj;
    if ( NULL == pRobo )
    { return; }

    ui->gpTrace->setVisible( pRobo->interpAble() );
}

int RoboComPref::applyGroupId()
{
    VRobot *pRobo;

    //! set to self
    pRobo = (VRobot*)m_pModelObj;
    if ( NULL == pRobo )
    { return ERR_INVALID_DEVICE_NAME; }

    //! foreach sub device
    MegaDevice::deviceMRQ *pMRQ;
    int ret;
    int gpId, axesId;
    foreach( QString str, pRobo->mAxesConnectionName )
    {
        axesId = 0;

        //! find device by connection name
        pMRQ = pRobo->findDevice( str, &axesId );
        if ( NULL == pMRQ )
        {
            return ERR_INVALID_DEVICE_NAME;
        }

        //! set group id
        gpId = ui->spinBox->value();
        ret = pMRQ->setCAN_GROUPID1( gpId );
        if ( ret != 0 )
        {
            logDbg();
            return ERR_DEVICE_WRITE_FAIL;
        }

        //! set sub group
//        subGroup = ui->cmbGpSubId->currentText().toInt();
//        pMRQ->setIDENTITY_GROUP( axesId, (MRQ_IDENTITY_GROUP)ui->cmbGroup->currentIndex(), subGroup );

        pMRQ->setCAN_APPLYPARA();
    }

    return 0;
}
