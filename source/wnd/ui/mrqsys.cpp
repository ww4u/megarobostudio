#include "mrqsys.h"
#include "ui_mrqsys.h"
#include "../../com/comassist.h"

#define otp_temprature_unit (0.1f)
#define driver_current_unit  (0.1f)

mrqSys::mrqSys(QWidget *parent) :
    mrqView(parent),
    ui(new Ui::mrqSys)
{
    ui->setupUi(this);

    spyEdited();
}

mrqSys::~mrqSys()
{
    delete ui;
}

int mrqSys::setApply()
{
    return apply();
}

void mrqSys::modelChanged()
{
    adaptUi();

    updateUi();
}

void mrqSys::spyEdited()
{
    QCheckBox *checkBoxes[]=
    {
        ui->chkOtp,
    };

    QLineEdit *edits[]={
        ui->edtAlias,
    };

    QSpinBox *spinBoxes[]={
    };
    QDoubleSpinBox *doubleSpinBoxes[]={
        ui->spinHighT
    };

    QComboBox *comboxes[]={
        ui->cmbTResp,
    };

    install_spy();
}
void mrqSys::setupUi()
{}
void mrqSys::desetupUi()
{}


int mrqSys::apply()
{
    MegaDevice::deviceMRQ *pDevice;
    pDevice = getDevice();
    Q_ASSERT( NULL != pDevice );

    //! otp
    if ( m_pMrqModel->temperatures() > 0 )
    {
        pDevice->setOTP_STATE( (MRQ_CAN_NETMANAGELED)ui->chkOtp->isChecked() );
        pDevice->setOTP_THRESHOLD( comAssist::align(  ui->spinHighT->value(),otp_temprature_unit) );
        pDevice->setOTP_RESPONSE(
                    (MRQ_MOTIONPLAN_OOSLINERESPONSE_1)ui->cmbTResp->currentIndex()
                    );
    }

    //! driver
    if ( m_pMrqModel->driverId() == VRobot::motor_driver_820 )
    {
        pDevice->setNEWDRIVER_CURRENT( comAssist::align( ui->spin820Current->value(),driver_current_unit) );
        pDevice->setNEWDRIVER_MICROSTEPS( (MRQ_NEWDRIVER_MICROSTEPS)ui->cmb820MicroStep->value() );
    }

    //! name
    pDevice->setName(
                 ui->edtAlias->text()
                );
    return 0;

}

int mrqSys::updateUi()
{
    Q_ASSERT( NULL != m_pMrqModel );

    //! otp
    if ( m_pMrqModel->temperatures() > 0 )
    {
        ui->chkOtp->setChecked( (bool)m_pMrqModel->mOTP_STATE );
        ui->spinHighT->setValue( ( m_pMrqModel->mOTP_THRESHOLD)*otp_temprature_unit );
        ui->cmbTResp->setCurrentIndex( m_pMrqModel->mOTP_RESPONSE );
    }

    //! driver
    if ( m_pMrqModel->driverId() == VRobot::motor_driver_820 )
    {
        ui->spin820Current->setValue( m_pMrqModel->mNEWDRIVER_CURRENT * driver_current_unit );
        ui->cmb820MicroStep->setValue( m_pMrqModel->mNEWDRIVER_MICROSTEPS );
    }

    //! name
    ui->edtAlias->setText( m_pMrqModel->getName() );

    return 0;
}

void mrqSys::adaptUi()
{
    Q_ASSERT( NULL != m_pMrqModel );

    if ( m_pMrqModel->temperatures() > 0 )
    { ui->groupBox_2->setVisible( true ); }
    else
    { ui->groupBox_2->setVisible( false ); }

    if ( m_pMrqModel->driverId() == VRobot::motor_driver_820 )
    {
        ui->gp820->setVisible( true );
    }
    else
    {
        ui->gp820->setVisible( false );
    }

    if ( ui->cmb820MicroStep->count() > 0 )
    {}
    else
    {
        //! microstep
        //! fill the micro step
        QStringList microList;
        int base;

        m_pMrqModel->microStepAttr( microList, base );
        for ( int i = base; i < microList.size(); i++ )
        {
            ui->cmb820MicroStep->addItem( microList.at(i), i );
        }
    }
}

void mrqSys::on_edtAlias_textEdited(const QString &arg1)
{
    if ( arg1.length() < 1 )
    {
        sysPrompt( tr("Invalid name") );
        ui->edtAlias->setText( m_pMrqModel->getName() );
    }
}
