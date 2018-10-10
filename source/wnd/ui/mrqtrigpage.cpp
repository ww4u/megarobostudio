#include "mrqtrigpage.h"
#include "ui_mrqtrigpage.h"

#include "../../com/comassist.h"

//#define peri_unit   0.001f

MrqTrigPage::MrqTrigPage(QWidget *parent) :
    mrqView(parent),
    ui(new Ui::MrqTrigPage)
{
    ui->setupUi(this);
}

MrqTrigPage::~MrqTrigPage()
{
    delete ui;
}

int MrqTrigPage::setApply()
{
    return apply();
}

void MrqTrigPage::modelChanged()
{
    //! the first time
    if ( mTrigChans.size() < 1 )
    {
        //! trig chan
        Q_ASSERT( NULL != m_pMrqModel );
        MrqTrigChan *pTrigChan;
        for ( int i = 0; i < m_pMrqModel->trigSrcs(); i++ )
        {
            pTrigChan = new MrqTrigChan();
            Q_ASSERT( NULL != pTrigChan );

            mTrigChans.append( pTrigChan );

            Q_ASSERT(  mAxesId < m_pMrqModel->axes() );
            ui->tabWidget->addTab( pTrigChan,
                                   m_pMrqModel->trigSrcAlias( mAxesId, i ) );
        }

        spyEdited();
    }

    updateUi();
}

int MrqTrigPage::apply()
{
    MegaDevice::deviceMRQ *pDevice;
    pDevice = getDevice();
    Q_ASSERT( NULL != pDevice );

    trigLevelConfig levelConifg;
    trigPatternConfig pattConfig;

    pDevice->setTRIGGER_MODE( mAxesId, (MRQ_TRIGGER_MODE)ui->comboBox->currentIndex() );

    MRQ_TRIGGER_LEVELSTATE trigId;
    for ( int i = 0; i < mTrigChans.size(); i++ )
    {
        Q_ASSERT( i <= MRQ_TRIGGER_LEVELSTATE_TRIG5 );

        mTrigChans.at(i)->getConfig( levelConifg, pattConfig );

        trigId = ( MRQ_TRIGGER_LEVELSTATE )pDevice->trigSrcIdList().at( i );

        pDevice->setTRIGGER_LEVELSTATE( mAxesId,
                                        trigId,
                                        (MRQ_SYSTEM_REVMOTION)levelConifg.mbOnOff );

        pDevice->setTRIGGER_LEVELTYPE( mAxesId,
                                        trigId,
                                        (MRQ_TRIGGER_LEVELTYPE_1)levelConifg.mTypeIndex );

        pDevice->setTRIGGER_LEVELRESP( mAxesId,
                                        trigId,
                                        (MRQ_TRIGGER_LEVELRESP_1)levelConifg.mRespIndex );

        pDevice->setTRIGGER_LEVELSPERIOD( mAxesId,
                                        trigId,
                                         levelConifg.mPeriod );

        //! run able
        if ( pDevice->runWaveAble() )
        {
            pDevice->setTRIGGER_LEVELRUNWAVE( mAxesId,
                                            trigId,
                                            (MRQ_MOTION_SWITCH_1)levelConifg.mRunIndex);
        }

        //! \todo patt
    }

    return 0;
}

void MrqTrigPage::spyEdited()
{
    foreach( MrqTrigChan *pPage, mTrigChans )
    {
        LINK_MODIFIED( pPage );
    }
}

int MrqTrigPage::updateUi()
{
    Q_ASSERT( NULL != m_pMrqModel );

    if ( NULL == m_pMrqModel )
    { return ERR_NULL_POINTER; }

    MegaDevice::MRQ_model *pModel;
    pModel = m_pMrqModel->getModel();

    trigLevelConfig levelConifg;
    trigPatternConfig pattConfig;

    //! \todo only pattern
//    ui->comboBox->setCurrentIndex( pModel->mTRIGGER_MODE[mAxesId] );

    int trigId;
    for ( int i = 0; i < mTrigChans.size(); i++ )
    {
        Q_ASSERT( i < sizeof_array(pModel->mTRIGGER_LEVELSTATE[ mAxesId ]) );

        trigId = m_pMrqModel->trigSrcIdList().at( i );

        levelConifg.mbOnOff = pModel->mTRIGGER_LEVELSTATE[ mAxesId ][ trigId ];
        levelConifg.mTypeIndex = pModel->mTRIGGER_LEVELTYPE[ mAxesId ][ trigId ];
        levelConifg.mRespIndex = pModel->mTRIGGER_LEVELRESP[ mAxesId ][ trigId ];
        levelConifg.mPeriod = pModel->mTRIGGER_LEVELSPERIOD[ mAxesId ][ trigId ] /** peri_unit*/;

        levelConifg.mRunIndex = pModel->mTRIGGER_LEVELRUNWAVE[ mAxesId ][ trigId ];

        //! \todo patt config

        //! config the page 0/1
        mTrigChans.at(i)->setConfig( levelConifg,
                                     pattConfig );
    }

    return 0;
}
int MrqTrigPage::updateData()
{
    if ( NULL == m_pMrqModel )
    { return ERR_NULL_POINTER; }

    MegaDevice::MRQ_model *pModel;
    pModel = m_pMrqModel->getModel();

    trigLevelConfig levelConifg;
    trigPatternConfig pattConfig;

    pModel->mTRIGGER_MODE[ mAxesId ] = (MRQ_TRIGGER_MODE)ui->comboBox->currentIndex();

    int trigId;
    for ( int i = 0; i < mTrigChans.size(); i++ )
    {
        Q_ASSERT( i < sizeof_array(pModel->mTRIGGER_LEVELSTATE[ mAxesId ]) );

        mTrigChans.at(i)->getConfig( levelConifg, pattConfig );

        trigId = m_pMrqModel->trigSrcIdList().at( i );

        pModel->mTRIGGER_LEVELSTATE[ mAxesId ][ trigId ] = (MRQ_SYSTEM_REVMOTION)levelConifg.mbOnOff;
        pModel->mTRIGGER_LEVELTYPE[ mAxesId ][ trigId ] = (MRQ_TRIGGER_LEVELTYPE_1)levelConifg.mTypeIndex;
        pModel->mTRIGGER_LEVELRESP[ mAxesId ][ trigId ] = (MRQ_TRIGGER_LEVELRESP_1)levelConifg.mRespIndex;
        pModel->mTRIGGER_LEVELSPERIOD[ mAxesId ][ trigId ] = levelConifg.mPeriod;

        pModel->mTRIGGER_LEVELRUNWAVE[ mAxesId ][ trigId ] = (MRQ_MOTION_SWITCH_1)levelConifg.mRunIndex;

        //! \todo patt
    }

    return 0;
}
