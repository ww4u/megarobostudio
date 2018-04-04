#include "deltapref.h"
#include "ui_deltapref.h"

#include "../widget/megamessagebox.h"

DeltaPref::DeltaPref(QWidget *parent) :
    modelView(parent),
    ui(new Ui::deltapref)
{
    ui->setupUi(this);

    mCcwChecks.append( ui->chkLSCcw );
    mCcwChecks.append( ui->chkRSCcw );
    mCcwChecks.append( ui->chkHandCcw );
    mCcwChecks.append( ui->chkPlateCcw );

    mBodyChecks.append( ui->chkLSBody );
    mBodyChecks.append( ui->chkRSBody );
    mBodyChecks.append( ui->chkHandBody );
    mBodyChecks.append( ui->chkPlateBody );

    foreach( QCheckBox *pCheck, mBodyChecks )
    {
        connect( pCheck, SIGNAL(clicked(bool)),
                 this, SLOT(slot_body_changed()) );
    }

    connect( this, SIGNAL(signal_joint_zero(int,bool)),
             this, SLOT(slot_joint_zero(int,bool)) );


    //! post change
    slot_body_changed();
}

DeltaPref::~DeltaPref()
{
    delete ui;
}

void DeltaPref::setModelObj( mcModelObj *pObj )
{
    modelView::setModelObj( pObj );

    updateUi();
}

int DeltaPref::setApply()
{
    updateData();

    return 0;
}

void DeltaPref::updateData()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    robotDelta *pRobo = (robotDelta*)pBase;
    Q_ASSERT( NULL != pRobo );
    pRobo->setZeroAttr( ui->spinZeroTime->value(),
                        ui->spinZeroAngle->value(),
                        ui->spinZeroSpeed->value() );

    //! init
    pRobo->mInitAngles[0] = ui->spinAngleLS->value();
    pRobo->mInitAngles[1] = ui->spinAngleRS->value();
}

void DeltaPref::updateUi()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    robotDelta *pRobo = (robotDelta*)pBase;
    Q_ASSERT( NULL != pRobo );

    double time, angle, speed;
    pRobo->zeroAttr( time, angle, speed );

    ui->spinZeroTime->setValue( time );
    ui->spinZeroAngle->setValue( angle );
    ui->spinZeroSpeed->setValue( speed );

    //! init angle
    Q_ASSERT( pRobo->mInitAngles.size() >= 2 );
    ui->spinAngleLS->setValue( pRobo->mInitAngles[0] );
    ui->spinAngleRS->setValue( pRobo->mInitAngles[1] );
}

void DeltaPref::zeroJoint( int jointId, bool bCcw )
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    pBase->goZero( jointId, bCcw );
}

void DeltaPref::slot_joint_zero( int jId, bool bccw )
{
    MegaMessageBox msgBox( tr("Sure to Zero?") );
    int ret = msgBox.exec();
    if ( ret == QMessageBox::Ok )
    { zeroJoint( jId, bccw ); }
}

void DeltaPref::slot_body_changed()
{
    int checkCount = 0;

    foreach( QCheckBox *pCheck, mBodyChecks )
    {
        if ( pCheck->isChecked() )
        { checkCount++; }
        else
        {}
    }

    if ( checkCount > 0 )
    { ui->btnZeroBody->setEnabled(true);}
    else
    { ui->btnZeroBody->setEnabled(false);}
}

#define sig_joint( id, chk )    emit signal_joint_zero( id, ui->chk->isChecked() );

void DeltaPref::on_btnZeroLS_clicked()
{
    sig_joint( 0, chkLSCcw )
}

void DeltaPref::on_btnZeroRS_clicked()
{
    sig_joint( 1, chkRSCcw )
}

void DeltaPref::on_btnZeroHand_clicked()
{
    sig_joint( 2, chkHandCcw )
}

void DeltaPref::on_btnZeroPlate_clicked()
{
    sig_joint( 3, chkPlateCcw )
}

void DeltaPref::on_btnZeroBody_clicked()
{
    MegaMessageBox msgBox( tr("Sure to Zero?") );
    int ret = msgBox.exec();
    if ( ret == QMessageBox::Ok )
    {}
    else
    { return; }

    QList< int > jointList;
    QList< bool > ccwList;

    Q_ASSERT( mBodyChecks.size() == mCcwChecks.size() );

    for( int i = 0; i < mBodyChecks.size(); i++ )
    {
        if ( mBodyChecks.at(i)->isChecked() )
        {
            jointList.append(i);
            ccwList.append( mCcwChecks.at(i)->isChecked() );
        }
    }

    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    pBase->goZero( jointList, ccwList );
}
