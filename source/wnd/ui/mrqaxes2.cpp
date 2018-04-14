#include "mrqaxes2.h"
#include "ui_mrqaxes2.h"

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

void mrqAxes2::on_btnInitZero_clicked()
{
//    m_pMrqModel->goInit( mAxesId,
//                         (MRQ_MOTION_INITPOSITIONUNIT)ui->cmbInitUnit->currentIndex(),
//                         (MRQ_MOTION_INITIOSIGNAL)ui->cmbInitIO->currentIndex(),
//                         ui->cmbInitPos->currentText().toFloat()
//                         );
    logDbg();
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

    QLineEdit *edits[]={

    };

    QSpinBox *spinBoxes[]={
        ui->spinOutOfLine,
    };
    QDoubleSpinBox *doubleSpinBoxes[]={
        ui->spinFeedback,
        ui->spinInitPosition,
        ui->spinStopDistance,
        ui->spinStopTime,
    };

    QComboBox *comboxes[]={
        ui->cmbTuneMode,
        ui->cmbPlanMode,
        ui->cmbTimeWidth,
        ui->cmbInitUnit,

        ui->cmbInitIO,
        ui->cmbLineResponse,
        ui->cmbStopMode,

    };

    install_spy();
}

void mrqAxes2::setupUi()
{
    //! varialbes
    ui->cmbPlanMode->addItem( tr("Linear"), ( (int)1 ) );
    ui->cmbPlanMode->addItem( tr("Cubic"), ( (int)0 ) );
    ui->cmbPlanMode->addItem( tr("Trapezoid"), ( (int)3 ) );

}
void mrqAxes2::desetupUi()
{}

int mrqAxes2::apply()
{
    MegaDevice::deviceMRQ *pDevice;
    pDevice = getDevice();
    Q_ASSERT( NULL != pDevice );

    int ret;

    //! tune
    checked_call( pDevice->setMOTIONPLAN_MOTIONMODE( mAxesId,
                                                     mPage,
                                           (MRQ_MOTIONPLAN_MOTIONMODE_1)ui->cmbTuneMode->currentIndex()
                                           ) );
    checked_call( pDevice->setMOTIONPLAN_MODIFYDUTY( mAxesId,
                                                     mPage,
                                           (MRQ_MOTIONPLAN_MODIFYDUTY_1)ui->cmbTimeWidth->currentIndex()
                                           ) );
    checked_call( pDevice->setMOTIONPLAN_PLANMODE( mAxesId,
                                                   mPage,
                                           (MRQ_MOTIONPLAN_PLANMODE_1)ui->cmbPlanMode->value()
                                           ) );

    checked_call( pDevice->setENCODER_FEEDBACKRATIO( mAxesId,
                                                     ui->spinFeedback->value()
                                           ) );
    //! stop
    checked_call( pDevice->setMOTIONPLAN_STOPMODE( mAxesId,
                                                   mPage,
                                    (MRQ_MOTIONPLAN_STOPMODE_1)ui->cmbStopMode->currentIndex()
                                    ) );
    checked_call( pDevice->setMOTIONPLAN_STOPDISTANCE( mAxesId,
                                                       mPage,
                                    ui->spinStopDistance->value()
                                    ) );
    checked_call( pDevice->setMOTIONPLAN_STOPTIME( mAxesId,
                                                   mPage,
                                    ui->spinStopTime->value()
                                    ) );
    //! init pos
//    checked_call( pDevice->setMOTION_INITPOSITIONUNIT( mAxesId,
//                                         (MRQ_MOTION_INITPOSITIONUNIT)ui->cmbInitUnit->currentIndex() ) );

//    checked_call( pDevice->setMOTION_INITIOSIGNAL( mAxesId,
//                                     (MRQ_MOTION_INITIOSIGNAL)ui->cmbInitIO->currentIndex() ) );

//    checked_call( pDevice->setMOTION_INITPOSITION( mAxesId,
//                                     ui->spinInitPosition->value() ) );

    //! out of line
    checked_call( pDevice->setMOTIONPLAN_OOSLINESTATE( mAxesId,
                                                       mPage,
                                      (MRQ_CAN_NETMANAGELED)ui->chkOutOfLineState->isEnabled() ) );
    checked_call( pDevice->setMOTIONPLAN_OOSLINEOUTNUM( mAxesId,
                                                        mPage,
                                      ui->spinOutOfLine->value() ) );
    checked_call( pDevice->setMOTIONPLAN_OOSLINERESPONSE( mAxesId,
                                                          mPage,
                                        (MRQ_MOTIONPLAN_OOSLINERESPONSE_1)ui->cmbLineResponse->currentIndex() ) );

    return ret;
}
int mrqAxes2::updateUi()
{
    Q_ASSERT( NULL != m_pMrqModel );
    //! tune
    ui->cmbTuneMode->setCurrentIndex( m_pMrqModel->mMOTIONPLAN_MOTIONMODE[mAxesId][0] );
    ui->cmbTimeWidth->setCurrentIndex( ( m_pMrqModel->mMOTIONPLAN_MODIFYDUTY[mAxesId][0]));
    ui->cmbPlanMode->setValue( m_pMrqModel->mMOTIONPLAN_PLANMODE[mAxesId][0] );
    ui->spinFeedback->setValue( ( m_pMrqModel->mMOTOR_PEAKACCELERATION[mAxesId]) );

    //! stop
    ui->cmbStopMode->setCurrentIndex( m_pMrqModel->mMOTIONPLAN_STOPMODE[mAxesId][0] );
    ui->spinStopDistance->setValue( ( m_pMrqModel->mMOTIONPLAN_STOPDISTANCE[mAxesId][0]) );
    ui->spinStopTime->setValue( ( m_pMrqModel->mMOTIONPLAN_STOPTIME[mAxesId][0]) );

//    //! position
//    ui->cmbInitUnit->setCurrentIndex( m_pMrqModel->mMOTION_INITPOSITIONUNIT[mAxesId] );
//    ui->cmbInitIO->setCurrentIndex( m_pMrqModel->mMOTION_INITIOSIGNAL[mAxesId] );
//    ui->spinInitPosition->setValue( ( m_pMrqModel->mMOTION_INITPOSITION[mAxesId]) );

    //! out of line
    ui->chkOutOfLineState->setChecked( m_pMrqModel->mMOTIONPLAN_OOSLINESTATE[mAxesId][0] );
    ui->spinOutOfLine->setValue( m_pMrqModel->mMOTIONPLAN_OOSLINEOUTNUM[mAxesId][0] );
    ui->cmbLineResponse->setCurrentIndex( m_pMrqModel->mMOTIONPLAN_OOSLINERESPONSE[mAxesId][0] );

    return 0;
}
