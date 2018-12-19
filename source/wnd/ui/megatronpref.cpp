#include "megatronpref.h"
#include "ui_megatronpref.h"

#include "../../widget/megamessagebox.h"

MegatronPref::MegatronPref(QWidget *parent) :
    modelView(parent),
    ui(new Ui::megatronpref)
{
    ui->setupUi(this);

    //! checks
    mBodyChecks.append( ui->chkFxBody );
    mBodyChecks.append( ui->chkFzBody );
    mBodyChecks.append( ui->chkBxBody );
    mBodyChecks.append( ui->chkBzBody );
    mBodyChecks.append( ui->chkLyBody );
    mBodyChecks.append( ui->chkRyBody );

    mCcwChecks.append( ui->chkFxCcw );
    mCcwChecks.append( ui->chkFzCcw );
    mCcwChecks.append( ui->chkBxCcw );
    mCcwChecks.append( ui->chkBzCcw );
    mCcwChecks.append( ui->chkLyCcw );
    mCcwChecks.append( ui->chkRyCcw );

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

MegatronPref::~MegatronPref()
{
    delete ui;
}

void MegatronPref::changeEvent(QEvent * event)
{
    modelView::changeEvent( event );

    if (event->type() == QEvent::LanguageChange)
    { ui->retranslateUi( this ); }
}

void MegatronPref::setModelObj( mcModelObj *pObj )
{
    modelView::setModelObj( pObj );

    updateUi();
}

int MegatronPref::setApply()
{
    updateData();

    return 0;
}
void MegatronPref::updateScreen()
{
    updateUi();
}
void MegatronPref::spyEdited()
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
    };

    QComboBox *comboxes[]={

    };

    install_spy();
}

void MegatronPref::updateData()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    robotMegatron *pRobo = (robotMegatron*)pBase;
    Q_ASSERT( NULL != pRobo );
    pRobo->setZeroAttr( ui->spinZeroTime->value(),
                        ui->spinZeroAngle->value() );

    pRobo->setGapAttr( ui->spinGapTime->value(),
                       ui->spinGapDistance->value() );
}

void MegatronPref::updateUi()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    robotMegatron *pRobo = (robotMegatron*)pBase;
    Q_ASSERT( NULL != pRobo );

    double time, angle;
    pRobo->zeroAttr( time, angle );

    ui->spinZeroTime->setValue( time );
    ui->spinZeroAngle->setValue( angle );

    pRobo->gapAttr( time, angle );
    ui->spinGapTime->setValue( time );
    ui->spinGapDistance->setValue( angle );

    //! zero ccw
    QList<bool> zeroCcw = pRobo->jointZeroCcwList();
    for ( int i = 0; i < mCcwChecks.size(); i++ )
    { mCcwChecks.at(i)->setChecked( zeroCcw.at(i) ); }

    slot_ccw_changed();
}

void MegatronPref::zeroJoint( int jointId, bool bCcw )
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    pBase->goZero( tpvRegion(0,ui->widget->page()), jointId, bCcw );
}

void MegatronPref::slot_joint_zero( int jId, bool bccw )
{
    MegaZeroAffirmMessageBox msgBox;
    int ret = msgBox.exec();
    if ( ret == QMessageBox::Ok )
    { zeroJoint( jId, bccw ); }
}

void MegatronPref::slot_ccw_changed()
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

void MegatronPref::slot_body_changed()
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
void MegatronPref::on_btnZeroFx_clicked()
{ sig_joint(0, chkFxCcw ); }

void MegatronPref::on_btnZeroFz_clicked()
{ sig_joint(1, chkFzCcw ); }

void MegatronPref::on_btnZeroBx_clicked()
{ sig_joint(2, chkBxCcw ); }

void MegatronPref::on_btnZeroBz_clicked()
{ sig_joint(3, chkBzCcw ); }

void MegatronPref::on_btnZeroLy_clicked()
{ sig_joint(4, chkLyCcw ); }

void MegatronPref::on_btnZeroRy_clicked()
{ sig_joint(5, chkRyCcw ); }

void MegatronPref::on_btnZeroBody_clicked()
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

    pBase->goZero( tpvRegion(0,ui->widget->page()), jointList, ccwList );
}

void MegatronPref::on_chkAllCcw_clicked(bool checked)
{
    foreach( QCheckBox *pCheck, mCcwChecks )
    {
        pCheck->setChecked( checked );
    }
}

void MegatronPref::on_chkAllBody_clicked(bool checked)
{
    foreach( QCheckBox *pCheck, mBodyChecks )
    {
        pCheck->setChecked( checked );
    }
}
