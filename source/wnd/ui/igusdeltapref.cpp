#include "igusdeltapref.h"
#include "ui_igusdrylinpref.h"

#include "../widget/megamessagebox.h"

IgusDeltaPref::IgusDeltaPref(QWidget *parent) :
    modelView(parent),
    ui(new Ui::igusdrylinpref)
{
    ui->setupUi(this);

    mCcwChecks.append( ui->chkLSCcw );
    mCcwChecks.append( ui->chkRSCcw );
    mCcwChecks.append( ui->chkPlateCcw );
//    mCcwChecks.append( ui->chkHandCcw );

    mBodyChecks.append( ui->chkLSBody );
    mBodyChecks.append( ui->chkRSBody );
    mBodyChecks.append( ui->chkPlateBody );
//    mBodyChecks.append( ui->chkHandBody );

    foreach( QCheckBox *pCheck, mBodyChecks )
    {
        connect( pCheck, SIGNAL(clicked(bool)),
                 this, SLOT(slot_body_changed()) );
    }
    connect( ui->chkAllBody, SIGNAL(clicked(bool)),
             this, SLOT(slot_body_changed()) );

    foreach( QCheckBox *pCheck, mCcwChecks )
    {
        connect( pCheck, SIGNAL(clicked(bool)),
                 this, SLOT(slot_ccw_changed()) );
    }

    connect( this, SIGNAL(signal_joint_zero(int,bool)),
             this, SLOT(slot_joint_zero(int,bool)) );

    //! post change
    slot_body_changed();
    slot_ccw_changed();

    spyEdited();
}

IgusDeltaPref::~IgusDeltaPref()
{
    delete ui;
}

void IgusDeltaPref::setModelObj( mcModelObj *pObj )
{
    modelView::setModelObj( pObj );

    updateUi();
}

int IgusDeltaPref::setApply()
{
    updateData();

    return 0;
}

void IgusDeltaPref::updateScreen()
{
    updateUi();
}

void IgusDeltaPref::spyEdited()
{
    QCheckBox *checkBoxes[]=
    {
    };

    QRadioButton *radBoxes[] = {
    };

    QLineEdit *edits[]={

    };

    QSpinBox *spinBoxes[]={

    };

    QDoubleSpinBox *doubleSpinBoxes[]={
        ui->spinZeroTime,
        ui->spinZeroAngle,

        ui->spinInitT,
        ui->spinInitLeg,
//        ui->spinInitH,
    };

    QComboBox *comboxes[]={
        ui->cmbLimit
    };

    install_spy();
}

void IgusDeltaPref::updateData()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    robotIgusDelta *pRobo = (robotIgusDelta*)pBase;
    Q_ASSERT( NULL != pRobo );
    pRobo->setZeroAttr( ui->spinZeroTime->value(),
                        ui->spinZeroAngle->value(),
                        ui->cmbLimit->currentIndex() );

    //! init
    pRobo->setInitAttr( ui->spinInitT->value(),
                        ui->spinInitLeg->value(),
                        0 );

    pRobo->setJointZeroCcw( 0, ui->chkLSCcw->isChecked() );
    pRobo->setJointZeroCcw( 1, ui->chkRSCcw->isChecked() );
    pRobo->setJointZeroCcw( 0, ui->chkPlateCcw->isChecked() );
}

void IgusDeltaPref::updateUi()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    robotIgusDelta *pRobo = (robotIgusDelta*)pBase;
    Q_ASSERT( NULL != pRobo );

    //! zero
    double time, angle;
    int limitOpt;
    pRobo->zeroAttr( time, angle, limitOpt );

    ui->spinZeroTime->setValue( time );
    ui->spinZeroAngle->setValue( angle );
    ui->cmbLimit->setCurrentIndex( limitOpt );

    //! init
    double initT, initLeg, initH;
    pRobo->initAttr( initT, initLeg, initH );
    ui->spinInitT->setValue( initT );

    ui->spinInitLeg->setValue( initLeg );
//    ui->spinInitH->setValue( initH );

    //! zero ccw
    QList<bool> zeroCcw = pRobo->jointZeroCcwList();
    for ( int i = 0; i < mCcwChecks.size(); i++ )
    {
        mCcwChecks.at(i)->setChecked( zeroCcw.at(i) );
    }
    slot_ccw_changed();
}

void IgusDeltaPref::zeroJoint( int jointId, bool bCcw )
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    pBase->goZero( tpvRegion(0, ui->widget->page() ), jointId, bCcw );
}

void IgusDeltaPref::slot_joint_zero( int jId, bool bccw )
{
    MegaZeroAffirmMessageBox msgBox;
    int ret = msgBox.exec();
    if ( ret == QMessageBox::Ok )
    { zeroJoint( jId, bccw ); }
}

void IgusDeltaPref::slot_ccw_changed()
{
    //! update the all
    foreach( QCheckBox *pCheck, mCcwChecks )
    {
        if ( pCheck->isChecked() )
        {}
        else
        {
            ui->chkAllCcw->setChecked( false );
            return;
        }
    }

    ui->chkAllCcw->setChecked( true );
}

void IgusDeltaPref::slot_body_changed()
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

    //! all
    if ( checkCount == mBodyChecks.size() )
    { ui->chkAllBody->setChecked(true); }
    else
    { ui->chkAllBody->setChecked(false); }
}

#define sig_joint( id, chk )    emit signal_joint_zero( id, ui->chk->isChecked() );

void IgusDeltaPref::on_btnZeroLS_clicked()
{
    sig_joint( 0, chkLSCcw )
}

void IgusDeltaPref::on_btnZeroRS_clicked()
{
    sig_joint( 1, chkRSCcw )
}

//void IgusDeltaPref::on_btnZeroHand_clicked()
//{
//    sig_joint( 3, chkHandCcw )
//}

void IgusDeltaPref::on_btnZeroPlate_clicked()
{
    sig_joint( 2, chkPlateCcw )
}

void IgusDeltaPref::on_btnZeroBody_clicked()
{
    MegaZeroAffirmMessageBox msgBox;
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

    pBase->goZero( tpvRegion(0, ui->widget->page() ), jointList, ccwList );
}

void IgusDeltaPref::on_chkAllCcw_clicked(bool checked)
{
    foreach( QCheckBox *pCheck, mCcwChecks )
    {
        pCheck->setChecked( checked );
    }
}

void IgusDeltaPref::on_chkAllBody_clicked(bool checked)
{
    foreach( QCheckBox *pCheck, mBodyChecks )
    {
        pCheck->setChecked( checked );
    }
}
