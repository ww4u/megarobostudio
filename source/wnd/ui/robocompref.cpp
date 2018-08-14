#include "robocompref.h"
#include "ui_robocompref.h"

#include "../../com/comassist.h"

#define time_unit   (1.0e-6f)

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

//    ui->label_10->setVisible(false);
//    ui->cmbGroup->setVisible(false);

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
    QRadioButton *radBoxes[] = {
    };
    QLineEdit *edits[]={
//        ui->edtAlias,
    };

    QSpinBox *spinBoxes[]={
//        ui->spinBox,
//        ui->spinBox_2,
    };

    QDoubleSpinBox *doubleSpinBoxes[]={
        ui->spinInterpStep,
        ui->spinZeroSpeed,

        ui->spinZeroTmo,
        ui->spinZeroTick,
    };

    QComboBox *comboxes[]={
//        ui->cmbGroup,
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

//    ui->edtAlias->setText( pBase->getName() );
    ui->widget->setAlias( pBase->getName() );

    Q_ASSERT( NULL != m_pmcModel );
    ui->widget->setGroupSel( pBase->groupSel() );
    ui->widget->setGroupId( pBase->canGroupId() );

    ui->widget->setSubGroupIndex( pBase->subGroup() );
    ui->widget->setSubGroupId( pBase->subGroupId() );

    double spd;
    int zeroTmo, zeroTick;
    pBase->zeroPref( spd, zeroTmo, zeroTick );
    ui->spinZeroSpeed->setValue( spd );
    ui->spinZeroTmo->setValue( zeroTmo * time_unit );
    ui->spinZeroTick->setValue( zeroTick * time_unit );

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

    pRobo->setGroupSel( ui->widget->groupSel() );
    pRobo->setCanGroupId( ui->widget->groupId() );

    pRobo->setSubGroup( ui->widget->subGroupIndex() );
    pRobo->setSubGroupId( ui->widget->subGroupId() );

    //! interp
//    RawRobo *pRawRobo = (RawRobo*)pRobo;
//    pRawRobo->setPlanStep( ui->spinInterpStep->value() );
//    pRawRobo->setPlanMode( (eRoboPlanMode)ui->cmbInterpMode->currentIndex() );

    pRobo->setPlanAttr( PlanAttr( (eRoboPlanMode)ui->cmbInterpMode->currentIndex(),
                                  ui->spinInterpStep->value()) );

    //! zero speed
    pRobo->setZeroPref( ui->spinZeroSpeed->value(),
                        comAssist::align( ui->spinZeroTmo->value(), time_unit ),
                        comAssist::align( ui->spinZeroTick->value(), time_unit )
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

//        //! disable the other group
//        for ( int i = 0; i < ui->cmbGroup->count(); i++ )
//        {
//            if ( i != (MRQ_IDENTITY_GROUP)ui->cmbGroup->currentIndex() )
//            {
//                pMRQ->setIDENTITY_GROUP( axesId,
//                                        (MRQ_IDENTITY_GROUP)( i ),
//                                         sub_group_id_from );
//            }
//        }

        pMRQ->setIDENTITY_GROUP( axesId,
                                 (MRQ_IDENTITY_GROUP)ui->widget->subGroupIndex(),
                                 ( ui->widget->subGroupId() ) );

        pMRQ->setCAN_APPLYPARA();
    }

    return 0;
}
