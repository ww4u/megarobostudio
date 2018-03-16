#include "pvtpref.h"
#include "ui_pvtpref.h"


modelPvtPref::modelPvtPref()
{
    mVernierStep = 64;
    mSteps = 200;
    mEncoderLines = 2000;
    mSlowRatio = 10;
}

PvtPref::PvtPref(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PvtPref)
{
    ui->setupUi(this);

    ui->cmbVernier->addItemInt( "256" );
    ui->cmbVernier->addItemInt( "128" );
    ui->cmbVernier->addItemInt( "64" );
    ui->cmbVernier->addItemInt( "32" );
    ui->cmbVernier->addItemInt( "16" );
    ui->cmbVernier->addItemInt( "8" );
    ui->cmbVernier->addItemInt( "4" );
    ui->cmbVernier->addItemInt( "2" );
    ui->cmbVernier->addItemInt( "1" );

    m_pPref = NULL;
}

PvtPref::~PvtPref()
{
    delete ui;
}

void PvtPref::setModel( modelPvtPref *pref )
{
    Q_ASSERT( NULL != pref );

    m_pPref = pref;

    updateUi();
}

modelPvtPref *PvtPref::model()
{
    return m_pPref;
}

void PvtPref::updateData()
{
    Q_ASSERT( NULL != m_pPref );

    m_pPref->mVernierStep = ui->cmbVernier->value();
    m_pPref->mSteps = ui->spinStepCount->value();
    m_pPref->mEncoderLines = ui->spinEncoder->value();
    m_pPref->mSlowRatio = ui->spinSlow->value();
}
void PvtPref::updateUi()
{
    Q_ASSERT( NULL != m_pPref );

    ui->cmbVernier->setValue( m_pPref->mVernierStep );
    ui->spinStepCount->setValue( m_pPref->mSteps );
    ui->spinEncoder->setValue( m_pPref->mEncoderLines );
    ui->spinSlow->setValue( m_pPref->mSlowRatio );
}

void PvtPref::on_PvtPref_accepted()
{
    updateData();
}
