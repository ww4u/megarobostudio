#include "mrqin.h"
#include "ui_mrqin.h"

#include "../../com/comassist.h"
#define peri_unit   0.001f

MrqIn::MrqIn(QWidget *parent) :
    mrqView(parent),
    ui(new Ui::MrqIn)
{
    ui->setupUi(this);

    setupUi();

    spyEdited();
}

MrqIn::~MrqIn()
{
    delete ui;

    desetupUi();
}

int MrqIn::setApply()
{
    return apply();
}

void MrqIn::modelChanged()
{
    adaptUi();

    updateUi();
}

void MrqIn::spyEdited()
{
    modelView *pView;
    for ( int i = 0; i < ui->tabWidget->count(); i++ )
    {
        pView = (modelView*)ui->tabWidget->widget(i);
        LINK_MODIFIED( pView );
    }

    for ( int i = 0; i < ui->tabWidget_2->count(); i++ )
    {
        pView = (modelView*)ui->tabWidget->widget(i);
        LINK_MODIFIED( pView );
    }
}

void MrqIn::setupUi()
{}
void MrqIn::desetupUi()
{}

int MrqIn::apply()
{
    MegaDevice::deviceMRQ *pDevice;
    pDevice = getDevice();
    Q_ASSERT( NULL != pDevice );

    int ret;

    {
        XiConfig cfg;

        ui->tab->getConfig( cfg );

        checked_call( pDevice->setISOLATORIN_STATE(
                                      (MRQ_SYSTEM_REVMOTION)cfg.mbEn ) );
        checked_call( pDevice->setISOLATORIN_TYPE(
                                         (MRQ_TRIGGER_LEVELTYPE_1)cfg.mTypeIndex ) );
        checked_call( pDevice->setISOLATORIN_RESPONSE( (MRQ_MOTIONPLAN_OOSLINERESPONSE_1)cfg.mRespIndex ) );
        checked_call( pDevice->setISOLATORIN_RESPCHAN( (byte)cfg.mRespCH) );

        checked_call( pDevice->setISOLATORIN_SPERIOD( (uint32)comAssist::align( cfg.mPeri, peri_unit) ) );
    }

    //! for ai
    if ( m_pMrqModel->ains() > 0 )
    {
        AiConfig cfg;
        ui->tab_5->getConfig( cfg );

        checked_call( pDevice->setANALOGIN_STATE(
                                      (MRQ_SYSTEM_REVMOTION)cfg.mbEn ) );
        checked_call( pDevice->setANALOGIN_THRESHOLDH( cfg.mHT ) );
        checked_call( pDevice->setANALOGIN_THRESHOLDL( cfg.mLT ) );
        checked_call( pDevice->setANALOGIN_RESPONSEH( (MRQ_MOTIONPLAN_OOSLINERESPONSE_1)cfg.mHResp) );
        checked_call( pDevice->setANALOGIN_RESPONSEL( (MRQ_MOTIONPLAN_OOSLINERESPONSE_1)cfg.mLResp) );
    }

    return 0;
}

int MrqIn::updateUi()
{
    //! xi
    XiConfig cfg;
    for ( int i = 0; i <m_pMrqModel->isis(); i++ )
    {
        cfg.mbEn = m_pMrqModel->mISOLATORIN_STATE;

        cfg.mTypeIndex = m_pMrqModel->mISOLATORIN_STATE;
        cfg.mRespIndex = m_pMrqModel->mISOLATORIN_RESPONSE;
        cfg.mRespCH = m_pMrqModel->mISOLATORIN_RESPCHAN;
        cfg.mPeri = m_pMrqModel->mISOLATORIN_SPERIOD * peri_unit;

        //! cfg
        ((MrqXI*)ui->tabWidget->widget( i ))->setConfig( cfg );
    }

    //! ax
    if ( m_pMrqModel->ains() > 0 )
    {
        AiConfig cfg;

        cfg.mbEn = m_pMrqModel->mANALOGIN_STATE;
        cfg.mHT = m_pMrqModel->mANALOGIN_THRESHOLDH;
        cfg.mLT = m_pMrqModel->mANALOGIN_THRESHOLDL;

        cfg.mHResp = m_pMrqModel->mANALOGIN_RESPONSEH;
        cfg.mLResp = m_pMrqModel->mANALOGIN_RESPONSEL;

        ((MrqAi*)ui->tabWidget_2->widget( 0 ))->setConfig( cfg );
    }

    return 0;
}

void MrqIn::adaptUi()
{
    //! adapt the xin
    if ( m_pMrqModel->isis() > 0 )
    {
        ui->groupBox_7->setVisible( true );

        int cnt = ui->tabWidget->count();
        for ( int i = m_pMrqModel->isis(); i  < cnt; i++ )
        {
            ui->tabWidget->removeTab( m_pMrqModel->isis() );
        }
    }
    else
    { ui->groupBox_7->setVisible( false ); }

    //! ai
    if ( m_pMrqModel->ains() > 0 )
    { ui->groupBox->setVisible( true); }
    else
    { ui->groupBox->setVisible( false ); }
}

