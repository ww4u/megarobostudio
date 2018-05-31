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
    mCcwChecks.append( ui->chkPlateCcw );
    mCcwChecks.append( ui->chkHandCcw );

    mBodyChecks.append( ui->chkLSBody );
    mBodyChecks.append( ui->chkRSBody );
    mBodyChecks.append( ui->chkPlateBody );
    mBodyChecks.append( ui->chkHandBody );

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

void DeltaPref::updateScreen()
{
    updateUi();
}

void DeltaPref::spyEdited()
{
    QCheckBox *checkBoxes[]=
    {
    };

    QLineEdit *edits[]={

    };

    QSpinBox *spinBoxes[]={

    };

    QDoubleSpinBox *doubleSpinBoxes[]={
        ui->spinZeroTime,
        ui->spinZeroAngle,

//        ui->spinAngleRS,
//        ui->spinAngleLS,

        ui->spinInitT,
        ui->spinInitL,
        ui->spinInitR,
        ui->spinInitY,
        ui->spinInitH,
    };

    QComboBox *comboxes[]={

    };

    install_spy();
}

void DeltaPref::updateData()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    robotDelta *pRobo = (robotDelta*)pBase;
    Q_ASSERT( NULL != pRobo );
    pRobo->setZeroAttr( ui->spinZeroTime->value(),
                        ui->spinZeroAngle->value() );

    //! init
    pRobo->setInitAttr( ui->spinInitT->value(),
                        ui->spinInitL->value(),
                        ui->spinInitR->value(),
                        ui->spinInitY->value(),
                        ui->spinInitH->value() );
}

void DeltaPref::updateUi()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    robotDelta *pRobo = (robotDelta*)pBase;
    Q_ASSERT( NULL != pRobo );

    //! zero
    double time, angle;
    pRobo->zeroAttr( time, angle );

    ui->spinZeroTime->setValue( time );
    ui->spinZeroAngle->setValue( angle );


    //! init
    double initT, initL, initR, initY, initH;
    pRobo->initAttr( initT, initL, initR, initY, initH );
    ui->spinInitT->setValue( initT );

    ui->spinInitL->setValue( initL );
    ui->spinInitR->setValue( initR );
    ui->spinInitY->setValue( initY );
    ui->spinInitH->setValue( initH );

    //! zero ccw
    QList<bool> zeroCcw = pRobo->jointZeroCcwList();
    for ( int i = 0; i < mCcwChecks.size(); i++ )
    {
        mCcwChecks.at(i)->setChecked( zeroCcw.at(i) );
    }
    slot_ccw_changed();
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
    MegaZeroAffirmMessageBox msgBox;
    int ret = msgBox.exec();
    if ( ret == QMessageBox::Ok )
    { zeroJoint( jId, bccw ); }
}

void DeltaPref::slot_ccw_changed()
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

    //! all
    if ( checkCount == mBodyChecks.size() )
    { ui->chkAllBody->setChecked(true); }
    else
    { ui->chkAllBody->setChecked(false); }
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
    sig_joint( 3, chkHandCcw )
}

void DeltaPref::on_btnZeroPlate_clicked()
{
    sig_joint( 2, chkPlateCcw )
}

void DeltaPref::on_btnZeroBody_clicked()
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

    pBase->goZero( jointList, ccwList );
}

void DeltaPref::on_chkAllCcw_clicked(bool checked)
{
    foreach( QCheckBox *pCheck, mCcwChecks )
    {
        pCheck->setChecked( checked );
    }
}

void DeltaPref::on_chkAllBody_clicked(bool checked)
{
    foreach( QCheckBox *pCheck, mBodyChecks )
    {
        pCheck->setChecked( checked );
    }
}
