#include "h2config.h"
#include "ui_h2config.h"

H2Config::H2Config(QWidget *parent) :
    modelView(parent),
    ui(new Ui::H2Config)
{
    ui->setupUi(this);
}

H2Config::~H2Config()
{
    delete ui;
}

void H2Config::setModelObj( mcModelObj *pObj )
{
    modelView::setModelObj( pObj );

    updateUi();
}

int H2Config::setApply()
{
    updateData();

    return 0;
}
void H2Config::updateScreen()
{
    updateUi();
}
void H2Config::spyEdited()
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
        ui->spinArm0,
        ui->spinArm1,
        ui->spinArm2,
        ui->spinArm3,
        ui->spinArm4,
        ui->spinArm5,
    };

    QComboBox *comboxes[]={

    };

    install_spy();
}

void H2Config::updateData()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    robotH2 *pRobo = (robotH2*)pBase;
    Q_ASSERT( NULL != pRobo );

    pRobo->mArmLengths[0] = ui->spinArm0->value();
    pRobo->mArmLengths[1] = ui->spinArm1->value();
    pRobo->mArmLengths[2] = ui->spinArm2->value();
    pRobo->mArmLengths[3] = ui->spinArm3->value();

    pRobo->mArmLengths[4] = ui->spinArm4->value();
    pRobo->mArmLengths[5] = ui->spinArm5->value();

    //! transfer && shift
    double t[2*2], s[2*1], invt[2*2];
//    bool b;
//    b = ui->gpTransfer->isChecked();
    t[0*2+0] = ui->spinR00->value();
    t[0*2+1] = ui->spinR01->value();
    t[1*2+0] = ui->spinR10->value();
    t[1*2+1] = ui->spinR11->value();

    s[0*1+0] = ui->spinS00->value();
    s[1*1+0] = ui->spinS10->value();

    invt[0*2+0] = ui->spinR00_2->value();
    invt[0*2+1] = ui->spinR01_2->value();
    invt[1*2+0] = ui->spinR10_2->value();
    invt[1*2+1] = ui->spinR11_2->value();

    pRobo->setTransfer( t, s, invt );
}

void H2Config::updateUi()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    robotH2 *pRobo = (robotH2*)pBase;
    Q_ASSERT( NULL != pRobo );

    ui->spinArm0->setValue( pRobo->mArmLengths[0] );
    ui->spinArm1->setValue( pRobo->mArmLengths[1] );
    ui->spinArm2->setValue( pRobo->mArmLengths[2] );
    ui->spinArm3->setValue( pRobo->mArmLengths[3] );

    ui->spinArm4->setValue( pRobo->mArmLengths[4] );
    ui->spinArm5->setValue( pRobo->mArmLengths[5] );

    //! transfer && shift
    double t[2*2], s[2*1], invt[2*2];
//    bool b;
    pRobo->transfer( /*b,*/ t, s, invt );
//    ui->gpTransfer->setChecked( b );
    ui->spinR00->setValue( t[0*2+0] );
    ui->spinR01->setValue( t[0*2+1] );
    ui->spinR10->setValue( t[1*2+0] );
    ui->spinR11->setValue( t[1*2+1] );

    ui->spinS00->setValue( s[0*1+0]);
    ui->spinS10->setValue( s[1*1+0]);

    ui->spinR00_2->setValue( invt[0*2+0] );
    ui->spinR01_2->setValue( invt[0*2+1] );
    ui->spinR10_2->setValue( invt[1*2+0] );
    ui->spinR11_2->setValue( invt[1*2+1] );
}
