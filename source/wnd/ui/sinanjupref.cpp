#include "sinanjupref.h"
#include "ui_sinanjupref.h"
#include "../../robot/robotfact.h"
SinanjuPref::SinanjuPref(QWidget *parent) :
    modelView(parent),
    ui(new Ui::SinanjuPref)
{
    ui->setupUi(this);

    //! fill table
    int i = 0;
    mLabels[i] = ui->label_7;i++;
    mLabels[i] = ui->label_6;i++;
    mLabels[i] = ui->label_5;i++;
    mLabels[i] = ui->label_4;i++;
    mLabels[i] = ui->label_3;i++;

    i = 0;
    mEdits[i] = ui->lineEdit_6;i++;
    mEdits[i] = ui->lineEdit_5;i++;
    mEdits[i] = ui->lineEdit_4;i++;
    mEdits[i] = ui->lineEdit_3;i++;
    mEdits[i] = ui->lineEdit_2;i++;
}

SinanjuPref::~SinanjuPref()
{
    delete ui;
}

void SinanjuPref::setModelObj( mcModelObj *pObj )
{
    modelView::setModelObj( pObj );

    initModel();

    updateUi();
}

//! set group id
int SinanjuPref::setApply()
{
    updateData();

    return 0;
}

void SinanjuPref::updateData()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );
logDbg();
    pBase->mAxesConnectionName.clear();
    for ( int i = 0; i < pBase->axes(); i++ )
    {
        pBase->mAxesConnectionName<<mEdits[i]->text();
    }
logDbg();
//    pBase->setName( ui->edtAlias->text() );
//    pBase->setCanGroupId( ui->edtGroupCanID->text().toInt() );
//    pBase->setSubGroup( ui->cmbGroup->currentIndex() );
//    pBase->setSubGroupId( ui->cmbGpSubId->currentText().toInt() );
}

void SinanjuPref::updateUi()
{

}

void SinanjuPref::initModel()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    int i;
    for( i = 0; i < pBase->axes(); i++ )
    {
        mLabels[i]->setVisible( true );
        mEdits[i]->setVisible( true );

        mLabels[i]->setText( (pBase->mJointName.at(i)) );
        mEdits[i]->setText( (pBase->mAxesConnectionName.at(i)) );
    }

    //! other to false
    for ( ; i < 5; i++ )
    {
        mLabels[i]->setVisible( false );
        mEdits[i]->setVisible( false );
    }
//    ui->edtAlias->setText( pBase->getName() );

////    Q_ASSERT( NULL != m_pmcModel );
////    ui->edtGroupCanID->setText( QString::number( pBase->canGroupId() ) );
////    ui->cmbGroup->setCurrentIndex( pBase->subGroup() );
////    ui->cmbGpSubId->setCurrentText( QString::number( pBase->subGroupId() ) );

////    //! config range
////    m_pGroupIdValidaor->setRange( m_pmcModel->mSysPref.mGroupIdFrom,
////                                  m_pmcModel->mSysPref.mGroupIdTo );
}



