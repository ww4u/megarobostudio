#include "sinanjupref.h"
#include "ui_sinanjupref.h"
#include "../../robot/robotfact.h"
#include "../widget/megamessagebox.h"
SinanjuPref::SinanjuPref(QWidget *parent) :
    modelView(parent),
    ui(new Ui::SinanjuPref)
{
    ui->setupUi(this);

    //! fill table
    int i = 0;
    mLabels[i] = ui->label_7;i++;
    mLabels[i] = ui->label_6;i++;
    mLabels[i] = ui->label_5;i++;
    mLabels[i] = ui->label_4;i++;
    mLabels[i] = ui->label_3;i++;

    i = 0;
//    mEdits[i] = ui->lineEdit_6;i++;
//    mEdits[i] = ui->lineEdit_5;i++;
//    mEdits[i] = ui->lineEdit_4;i++;
//    mEdits[i] = ui->lineEdit_3;i++;
//    mEdits[i] = ui->lineEdit_2;i++;

    //! init angles
    mInitAngles.append( ui->spinAngleBase );
    mInitAngles.append( ui->spinAngleBArm );
    mInitAngles.append( ui->spinAngleSArm );
    mInitAngles.append( ui->spinAngleWrist );

    //! invert
    mInvertAngles.append( ui->chkInvBase );
    mInvertAngles.append( ui->chkInvBA );
    mInvertAngles.append( ui->chkInvSA );
    mInvertAngles.append( ui->chkInvWrist );

    spyEdited();
}

SinanjuPref::~SinanjuPref()
{
    delete ui;
}

void SinanjuPref::setModelObj( mcModelObj *pObj )
{
    modelView::setModelObj( pObj );

    initModel();

    updateUi();
}

//! set group id
int SinanjuPref::setApply()
{
    updateData();

    return 0;
}

void SinanjuPref::updateData()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    //! init angle
    pBase->mInitAngles.clear();
    for ( int i = 0; i < 4; i++ )
    {
        pBase->mInitAngles.append( mInitAngles.at(i)->value() );
    }

    //! invert angles
    pBase->mAngleDir.clear();
    for ( int i = 0; i < 4; i++ )
    {
        pBase->mAngleDir.append( mInvertAngles.at(i)->isChecked() );
    }

    robotSinanju *pRobo = (robotSinanju*)pBase;
    if ( NULL == pRobo )
    {
        sysError( tr("Invalid robot") );
        return;
    }

    //! hand zero
    pRobo->setHandZeroAttr( ui->spinZeroTime->value(),
                            ui->spinZeroAngle->value(),
                            ui->spinZeroSpeed->value() );
}

void SinanjuPref::updateUi()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;

    Q_ASSERT( pBase->axes() >= pBase->mInitAngles.size() );

    //! init angle
    for ( int i = 0; i < 4; i++ )
    {
        mInitAngles.at(i)->setValue( pBase->mInitAngles.at(i) );
    }

    for ( int i = 0; i < 4; i++ )
    {
        mInvertAngles.at(i)->setChecked( pBase->mAngleDir.at(i) );
    }

    //! zero hand
    robotSinanju *pRobo = (robotSinanju*)pBase;
    if ( NULL == pRobo )
    {
        sysError( tr("Invalid robot") );
        return;
    }

    double zeroTime, zeroSpeed, zeroAngle;
    pRobo->handZeroAttr( zeroTime, zeroAngle, zeroSpeed );
    ui->spinZeroTime->setValue( zeroTime );
    ui->spinZeroAngle->setValue( zeroAngle );
    ui->spinZeroSpeed->setValue( zeroSpeed );
}

void SinanjuPref::initModel()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    int i;
    for( i = 0; i < pBase->axes(); i++ )
    {
        mLabels[i]->setText( (pBase->mJointName.at(i)) );
    }
}

void SinanjuPref::spyEdited()
{
    QLineEdit *edits[]={

    };

    QSpinBox *spinBoxes[]={

    };

    QDoubleSpinBox *doubleSpinBoxes[]={
        ui->spinAngleWrist,
        ui->spinAngleSArm,
        ui->spinAngleBase,
        ui->spinAngleBArm,

        ui->spinZeroTime,
        ui->spinZeroAngle,
        ui->spinZeroSpeed,
    };

    QComboBox *comboxes[]={

    };

    install_spy();
}

void SinanjuPref::on_btnGoZero_clicked()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    MegaZeroAffirmMessageBox msgBox;
    int ret = msgBox.exec();
    if ( ret == QMessageBox::Ok )
    {
        pBase->goZero();
    }
}

void SinanjuPref::on_btnZeroHand_clicked()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    MegaZeroAffirmMessageBox msgBox;
    int ret = msgBox.exec();
    if ( ret == QMessageBox::Ok )
    {
        pBase->goZero( 4, ui->chkHandZeroCcw->isChecked() );
    }
}
