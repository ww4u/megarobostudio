#include "h2pref.h"
#include "ui_h2pref.h"

#include "megamessagebox.h"

H2Pref::H2Pref(QWidget *parent) :
    modelView(parent),
    ui(new Ui::H2Pref)
{
    ui->setupUi(this);

    //! checks
    mBodyChecks.append( ui->chkXBody );
    mBodyChecks.append( ui->chkYBody );

    mCcwChecks.append( ui->chkXCcw );
    mCcwChecks.append( ui->chkYCcw );

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

H2Pref::~H2Pref()
{
    delete ui;
}

void H2Pref::setModelObj( mcModelObj *pObj )
{
    modelView::setModelObj( pObj );

    updateUi();
}

int H2Pref::setApply()
{
    updateData();

    return 0;
}
void H2Pref::updateScreen()
{
    updateUi();
}
void H2Pref::spyEdited()
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
        ui->spinZeroSpeed,
    };

    QComboBox *comboxes[]={

    };

    install_spy();
}

void H2Pref::updateData()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    robotH2 *pRobo = (robotH2*)pBase;
    Q_ASSERT( NULL != pRobo );
    pRobo->setZeroAttr( ui->spinZeroTime->value(),
                        ui->spinZeroAngle->value(),
                        ui->spinZeroSpeed->value() );
}

void H2Pref::updateUi()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    robotH2 *pRobo = (robotH2*)pBase;
    Q_ASSERT( NULL != pRobo );

    double time, angle, speed;
    pRobo->zeroAttr( time, angle, speed );

    ui->spinZeroTime->setValue( time );
    ui->spinZeroAngle->setValue( angle );
    ui->spinZeroSpeed->setValue( speed );
}

void H2Pref::zeroJoint( int jointId, bool bCcw )
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    pBase->goZero( jointId, bCcw );
}

void H2Pref::slot_joint_zero( int jId, bool bccw )
{
    MegaZeroAffirmMessageBox msgBox;
    int ret = msgBox.exec();
    if ( ret == QMessageBox::Ok )
    { zeroJoint( jId, bccw ); }
}

void H2Pref::slot_ccw_changed()
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

void H2Pref::slot_body_changed()
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

void H2Pref::on_chkAllCcw_clicked(bool checked)
{
    foreach( QCheckBox *pCheck, mCcwChecks )
    {
        pCheck->setChecked( checked );
    }
}

void H2Pref::on_chkAllBody_clicked(bool checked)
{
    foreach( QCheckBox *pCheck, mBodyChecks )
    {
        pCheck->setChecked( checked );
    }
}
#define sig_joint( id, chk )    emit signal_joint_zero( id, ui->chk->isChecked() );
void H2Pref::on_btnZeroX_clicked()
{
    sig_joint(0, chkXCcw );
}

void H2Pref::on_btnZeroY_clicked()
{
    sig_joint(0, chkYCcw );
}

void H2Pref::on_btnZeroBody_clicked()
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
