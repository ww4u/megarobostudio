#include "sinanjuconfig.h"
#include "ui_sinanjuconfig.h"


#define set_rot_r( r, c )   mRots[ r * 3 + c ] = ui->spinR##r##c;
#define set_rot_ir( r, c )   mRotInvs[ r * 3 + c ] = ui->spinR##r##c##_2;
#define set_rot_s( r, c )   mShifts[ r * 1 + c ] = ui->spinS##r##c;
SinanjuConfig::SinanjuConfig(QWidget *parent) :
    modelView(parent),
    ui(new Ui::SinanjuConfig)
{
    ui->setupUi(this);

    spyEdited();

    //! rot
    set_rot_r( 0, 0 );
    set_rot_r( 0, 1 );
    set_rot_r( 0, 2 );

    set_rot_r( 1, 0 );
    set_rot_r( 1, 1 );
    set_rot_r( 1, 2 );

    set_rot_r( 2, 0 );
    set_rot_r( 2, 1 );
    set_rot_r( 2, 2 );

    //! shifts
    set_rot_s( 0, 0 );
    set_rot_s( 1, 0 );
    set_rot_s( 2, 0 );

    //! inv
    set_rot_ir( 0, 0 );
    set_rot_ir( 0, 1 );
    set_rot_ir( 0, 2 );

    set_rot_ir( 1, 0 );
    set_rot_ir( 1, 1 );
    set_rot_ir( 1, 2 );

    set_rot_ir( 2, 0 );
    set_rot_ir( 2, 1 );
    set_rot_ir( 2, 2 );
}

SinanjuConfig::~SinanjuConfig()
{
    delete ui;
}

void SinanjuConfig::setModelObj( mcModelObj *pObj )
{
    modelView::setModelObj(pObj);

    updateUi();
}

int SinanjuConfig::setApply()
{
    updateData();

    return 0;
}

void SinanjuConfig::updateScreen()
{
    updateUi();
}

void SinanjuConfig::spyEdited()
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
        ui->spinBaseLen,
        ui->spinBigLen,
        ui->spinLittleLen,

        ui->spinX,
        ui->spinY,
        ui->spinZ,
    };

    QComboBox *comboxes[]={

    };

    install_spy();
}

void SinanjuConfig::updateData()
{
    Q_ASSERT( m_pModelObj != NULL );
    robotSinanju *pRobo = ( robotSinanju *)m_pModelObj;
    Q_ASSERT( NULL != pRobo );

    pRobo->mArmLengths[0] = ui->spinBaseLen->value();
    pRobo->mArmLengths[1] = ui->spinBigLen->value();
    pRobo->mArmLengths[2] = ui->spinLittleLen->value();

    pRobo->mInitPos[0] = ui->spinX->value();
    pRobo->mInitPos[1] = ui->spinY->value();
    pRobo->mInitPos[2] = ui->spinZ->value();

    pRobo->setJointFactoryAngle( 0, ui->angleFactBase->value() );
    pRobo->setJointFactoryAngle( 1, ui->angleFactBa->value() );
    pRobo->setJointFactoryAngle( 2, ui->angleFactLa->value() );
    pRobo->setJointFactoryAngle( 3, ui->angleFactWr->value() );

    //! set data
    double rot[ sizeof_array(mRots) ], rotInv[ sizeof_array(mRots) ], shift[ sizeof_array(mShifts) ];
    //! rot
    for ( int i = 0; i < sizeof_array( rot ); i++ )
    {
        Q_ASSERT( i < sizeof_array(mRots) );
        rot[i] = mRots[i]->value();
    }
    //! shift
    for ( int i = 0; i < sizeof_array( shift ); i++ )
    {
        Q_ASSERT( i < sizeof_array(mShifts) );
        shift[i] = mShifts[i]->value();
    }

    //! rotInv
    for ( int i = 0; i < sizeof_array( rotInv ); i++ )
    {
        Q_ASSERT( i < sizeof_array(mRotInvs) );
        rotInv[i] = mRotInvs[i]->value();
    }

    pRobo->setTransfer( ui->gpTransfer->isChecked(),
                        rot,
                        shift,
                        rotInv );
}
void SinanjuConfig::updateUi()
{
    Q_ASSERT( m_pModelObj != NULL );
    robotSinanju *pRobo = ( robotSinanju *)m_pModelObj;
    Q_ASSERT( NULL != pRobo );

    Q_ASSERT( pRobo->mArmLengths.size() >= 3 );

    ui->spinBaseLen->setValue( pRobo->mArmLengths.at(0) );
    ui->spinBigLen->setValue( pRobo->mArmLengths.at(1) );
    ui->spinLittleLen->setValue( pRobo->mArmLengths.at(2) );

    ui->spinX->setValue( pRobo->mInitPos.at(0) );
    ui->spinY->setValue( pRobo->mInitPos.at(1) );
    ui->spinZ->setValue( pRobo->mInitPos.at(2) );

    ui->angleFactBase->setValue( pRobo->jointFactoryAngle(0) );
    ui->angleFactBa->setValue( pRobo->jointFactoryAngle(1) );
    ui->angleFactLa->setValue( pRobo->jointFactoryAngle(2) );
    ui->angleFactWr->setValue( pRobo->jointFactoryAngle(3) );

    //! transfer
    bool bEn;
    double rot[ sizeof_array(mRots) ], rotInv[ sizeof_array(mRotInvs) ], shift[ sizeof_array(mShifts) ];
    pRobo->transfer( bEn, rot, shift, rotInv );

    ui->gpTransfer->setChecked( bEn );
    //! rot
    for ( int i = 0; i < sizeof_array( rot ); i++ )
    {
        Q_ASSERT( i < sizeof_array(mRots) );
        mRots[i]->setValue( rot[i]);
    }
    //! shift
    for ( int i = 0; i < sizeof_array( shift ); i++ )
    {
        Q_ASSERT( i < sizeof_array(mShifts) );
        mShifts[i]->setValue( shift[i]);
    }
    //! rotInv
    for ( int i = 0; i < sizeof_array( rot ); i++ )
    {
        Q_ASSERT( i < sizeof_array(mRotInvs) );
        mRotInvs[i]->setValue( rotInv[i]);
    }
}
