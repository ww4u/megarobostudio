#include "mrqalarm.h"
#include "ui_mrqalarm.h"

MrqAlarm::MrqAlarm(QWidget *parent) :
    mrqView(parent),
    ui(new Ui::MrqAlarm)
{
    ui->setupUi(this);

    //! \note only for the T4
    for ( int i = 0; i < 4; i++ )
    {
        mAnglePages.append( new EncoderAlarmPage() );
        Q_ASSERT( mAnglePages.at(i) != NULL );

        ui->tabWidget->insertTab( i, mAnglePages.at(i), QString("%1%2").arg( tr("S")).arg(i+1) );
    }

    for ( int i = 0; i < 4; i++ )
    {
        mDistPages.append( new DistanceAlarmPage() );
        Q_ASSERT( mDistPages.at(i) != NULL );

        ui->tabWidget_2->insertTab( i, mDistPages.at(i), QString("%1%2").arg( tr("S")).arg(i+1) );
    }

    spyEdited();
}

MrqAlarm::~MrqAlarm()
{
    delete ui;
}

void MrqAlarm::changeEvent(QEvent * event)
{
    mrqView::changeEvent( event );

    if (event->type() == QEvent::LanguageChange)
    { ui->retranslateUi( this ); }
}

int MrqAlarm::setApply()
{
    int ret;

    ret = applyAngleAlarm();
    if ( ret != 0 )
    { return ret; }

    ret = applyDistanceAlarm();
    if ( ret != 0 )
    { return ret; }

    return ret;
}

void MrqAlarm::modelChanged()
{
    updateUi();
}

void MrqAlarm::spyEdited()
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

void MrqAlarm::updateUi()
{
    Q_ASSERT( NULL != m_pMrqModel);

    //! angle configs
    EncoderAlarmConfig encAlarmConfig;
    int angleDir;
    for ( int i = 0; i < 4; i++ )
    {
        angleDir = m_pMrqModel->mAngleDir.at(i) ? 1 : -1;
        encAlarmConfig.mbEn = m_pMrqModel->mABSENCALARM_STATE[i];
        encAlarmConfig.mUpLimit = m_pMrqModel->mABSENCALARM_UPLIMIT[i] * angleDir;
        encAlarmConfig.mDownLimit = m_pMrqModel->mABSENCALARM_DOWNLIMIT[i] * angleDir;
        encAlarmConfig.mZero = m_pMrqModel->mABSENCALARM_ZEROVALUE[i];

        mAnglePages.at(i)->setData( encAlarmConfig );

        if ( m_pmcModel->mSysPref.mSysMode == sys_normal )
        { mAnglePages.at(i)->setZeroEnable(false); }
        else
        { mAnglePages.at(i)->setZeroEnable(true); }
    }

    ui->comboBox->setCurrentIndex( m_pMrqModel->mABSENCALARM_RESPONSE );

    //! dist configs
    DistAlarmConfig distAlarmConfig;
    for ( int i = 0; i < 4; i++ )
    {
        distAlarmConfig.mbEn = m_pMrqModel->mDISTANCEALARM_STATE[i];
        distAlarmConfig.mDist[0] = m_pMrqModel->mDISTANCEALARM_ALARM1DIST[i];
        distAlarmConfig.mDist[1] = m_pMrqModel->mDISTANCEALARM_ALARM2DIST[i];
        distAlarmConfig.mDist[2] = m_pMrqModel->mDISTANCEALARM_ALARM3DIST[i];

        mDistPages.at(i)->setData( distAlarmConfig );
    }
}

int MrqAlarm::applyAngleAlarm()
{
    MegaDevice::deviceMRQ *pDevice;
    pDevice = getDevice();
    Q_ASSERT( NULL != pDevice );

    EncoderAlarmConfig encAlarmConfig;
    int angleDir;
    for ( int i = 0; i < 4; i++ )
    {
        angleDir = m_pMrqModel->mAngleDir.at(i) ? 1 : -1;

        mAnglePages.at(i)->data( encAlarmConfig );

        pDevice->setABSENCALARM_STATE( (MRQ_IDENTITY_LABEL_1)i,
                                        (MRQ_SYSTEM_REVMOTION)encAlarmConfig.mbEn, DIFF_APPLY );

        pDevice->setABSENCALARM_UPLIMIT( (MRQ_IDENTITY_LABEL_1)i,
                                         encAlarmConfig.mUpLimit*angleDir, DIFF_APPLY );

        pDevice->setABSENCALARM_DOWNLIMIT( (MRQ_IDENTITY_LABEL_1)i,
                                         encAlarmConfig.mDownLimit*angleDir, DIFF_APPLY );

        pDevice->setABSENCALARM_ZEROVALUE( (MRQ_IDENTITY_LABEL_1)i,
                                         encAlarmConfig.mZero, DIFF_APPLY );

//        pDevice->setABSENCALARM_ZEROPOSITION( (MRQ_IDENTITY_LABEL_1)i,
//                                         (MRQ_ABSENCALARM_ZEROPOSITION_1)encAlarmConfig.mZeroPos );
    }

    pDevice->setABSENCALARM_RESPONSE( (MRQ_MOTIONPLAN_OOSLINERESPONSE_1)ui->comboBox->currentIndex(), DIFF_APPLY );

    return 0;
}
int MrqAlarm::applyDistanceAlarm()
{
    MegaDevice::deviceMRQ *pDevice;
    pDevice = getDevice();
    Q_ASSERT( NULL != pDevice );

    DistAlarmConfig distConfig;
    for ( int i = 0; i < 4; i++ )
    {
        mDistPages.at(i)->data( distConfig );

        pDevice->setDISTANCEALARM_STATE( (MRQ_IDENTITY_LABEL_1)i,
                                        (MRQ_SYSTEM_REVMOTION)distConfig.mbEn, DIFF_APPLY );

        pDevice->setDISTANCEALARM_ALARM1DIST( (MRQ_IDENTITY_LABEL_1)i,
                                         distConfig.mDist[0], DIFF_APPLY);

        pDevice->setDISTANCEALARM_ALARM2DIST( (MRQ_IDENTITY_LABEL_1)i,
                                         distConfig.mDist[1], DIFF_APPLY);

        pDevice->setDISTANCEALARM_ALARM3DIST( (MRQ_IDENTITY_LABEL_1)i,
                                         distConfig.mDist[2], DIFF_APPLY);
    }

    return 0;
}
