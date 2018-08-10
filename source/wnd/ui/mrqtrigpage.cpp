#include "mrqtrigpage.h"
#include "ui_mrqtrigpage.h"

#include "../../com/comassist.h"

//#define peri_unit   0.001f

MrqTrigPage::MrqTrigPage(QWidget *parent) :
    mrqView(parent),
    ui(new Ui::MrqTrigPage)
{
    ui->setupUi(this);

    //! sub chans
//    mTrigChans.append( ui->tabLeft );
//    mTrigChans.append( ui->tabRight );

//    MrqTrigChan *pTrigChan;
//    for ( int i = 0; i < 5; i++ )
//    {
//        pTrigChan = new MrqTrigChan();
//        Q_ASSERT( NULL != pTrigChan );

//        mTrigChans.append( pTrigChan );

//        ui->tabWidget->addTab( pTrigChan, QString("TRIG%1").arg(i+1) );
//    }

//    spyEdited();
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

    for ( int i = 0; i < mTrigChans.size(); i++ )
    {
        Q_ASSERT( i <= MRQ_TRIGGER_LEVELSTATE_TRIG5 );

        mTrigChans.at(i)->getConfig( levelConifg, pattConfig );

        pDevice->setTRIGGER_LEVELSTATE( mAxesId,
                                        (MRQ_TRIGGER_LEVELSTATE)i,
                                        (MRQ_SYSTEM_REVMOTION)levelConifg.mbOnOff );

        pDevice->setTRIGGER_LEVELTYPE( mAxesId,
                                        (MRQ_TRIGGER_LEVELSTATE)i,
                                        (MRQ_TRIGGER_LEVELTYPE_1)levelConifg.mTypeIndex );

        pDevice->setTRIGGER_LEVELRESP( mAxesId,
                                        (MRQ_TRIGGER_LEVELSTATE)i,
                                        (MRQ_MOTIONPLAN_OOSLINERESPONSE_1)levelConifg.mRespIndex );

        pDevice->setTRIGGER_LEVELSPERIOD( mAxesId,
                                        (MRQ_TRIGGER_LEVELSTATE)i,
                                         levelConifg.mPeriod );

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

    for ( int i = 0; i < mTrigChans.size(); i++ )
    {
        Q_ASSERT( i < sizeof_array(pModel->mTRIGGER_LEVELSTATE[ mAxesId ]) );

        levelConifg.mbOnOff = pModel->mTRIGGER_LEVELSTATE[ mAxesId ][ i ];
        levelConifg.mTypeIndex = pModel->mTRIGGER_LEVELTYPE[ mAxesId ][ i ];
        levelConifg.mRespIndex = pModel->mTRIGGER_LEVELRESP[ mAxesId ][ i ];
        levelConifg.mPeriod = pModel->mTRIGGER_LEVELSPERIOD[ mAxesId ][ i ] /** peri_unit*/;

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

    for ( int i = 0; i < mTrigChans.size(); i++ )
    {
        Q_ASSERT( i < sizeof_array(pModel->mTRIGGER_LEVELSTATE[ mAxesId ]) );

        mTrigChans.at(i)->getConfig( levelConifg, pattConfig );

        pModel->mTRIGGER_LEVELSTATE[ mAxesId ][ i ] = (MRQ_SYSTEM_REVMOTION)levelConifg.mbOnOff;
        pModel->mTRIGGER_LEVELTYPE[ mAxesId ][ i ] = (MRQ_TRIGGER_LEVELTYPE_1)levelConifg.mTypeIndex;
        pModel->mTRIGGER_LEVELRESP[ mAxesId ][ i ] = (MRQ_MOTIONPLAN_OOSLINERESPONSE_1)levelConifg.mRespIndex;
        pModel->mTRIGGER_LEVELSPERIOD[ mAxesId ][ i ] = levelConifg.mPeriod;

        //! \todo patt
    }

    return 0;
}
