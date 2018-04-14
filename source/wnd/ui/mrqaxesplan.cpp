#include "mrqaxesplan.h"
#include "ui_mrqaxesplan.h"

MrqAxesPlan::MrqAxesPlan(QWidget *parent) :
    mrqView(parent),
    ui(new Ui::MrqAxesPlan)
{
    ui->setupUi(this);

    spyEdited();
}

MrqAxesPlan::~MrqAxesPlan()
{
    delete ui;
}

int MrqAxesPlan::setApply()
{
    return apply();
}

void MrqAxesPlan::modelChanged()
{
    updateUi();logDbg();
}

void MrqAxesPlan::spyEdited()
{

}

int MrqAxesPlan::apply()
{
    MegaDevice::deviceMRQ *pDevice;
    pDevice = getDevice();
    Q_ASSERT( NULL != pDevice );

    int ret;
    checked_call( pDevice->setMOTIONPLAN_ENDSTATE( mAxesId,
                                                     mPage,
                                           (MRQ_MOTIONPLAN_ENDSTATE_1)ui->cmbEndState->currentIndex()
                                           ) );

    return 0;
}
int MrqAxesPlan::updateUi()
{
    Q_ASSERT( NULL != m_pMrqModel );

    if ( NULL == m_pMrqModel )
    { return ERR_NULL_POINTER; }

    //! only for page0
    ui->cmbEndState->setCurrentIndex( m_pMrqModel->mMOTIONPLAN_ENDSTATE[mAxesId][0] );

    return 0;
}
int MrqAxesPlan::updateData()
{
    if ( NULL == m_pMrqModel )
    { return ERR_NULL_POINTER; }

    return 0;
}
