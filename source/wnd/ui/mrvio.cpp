#include "mrvio.h"
#include "ui_mrvio.h"

MrvIo::MrvIo(QWidget *parent) :
    mrvView(parent),
    ui(new Ui::MrvIo)
{
    ui->setupUi(this);
}

MrvIo::~MrvIo()
{
    delete ui;
}

int MrvIo::setApply()
{
    int ret = apply();

    return ret;
}

void MrvIo::modelChanged()
{
//    adaptUi();

    updateUi();
}

void MrvIo::spyEdited()
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
    };

    QComboBox *comboxes[]={
        ui->cmbMode,

    };

    install_spy();

    //! sub page
    LINK_MODIFIED(ui->pageUart);
    LINK_MODIFIED(ui->pageSwitch);
}


int MrvIo::apply()
{
    //! mode
    m_pMRV->setIOCONFIG_IOFUNCSEL( mAxesId, (MRV_IOCONFIG_IOFUNCSEL)ui->cmbMode->currentIndex() );

    //! uart
    uartConfig uCfg;
    ui->pageUart->getConfig( uCfg );

    m_pMRV->setIOCONFIG_BAUD( mAxesId, (MRV_RS232_BAUD)uCfg.mBaudInd );
    m_pMRV->setIOCONFIG_WORDLEN( mAxesId, (MRV_RS232_WORDLEN)uCfg.mDataInd );
    m_pMRV->setIOCONFIG_PARITY( mAxesId, (MRV_RS232_PARITY)uCfg.mParityInd );
    m_pMRV->setIOCONFIG_STOPBIT( mAxesId, (MRV_RS232_STOPBIT)uCfg.mStopInd );

    m_pMRV->setIOCONFIG_FLOWCTL( mAxesId, (MRV_IOCONFIG_FLOWCTL)uCfg.mFlowInd );

    //! switch
    switchConfig sCfg;
    ui->pageSwitch->getConifig( sCfg );

    m_pMRV->setIOCONFIG_SWFILTERCOUNTER( mAxesId, sCfg.mFilterCount );
    m_pMRV->setIOCONFIG_SWFILTERDELAY( mAxesId, sCfg.mFilterDelay );
    m_pMRV->setIOCONFIG_TRIGEDGE( mAxesId, (MRV_IOCONFIG_TRIGEDGE)sCfg.mTrigEdge );

    //! apply
    m_pMRV->setIOCONFIG_APPLYPARA( mAxesId );

    return 0;
}

int MrvIo::updateUi()
{
    //! mode
    ui->cmbMode->setCurrentIndex( m_pMRV->mIOCONFIG_IOFUNCSEL[mAxesId] );

    //! uart
    uartConfig uCfg;
    uCfg.mBaudInd = m_pMRV->mIOCONFIG_BAUD[ mAxesId ];
    uCfg.mDataInd = m_pMRV->mIOCONFIG_WORDLEN[mAxesId];
    uCfg.mParityInd = m_pMRV->mIOCONFIG_PARITY[mAxesId];
    uCfg.mStopInd = m_pMRV->mIOCONFIG_STOPBIT[mAxesId];

    uCfg.mFlowInd = m_pMRV->mIOCONFIG_FLOWCTL[mAxesId];
    ui->pageUart->setConfig( uCfg );

    //! switch
    switchConfig sCfg;
    sCfg.mFilterCount = m_pMRV->mIOCONFIG_SWFILTERCOUNTER[mAxesId];
    sCfg.mFilterDelay = m_pMRV->mIOCONFIG_SWFILTERDELAY[mAxesId];
    sCfg.mTrigEdge = m_pMRV->mIOCONFIG_TRIGEDGE[mAxesId];
    ui->pageSwitch->setConfig( sCfg );

    return 0;
}


