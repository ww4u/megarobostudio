#include "deltaconfig.h"
#include "ui_deltaconfig.h"
#include "../../robot/delta/delta.h"

DeltaConfig::DeltaConfig(QWidget *parent) :
    modelView(parent),
    ui(new Ui::DeltaConfig)
{
    ui->setupUi(this);

    mSpinLens.append( ui->spinLen0 );
    mSpinLens.append( ui->spinLen1 );
    mSpinLens.append( ui->spinLen2 );
    mSpinLens.append( ui->spinLen3 );
    mSpinLens.append( ui->spinLen4 );

    mSpinRanges.append( ui->range0L );
    mSpinRanges.append( ui->range0H );
    mSpinRanges.append( ui->range1L );
    mSpinRanges.append( ui->range1H );
    mSpinRanges.append( ui->range2L );
    mSpinRanges.append( ui->range2H );

    mSpinP0s.append( ui->spinP00 );
    mSpinP0s.append( ui->spinP01 );
    mSpinP0s.append( ui->spinP02 );

    mSpinA0s.append( ui->spinA00 );
    mSpinA0s.append( ui->spinA01 );
    mSpinA0s.append( ui->spinA02 );

    spyEdited();
}

DeltaConfig::~DeltaConfig()
{
    delete ui;
}

void DeltaConfig::changeEvent( QEvent *event )
{
    modelView::changeEvent( event );

    if ( event->type() == QEvent::LanguageChange )
    { ui->retranslateUi( this ); }
}

void DeltaConfig::setModelObj( mcModelObj *pObj )
{
    modelView::setModelObj(pObj);

    updateUi();
}

int DeltaConfig::setApply()
{
    updateData();

    return 0;
}

void DeltaConfig::updateScreen()
{
    updateUi();
}
void DeltaConfig::spyEdited()
{
    QGroupBox *gpBox[]=
    {
    };
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
        ui->spinLen0,
        ui->spinLen1,
        ui->spinLen2,
        ui->spinLen3,
        ui->spinLen4,

        ui->range0L,
        ui->range0H,
        ui->range1L,
        ui->range1H,
        ui->range2L,
        ui->range2H,

        ui->spinP00,
        ui->spinP01,
        ui->spinP02,

        ui->spinA00,
        ui->spinA01,
        ui->spinA02,
    };

    QComboBox *comboxes[]={

    };

    install_spy();
}

void DeltaConfig::updateData()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    Q_ASSERT( pBase->mArmLengths.size() == mSpinLens.size() );
    for ( int i = 0; i < mSpinLens.size(); i++ )
    {
        exchange_container_spin( mSpinLens.at(i), pBase->mArmLengths[i], false );
    }

    robotDelta *pDelta = (robotDelta*)pBase;
    Q_ASSERT( pDelta->mAngleLimit.size() == mSpinRanges.size() );
    for ( int i = 0; i < mSpinLens.size(); i++ )
    {
        exchange_container_spin( mSpinRanges.at(i), pDelta->mAngleLimit[i], false );
    }

    Q_ASSERT( pDelta->mP0.size() == mSpinP0s.size() );
    for ( int i = 0; i < mSpinP0s.size(); i++ )
    {
        exchange_container_spin( mSpinP0s.at(i), pDelta->mP0[i], false );
    }

    Q_ASSERT( pDelta->mA0.size() == mSpinA0s.size() );
    for ( int i = 0; i < mSpinP0s.size(); i++ )
    {
        exchange_container_spin( mSpinA0s.at(i), pDelta->mA0[i], false );
    }
}
void DeltaConfig::updateUi()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    Q_ASSERT( pBase->mArmLengths.size() == mSpinLens.size() );
    for ( int i = 0; i < mSpinLens.size(); i++ )
    {
        exchange_container_spin( mSpinLens.at(i), pBase->mArmLengths[i], true );
    }

    robotDelta *pDelta = (robotDelta*)pBase;
    Q_ASSERT( pDelta->mAngleLimit.size() == mSpinRanges.size() );
    for ( int i = 0; i < mSpinLens.size(); i++ )
    {
        exchange_container_spin( mSpinRanges.at(i), pDelta->mAngleLimit[i], true );
    }

    Q_ASSERT( pDelta->mP0.size() == mSpinP0s.size() );
    for ( int i = 0; i < mSpinP0s.size(); i++ )
    {
        exchange_container_spin( mSpinP0s.at(i), pDelta->mP0[i], true );
    }

    Q_ASSERT( pDelta->mA0.size() == mSpinA0s.size() );
    for ( int i = 0; i < mSpinP0s.size(); i++ )
    {
        exchange_container_spin( mSpinA0s.at(i), pDelta->mA0[i], true );
    }
}
