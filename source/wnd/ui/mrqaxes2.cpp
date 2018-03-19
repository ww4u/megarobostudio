#include "mrqaxes2.h"
#include "ui_mrqaxes2.h"

mrqAxes2::mrqAxes2(QWidget *parent) :
    mrqView(parent),
    ui(new Ui::mrqaxes2)
{
    ui->setupUi(this);

    setupUi();
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
                                           (MRQ_MOTIONPLAN_MOTIONMODE)ui->cmbTuneMode->currentIndex()
                                           ) );
    checked_call( pDevice->setMOTIONPLAN_MODIFYDUTY( mAxesId,
                                           (MRQ_MOTIONPLAN_MODIFYDUTY)ui->cmbTimeWidth->currentIndex()
                                           ) );
    checked_call( pDevice->setMOTIONPLAN_PLANMODE( mAxesId,
                                           (MRQ_MOTIONPLAN_PLANMODE)ui->cmbPlanMode->value()
                                           ) );

    checked_call( pDevice->setMOTIONPLAN_FEEDBACKRATIO( mAxesId,
                                           (MRQ_MOTIONPLAN_MODIFYDUTY)ui->spinFeedback->value()
                                           ) );
    //! stop
    checked_call( pDevice->setSTOPDECEL_MODE( mAxesId,
                                    (MRQ_STOPDECEL_MODE)ui->cmbStopMode->currentIndex()
                                    ) );
    checked_call( pDevice->setSTOPDECEL_DISTANCE( mAxesId,
                                    ui->spinStopDistance->value()
                                    ) );
    checked_call( pDevice->setSTOPDECEL_TIME( mAxesId,
                                    ui->spinStopTime->value()
                                    ) );
    //! init pos
    checked_call( pDevice->setMOTION_INITPOSITIONUNIT( mAxesId,
                                         (MRQ_MOTION_INITPOSITIONUNIT)ui->cmbInitUnit->currentIndex() ) );

    checked_call( pDevice->setMOTION_INITIOSIGNAL( mAxesId,
                                     (MRQ_MOTION_INITIOSIGNAL)ui->cmbInitIO->currentIndex() ) );

    checked_call( pDevice->setMOTION_INITPOSITION( mAxesId,
                                     ui->spinInitPosition->value() ) );

    //! out of line
    checked_call( pDevice->setOUTOFSTEP_LINESTATE( mAxesId,
                                      (MRQ_SYSTEM_REPORTSWITCH)ui->chkOutOfLineState->isEnabled() ) );
    checked_call( pDevice->setOUTOFSTEP_LINEOUTNUM( mAxesId,
                                      ui->spinOutOfLine->value() ) );
    checked_call( pDevice->setOUTOFSTEP_LINERESPONSE( mAxesId,
                                        (MRQ_OUTOFSTEP_LINERESPONSE)ui->cmbLineResponse->currentIndex() ) );

    return ret;
}
int mrqAxes2::updateUi()
{
    //! tune
    ui->cmbTuneMode->setCurrentIndex( m_pMrqModel->mMOTIONPLAN_MOTIONMODE[mAxesId] );
    ui->cmbTimeWidth->setCurrentIndex( ( m_pMrqModel->mMOTIONPLAN_MODIFYDUTY[mAxesId]));
    ui->cmbPlanMode->setValue( m_pMrqModel->mMOTIONPLAN_PLANMODE[mAxesId] );
    ui->spinFeedback->setValue( ( m_pMrqModel->mMOTIONPLAN_FEEDBACKRATIO[mAxesId]) );

    //! stop
    ui->cmbStopMode->setCurrentIndex( m_pMrqModel->mSTOPDECEL_MODE[mAxesId] );
    ui->spinStopDistance->setValue( ( m_pMrqModel->mSTOPDECEL_DISTANCE[mAxesId]) );
    ui->spinStopTime->setValue( ( m_pMrqModel->mSTOPDECEL_TIME[mAxesId]) );

    //! position
    ui->cmbInitUnit->setCurrentIndex( m_pMrqModel->mMOTION_INITPOSITIONUNIT[mAxesId] );
    ui->cmbInitIO->setCurrentIndex( m_pMrqModel->mMOTION_INITIOSIGNAL[mAxesId] );
    ui->spinInitPosition->setValue( ( m_pMrqModel->mMOTION_INITPOSITION[mAxesId]) );

    //! out of line
    ui->chkOutOfLineState->setChecked( m_pMrqModel->mOUTOFSTEP_LINESTATE[mAxesId] );
    ui->spinOutOfLine->setValue( m_pMrqModel->mOUTOFSTEP_LINEOUTNUM[mAxesId] );
    ui->cmbLineResponse->setCurrentIndex( m_pMrqModel->mOUTOFSTEP_LINERESPONSE[mAxesId] );

    return 0;
}
