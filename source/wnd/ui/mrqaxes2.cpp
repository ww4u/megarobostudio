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
{ updateData(); }

void mrqAxes2::setupUi()
{}
void mrqAxes2::desetupUi()
{}

int mrqAxes2::apply()
{
    MegaDevice::deviceMRQ *pDevice;
    pDevice = getDevice();
    Q_ASSERT( NULL != pDevice );

    //! tune
    pDevice->setMOTIONPLAN_MOTIONMODE( mAxesId,
                                           (MRQ_MOTIONPLAN_MOTIONMODE)ui->cmbTuneMode->currentIndex()
                                           );
    pDevice->setMOTIONPLAN_MODIFYDUTY( mAxesId,
                                           (MRQ_MOTIONPLAN_MODIFYDUTY)ui->cmbTimeWidth->currentIndex()
                                           );
    pDevice->setMOTIONPLAN_FEEDBACKRATIO( mAxesId,
                                           (MRQ_MOTIONPLAN_MODIFYDUTY)ui->cmbTuneFb->currentText().toInt()
                                           );

    //! stop
    pDevice->setSTOPDECEL_MODE( mAxesId,
                                    (MRQ_STOPDECEL_MODE)ui->cmbStopMode->currentIndex()
                                    );
    pDevice->setSTOPDECEL_DISTANCE( mAxesId,
                                    ui->cmbStopDist->currentText().toFloat()
                                    );
    pDevice->setSTOPDECEL_TIME( mAxesId,
                                    ui->cmbStopTime->currentText().toFloat()
                                    );

    //! \todo loss step

    //! init pos
    pDevice->getModel()->setMOTION_INITPOSITIONUNIT( mAxesId,
                                                         (MRQ_MOTION_INITPOSITIONUNIT)ui->cmbInitUnit->currentIndex() );

    pDevice->getModel()->setMOTION_INITIOSIGNAL( mAxesId,
                                                         (MRQ_MOTION_INITIOSIGNAL)ui->cmbInitIO->currentIndex() );

    pDevice->getModel()->setMOTION_INITPOSITION( mAxesId,
                                                         ui->cmbInitPos->currentText().toFloat() );

    return 0;
}
int mrqAxes2::updateData()
{
    ui->cmbTuneMode->setCurrentIndex( m_pMrqModel->mMOTIONPLAN_MOTIONMODE[mAxesId] );
    ui->cmbTimeWidth->setCurrentText( QString::number( m_pMrqModel->mMOTIONPLAN_MODIFYDUTY[mAxesId]));
    ui->cmbTuneFb->setCurrentText( QString::number( m_pMrqModel->mMOTIONPLAN_FEEDBACKRATIO[mAxesId]));

    ui->cmbStopMode->setCurrentIndex( m_pMrqModel->mSTOPDECEL_MODE[mAxesId] );
    ui->cmbStopDist->setCurrentText( QString::number( m_pMrqModel->mSTOPDECEL_DISTANCE[mAxesId]) );
    ui->cmbStopTime->setCurrentText( QString::number( m_pMrqModel->mSTOPDECEL_TIME[mAxesId]) );

    ui->cmbInitUnit->setCurrentIndex( m_pMrqModel->mMOTION_INITPOSITIONUNIT[mAxesId] );
    ui->cmbInitIO->setCurrentIndex( m_pMrqModel->mMOTION_INITIOSIGNAL[mAxesId] );
    ui->cmbInitPos->setCurrentText( QString::number( m_pMrqModel->mMOTION_INITPOSITION[mAxesId]) );

    return 0;
}
