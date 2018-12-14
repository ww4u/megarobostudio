#include "mrqio.h"
#include "ui_mrqio.h"
#include "../../com/comassist.h"

#define peri_unit   0.001f
#define duty_unit   0.001f

mrqIo::mrqIo(QWidget *parent) :
    mrqView(parent),
    ui(new Ui::mrqIo)
{
    ui->setupUi(this);

    setupUi();

    spyEdited();
}

mrqIo::~mrqIo()
{
    delete ui;

    desetupUi();
}

int mrqIo::setApply()
{
    return apply();
}

void mrqIo::modelChanged()
{
    adaptUi();

    updateUi();
}

void mrqIo::spyEdited()
{
    modelView *pView;
    for ( int i = 0; i < ui->tabWidget->count(); i++ )
    {
        pView = (modelView*)ui->tabWidget->widget(i);
        LINK_MODIFIED( pView );
    }

    for ( int i = 0; i < ui->tabWidget_2->count(); i++ )
    {
        pView = (modelView*)ui->tabWidget_2->widget(i);
        LINK_MODIFIED( pView );
    }
}

void mrqIo::setupUi()
{

}
void mrqIo::desetupUi()
{}

int mrqIo::apply()
{
    MegaDevice::deviceMRQ *pDevice;
    pDevice = getDevice();
    Q_ASSERT( NULL != pDevice );

    int ret;

    //! for yo apply
    for ( int i = 0; i < m_pMrqModel->isos(); i++ )
    {
        YoConfig cfg;

        ((MrqYO*)ui->tabWidget->widget( i ))->getConfig( cfg );

        MRQ_ISOLATOROUTPUT_STATE isoCH = (MRQ_ISOLATOROUTPUT_STATE)i;

        checked_call( pDevice->setISOLATOROUTPUT_STATE( isoCH,
                                          (MRQ_DIGITALOUTPUT_STATE_1)cfg.mbEn, DIFF_APPLY ) );
        checked_call( pDevice->setISOLATOROUTPUT_SOURCE( isoCH,
                                           cfg.mSrcIndex, DIFF_APPLY ) );
        checked_call( pDevice->setISOLATOROUTPUT_CONDITION( isoCH,
                                              (MRQ_DIGITALOUTPUT_CONDITION_1)cfg.mCondIndex, DIFF_APPLY ) );
        checked_call( pDevice->setISOLATOROUTPUT_RESPONSE( isoCH,
                                             (MRQ_ISOLATOROUTPUT_RESPONSE_1)cfg.mRespIndex, DIFF_APPLY ) );
    }

    //! for do apply
    for ( int i = 0; i < m_pMrqModel->dos(); i++ )
    {
        DoConfig cfg;

        ((MrqDO*)ui->tabWidget_2->widget( i ))->getConfig( cfg );

        MRQ_DIGITALOUTPUT_STATE doCH;
        doCH = (MRQ_DIGITALOUTPUT_STATE)i;

        checked_call( pDevice->setDIGITALOUTPUT_STATE( doCH,
                                      (MRQ_DIGITALOUTPUT_STATE_1)cfg.mbEn, DIFF_APPLY ) );
        checked_call( pDevice->setDIGITALOUTPUT_POLARITY( doCH,
                                         (MRQ_DIGITALOUTPUT_POLARITY_1)cfg.mPolarityIndex, DIFF_APPLY ) );
        checked_call( pDevice->setDIGITALOUTPUT_PERIOD( doCH, (quint32)comAssist::align( cfg.mPeri, peri_unit), DIFF_APPLY ) );
        checked_call( pDevice->setDIGITALOUTPUT_DUTY( doCH, (quint16)comAssist::align( cfg.mDuty, duty_unit), DIFF_APPLY ) );

        checked_call( pDevice->setDIGITALOUTPUT_SOURCE( doCH,
                                                        cfg.mSrcIndex, DIFF_APPLY ) );
        checked_call( pDevice->setDIGITALOUTPUT_CONDITION( doCH,
                                          (MRQ_DIGITALOUTPUT_CONDITION_1)cfg.mCondIndex, DIFF_APPLY ) );
        checked_call( pDevice->setDIGITALOUTPUT_SIGNAL( doCH,
                                       (MRQ_DIGITALOUTPUT_SIGNAL_1)cfg.mSignalIndex, DIFF_APPLY ) );
    }

    return 0;
}

int mrqIo::updateUi()
{
    //! for yo apply
    for ( int i = 0; i < m_pMrqModel->isos(); i++ )
    {
        YoConfig cfg;

        cfg.mbEn = m_pMrqModel->mISOLATOROUTPUT_STATE[ i ];

        cfg.mSrcIndex = m_pMrqModel->mISOLATOROUTPUT_SOURCE[ i ];
        cfg.mCondIndex = m_pMrqModel->mISOLATOROUTPUT_CONDITION[ i ];
        cfg.mRespIndex = m_pMrqModel->mISOLATOROUTPUT_RESPONSE[ i ];

        ((MrqYO*)ui->tabWidget->widget( i ))->setConfig( cfg );
    }

    //! for do apply
    for ( int i = 0; i < m_pMrqModel->dos(); i++ )
    {
        DoConfig cfg;

        cfg.mbEn = m_pMrqModel->mDIGITALOUTPUT_STATE[ i ];

        cfg.mPolarityIndex = m_pMrqModel->mDIGITALOUTPUT_POLARITY[i];
        cfg.mSignalIndex = m_pMrqModel->mDIGITALOUTPUT_SIGNAL[i];
        cfg.mSrcIndex = m_pMrqModel->mDIGITALOUTPUT_SOURCE[i];
        cfg.mCondIndex = m_pMrqModel->mDIGITALOUTPUT_CONDITION[i];

        cfg.mPeri  = m_pMrqModel->mDIGITALOUTPUT_PERIOD[i] * peri_unit;
        cfg.mDuty  = m_pMrqModel->mDIGITALOUTPUT_DUTY[i] * duty_unit;

        ((MrqDO*)ui->tabWidget_2->widget( i ))->setConfig( cfg );
    }

    return 0;
}

void mrqIo::adaptUi()
{
    int cnt;

    //! for yo
    cnt = ui->tabWidget->count();
    for ( int i = m_pMrqModel->isos(); i < cnt; i++ )
    {
        ui->tabWidget->removeTab( m_pMrqModel->isos() );
    }

    //! for do
    cnt = ui->tabWidget_2->count();
    for ( int i = m_pMrqModel->dos(); i < cnt; i++ )
    {
        ui->tabWidget_2->removeTab( m_pMrqModel->dos() );
    }
}




