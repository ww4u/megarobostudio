#include "motionpref.h"
#include "ui_motionpref.h"

MotionPref::MotionPref(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MotionPref)
{
    ui->setupUi(this);

    m_pModel = NULL;
}

MotionPref::~MotionPref()
{
    delete ui;
}

void MotionPref::changeEvent(QEvent * event)
{
    QDialog::changeEvent( event );

    if (event->type() == QEvent::LanguageChange)
    { ui->retranslateUi( this ); }
}

void MotionPref::setModel( MotionModel *pModel )
{
    Q_ASSERT( NULL != pModel );

    m_pModel = pModel;

    updateUi();
}
MotionModel *MotionPref::model()
{ return m_pModel; }

#define update_spin_ui( member, spin )          m_pModel->member = ui->spin->value();
#define update_spin_data( member, spin )        ui->spin->setValue( m_pModel->member );
void MotionPref::updateData()
{
    Q_ASSERT( NULL != m_pModel );

    update_spin_data( mInitBase, spinAngleBase );
    update_spin_data( mInitBigArm, spinAngleBArm );
    update_spin_data( mInitSmallArm, spinAngleSArm );
    update_spin_data( mInitWrist, spinAngleWrist );

    update_spin_data( mRotBase, spinRotBase );
    update_spin_data( mRotBigArm, spinRotBigArm );
    update_spin_data( mRotSmallArm, spinRotSmallArm );
    update_spin_data( mRotWrist, spinRotWrist );
}
void MotionPref::updateUi()
{
    Q_ASSERT( NULL != m_pModel );

    update_spin_ui( mInitBase, spinAngleBase );
    update_spin_ui( mInitBigArm, spinAngleBArm );
    update_spin_ui( mInitSmallArm, spinAngleSArm );
    update_spin_ui( mInitWrist, spinAngleWrist );

    update_spin_ui( mRotBase, spinRotBase );
    update_spin_ui( mRotBigArm, spinRotBigArm );
    update_spin_ui( mRotSmallArm, spinRotSmallArm );
    update_spin_ui( mRotWrist, spinRotWrist );
}

void MotionPref::on_MotionPref_accepted()
{
    Q_ASSERT( NULL != m_pModel );

    updateData();
}
