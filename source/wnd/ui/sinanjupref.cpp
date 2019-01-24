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
    mLabels[i] = ui->labelBase;i++;
    mLabels[i] = ui->labelBA;i++;
    mLabels[i] = ui->labelSA;i++;
    mLabels[i] = ui->labelWrist;i++;
    mLabels[i] = ui->label_3;i++;

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

    ui->labelBase->setText( QString("%1(%2)").arg( tr("Base") ).arg( CHAR_DEGREE ) );
    ui->labelBA->setText( QString("%1(%2)").arg( tr("Big Arm") ).arg( CHAR_DEGREE ) );
    ui->labelSA->setText( QString("%1(%2)").arg( tr("Little Arm") ).arg( CHAR_DEGREE ) );
    ui->labelWrist->setText( QString("%1(%2)").arg( tr("Wrist") ).arg( CHAR_DEGREE ) );

    spyEdited();
}

SinanjuPref::~SinanjuPref()
{
    delete ui;
}

void SinanjuPref::changeEvent(QEvent * event)
{
    modelView::changeEvent( event );

    if (event->type() == QEvent::LanguageChange)
    { ui->retranslateUi( this ); }
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

void SinanjuPref::updateScreen()
{
    updateUi();
}

int SinanjuPref::setReset()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    if ( !pBase->checkLink() )
    {
        sysPrompt( tr("Invalid link") );
        return -1;
    }

    MegaDevice::deviceMRQ *pMrq;
    int ax;
    pMrq = pBase->jointDevice( 0, &ax );
    if ( NULL == pMrq )
    {
        sysPrompt( tr("Invalid link") );
        return -1;
    }

    //! write the setting
    pMrq->write( "*lrn \"mrq_sinanju.stp\"\r\n" );
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
                            ui->spinZeroAngle->value() );

    pRobo->setGapAttr( ui->spinGapTime->value(),
                       ui->spinGapAngle->value() );

    pRobo->setHandAble( ui->gpHand->isChecked() );

    //! ccw
    pRobo->setJointZeroCcw( 4, ui->chkHandZeroCcw->isChecked() );

    //! coord
    pRobo->setCoord( (eRoboCoord)ui->coordOption->option() );
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

    double zeroTime, zeroAngle;

    pRobo->handZeroAttr( zeroTime, zeroAngle );
    ui->spinZeroTime->setValue( zeroTime );
    ui->spinZeroAngle->setValue( zeroAngle );

    double gapTime, gapAngle;
    pRobo->gapAttr( gapTime, gapAngle );
    ui->spinGapTime->setValue( gapTime );
    ui->spinGapAngle->setValue( gapAngle );

    ui->gpHand->setChecked( pRobo->handAble() );

    ui->chkHandZeroCcw->setChecked( pRobo->jointZeroCcwList().at(4) );

    ui->coordOption->setOption( (int)pRobo->getCoord() );
}

void SinanjuPref::initModel()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );
}

void SinanjuPref::spyEdited()
{
    QGroupBox *gpBox[]=
    {
        ui->gpHand,
    };
    QCheckBox *checkBoxes[]=
    {
        ui->chkInvBase,
        ui->chkInvBA,
        ui->chkInvSA,
        ui->chkInvWrist,

        ui->chkHandZeroCcw,
    };
    QRadioButton *radBoxes[] = {
    };
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

        ui->spinGapAngle,
        ui->spinGapTime,
    };

    QComboBox *comboxes[]={

    };

    install_spy();
}

void SinanjuPref::on_btnZeroArm_clicked()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    if ( !pBase->checkLink() )
    {
        sysPrompt( tr("Invalid link") );
        return ;
    }

    MegaZeroAffirmMessageBox msgBox;
    int ret = msgBox.exec();
    if ( ret == QMessageBox::Ok )
    {
        QList<int> jList;
        QList<bool> ccwList;

        jList<<0<<1<<2<<3;
        ccwList<<false<<false<<false<<false;

        pBase->goZero( tpvRegion(0, ui->widget->page() ),
                       jList,
                       ccwList );
    }
}

void SinanjuPref::on_btnZeroHand_clicked()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    if ( !pBase->checkLink() )
    {
        sysPrompt( tr("Invalid link") );
        return ;
    }

    MegaZeroAffirmMessageBox msgBox;
    int ret = msgBox.exec();
    if ( ret == QMessageBox::Ok )
    {
        pBase->goZero( tpvRegion(0, ui->widget->page() ),
                       4, ui->chkHandZeroCcw->isChecked() );
    }
}

void SinanjuPref::on_btnZero_clicked()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    if ( !pBase->checkLink() )
    {
        sysPrompt( tr("Invalid link") );
        return ;
    }

    MegaZeroAffirmMessageBox msgBox;
    int ret = msgBox.exec();
    if ( ret == QMessageBox::Ok )
    {
        pBase->goZero( tpvRegion(0, ui->widget->page() ) );
    }
}

void SinanjuPref::on_btnUploadZero_clicked()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    if ( !pBase->checkLink() )
    {
        sysPrompt( tr("Invalid link") );
        return ;
    }

    if ( pBase->checkZeroValid() )
    {}
    else
    {
        MegaZeroAffirmMessageBox::warning(this, tr("Error"), tr("Zero in device is invalid") );
        return;
    }

    //! get from device
    float zeros[4];
    for ( int i = 0; i < 4; i++ )
    {
        zeros[i] = pBase->getZero(i);
    }

    //! get mechanical version
    int ver;
    ver = pBase->getMechanicalVersion();
    if ( ver == 0x5A )
    {
//        ui->chkInvBase->setChecked(false);
        pBase->mAngleDir[0] = false;
        //! modify the base length
        pBase->mArmLengths[0] = 263.8;
    }
    else
    {
//        ui->chkInvBase->setChecked(true);
        pBase->mAngleDir[0] = true;
        pBase->mArmLengths[0] = 257;
    }

    //! update the ui
//    ui->spinAngleBase->setValue( zeros[0] );
//    ui->spinAngleBArm->setValue( zeros[1] );
//    ui->spinAngleSArm->setValue( zeros[2] );
//    ui->spinAngleWrist->setValue( zeros[3] );

    //! save the angle
    Q_ASSERT( pBase->mInitAngles.size() >= 4 );
    for ( int i = 0; i < 4; i++ )
    {
        pBase->mInitAngles[i] = zeros[i];
    }

    //! save the dir
    pBase->mAngleDir[0] = ui->chkInvBA->isChecked();

    //! \note widget is not roboprop
    emit sigSaveRequest( this );

    //! update the other page
    emit sigUpdated();

    emit sigModified( true );
}

void SinanjuPref::on_btnFactory_clicked()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    if ( !pBase->checkLink() )
    {
        sysPrompt( tr("Invalid link") );
        return ;
    }

    MegaZeroAffirmMessageBox msgBox;
    int ret = msgBox.exec();
    if ( ret == QMessageBox::Ok )
    {
        pBase->goFactory( tpvRegion(0, ui->widget->page()) );
    }
}

