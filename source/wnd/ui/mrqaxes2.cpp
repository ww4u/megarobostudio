#include "mrqaxes2.h"
#include "ui_mrqaxes2.h"
#include "../com/comassist.h"

#define ACC_DEC_SCALE_UNIT  0.1f


mrqAxes2::mrqAxes2(QWidget *parent) :
    mrqView(parent),
    ui(new Ui::mrqaxes2)
{
    ui->setupUi(this);

    setupUi();

    spyEdited();
}

mrqAxes2::~mrqAxes2()
{
    delete ui;

    desetupUi();
}

int mrqAxes2::setApply()
{
    return apply();
}

void mrqAxes2::modelChanged()
{ updateUi(); }

void mrqAxes2::spyEdited()
{
    QCheckBox *checkBoxes[]=
    {
        ui->chkOutOfLineState,
    };
    QRadioButton *radBoxes[] = {
    };
    QLineEdit *edits[]={

    };

    QSpinBox *spinBoxes[]={
        ui->spinOutOfLine,
        ui->spinWarnPoint,
    };
    QDoubleSpinBox *doubleSpinBoxes[]={

        ui->spinStopDistance,
        ui->spinStopTime,

        ui->spinSAcc,
        ui->spinSDec,

        ui->spinCompP,
        ui->spinCompT,
    };

    QComboBox *comboxes[]={
        ui->cmbEndState,
        ui->cmbExecMode,

        ui->cmbTuneMode,
        ui->cmbPlanMode,
        ui->cmbTimeWidth,


        ui->cmbLineResponse,
        ui->cmbStopMode,

    };

    install_spy();
}

void mrqAxes2::setupUi()
{
    //! varialbes
    ui->cmbPlanMode->addItem( tr("Cubic"), ( (int)0 ) );
    ui->cmbPlanMode->addItem( tr("Trapezoid"), ( (int)1 ) );
    ui->cmbPlanMode->addItem( tr("S"), ( (int)2 ) );
}
void mrqAxes2::desetupUi()
{}

int mrqAxes2::apply()
{
    MegaDevice::deviceMRQ *pDevice;
    pDevice = getDevice();
    Q_ASSERT( NULL != pDevice );

    int ret;

    //! scale
    checked_call( pDevice->setAccScale( mAxesId,
                                           comAssist::align( ui->spinSAcc->value(),
                                                             ACC_DEC_SCALE_UNIT )
                                           ) );

    checked_call( pDevice->setDecScale( mAxesId,
                                           comAssist::align( ui->spinSDec->value(),
                                                             ACC_DEC_SCALE_UNIT )
                                           ) );
    //! plan
    MRQ_MOTION_SWITCH_1 nPage;
    for ( int iPage = 0; iPage < x_pages ; iPage++ )
    {
        nPage = (MRQ_MOTION_SWITCH_1)iPage;
        //! tune
        checked_call( pDevice->setMOTIONPLAN_EXECUTEMODE( mAxesId,
                                                         nPage,
                                               (MRQ_MOTIONPLAN_EXECUTEMODE_1)ui->cmbExecMode->currentIndex()
                                               ) );

        checked_call( pDevice->setMOTIONPLAN_MOTIONMODE( mAxesId,
                                                         nPage,
                                               (MRQ_MOTIONPLAN_MOTIONMODE_1)ui->cmbTuneMode->currentIndex()
                                               ) );
        checked_call( pDevice->setMOTIONPLAN_MODIFYDUTY( mAxesId,
                                                         nPage,
                                               (MRQ_MOTIONPLAN_MODIFYDUTY_1)ui->cmbTimeWidth->currentIndex()
                                               ) );
        checked_call( pDevice->setMOTIONPLAN_PLANMODE( mAxesId,
                                                       nPage,
                                               (MRQ_MOTIONPLAN_PLANMODE_1)ui->cmbPlanMode->value()
                                               ) );

        checked_call( pDevice->setMOTIONPLAN_WARNPOINT( mAxesId,
                                                       nPage,
                                               ui->spinWarnPoint->value()
                                               ) );

        checked_call( pDevice->setMOTIONPLAN_ENDSTATE( mAxesId,
                                                       nPage,
                                               (MRQ_MOTIONPLAN_ENDSTATE_1)ui->cmbEndState->currentIndex()
                                               ) );
        //! stop
        checked_call( pDevice->setMOTIONPLAN_STOPMODE( mAxesId,
                                                       nPage,
                                        (MRQ_MOTIONPLAN_STOPMODE_1)ui->cmbStopMode->currentIndex()
                                        ) );
        checked_call( pDevice->setMOTIONPLAN_STOPDISTANCE( mAxesId,
                                                           nPage,
                                        ui->spinStopDistance->value()
                                        ) );
        checked_call( pDevice->setMOTIONPLAN_STOPTIME( mAxesId,
                                                       nPage,
                                        ui->spinStopTime->value()
                                        ) );

        //! out of line
        checked_call( pDevice->setMOTIONPLAN_OOSLINESTATE( mAxesId,
                                                           nPage,
                                          (MRQ_SYSTEM_REVMOTION)ui->chkOutOfLineState->isChecked() ) );
        checked_call( pDevice->setMOTIONPLAN_OOSLINEOUTNUM( mAxesId,
                                                            nPage,
                                          ui->spinOutOfLine->value() ) );
        checked_call( pDevice->setMOTIONPLAN_OOSLINERESPONSE( mAxesId,
                                                              nPage,
                                            (MRQ_MOTIONPLAN_OOSLINERESPONSE_1)ui->cmbLineResponse->currentIndex() ) );
    }

    return ret;
}
int mrqAxes2::updateUi()
{
    Q_ASSERT( NULL != m_pMrqModel );

    //! motion
    ui->cmbExecMode->setCurrentIndex( m_pMrqModel->mMOTIONPLAN_EXECUTEMODE[mAxesId][0] );
    ui->cmbTuneMode->setCurrentIndex( m_pMrqModel->mMOTIONPLAN_MOTIONMODE[mAxesId][0] );on_cmbTuneMode_currentIndexChanged( m_pMrqModel->mMOTIONPLAN_MOTIONMODE[mAxesId][0] );
    ui->cmbTimeWidth->setCurrentIndex( ( m_pMrqModel->mMOTIONPLAN_MODIFYDUTY[mAxesId][0]));
    ui->cmbPlanMode->setValue( m_pMrqModel->mMOTIONPLAN_PLANMODE[mAxesId][0] );on_cmbPlanMode_currentIndexChanged( m_pMrqModel->mMOTIONPLAN_PLANMODE[mAxesId][0] );

    ui->spinWarnPoint->setValue( m_pMrqModel->mMOTIONPLAN_WARNPOINT[mAxesId][0] );
    ui->cmbEndState->setCurrentIndex( m_pMrqModel->mMOTIONPLAN_ENDSTATE[mAxesId][0] );

    //! acc
    ui->spinSAcc->setValue( m_pMrqModel->mAccList[mAxesId] * ACC_DEC_SCALE_UNIT );
    ui->spinSDec->setValue( m_pMrqModel->mDecList[mAxesId] * ACC_DEC_SCALE_UNIT );

    //! stop
    ui->cmbStopMode->setCurrentIndex( m_pMrqModel->mMOTIONPLAN_STOPMODE[mAxesId][0] );on_cmbStopMode_currentIndexChanged( m_pMrqModel->mMOTIONPLAN_STOPMODE[mAxesId][0] );
    ui->spinStopDistance->setValue( ( m_pMrqModel->mMOTIONPLAN_STOPDISTANCE[mAxesId][0]) );
    ui->spinStopTime->setValue( ( m_pMrqModel->mMOTIONPLAN_STOPTIME[mAxesId][0]) );

    //! out of line
    ui->chkOutOfLineState->setChecked( m_pMrqModel->mMOTIONPLAN_OOSLINESTATE[mAxesId][0] );
    ui->spinOutOfLine->setValue( m_pMrqModel->mMOTIONPLAN_OOSLINEOUTNUM[mAxesId][0] );
    ui->cmbLineResponse->setCurrentIndex( m_pMrqModel->mMOTIONPLAN_OOSLINERESPONSE[mAxesId][0] );

    return 0;
}

void mrqAxes2::on_cmbStopMode_currentIndexChanged(int index)
{
    bool bDelayVisible;
    if ( index == 0 )
    { bDelayVisible = false; }
    else
    { bDelayVisible = true; }

    ui->label_2->setVisible( bDelayVisible );
    ui->label_3->setVisible( bDelayVisible );
    ui->spinStopDistance->setVisible( false );
    ui->spinStopTime->setVisible( false );
}

void mrqAxes2::on_cmbTuneMode_currentIndexChanged(int index)
{
    bool bDutyVisible;
    bDutyVisible = (index != 0 );

    ui->label_23->setVisible( bDutyVisible );
    ui->cmbTimeWidth->setVisible( bDutyVisible );
}

void mrqAxes2::on_cmbPlanMode_currentIndexChanged(int index)
{
    if(index == 0 )
    { ui->gpSAcc->setVisible( false ); }
    else
    { ui->gpSAcc->setVisible( true ); }
}

void mrqAxes2::on_spinSAcc_valueChanged(double arg1)
{
    if ( ui->spinSDec->value() > 100-arg1 )
    { ui->spinSDec->setValue( 100 - arg1); }
}

void mrqAxes2::on_spinSDec_valueChanged(double arg1)
{
    if ( ui->spinSAcc->value() > 100-arg1 )
    { ui->spinSAcc->setValue( 100 - arg1); }
}
