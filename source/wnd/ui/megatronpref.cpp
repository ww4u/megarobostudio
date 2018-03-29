#include "megatronpref.h"
#include "ui_megatronpref.h"


MegatronPref::MegatronPref(QWidget *parent) :
    modelView(parent),
    ui(new Ui::megatronpref)
{
    ui->setupUi(this);

    //! edits
    mEdits.append( ui->lineEdit );
    mEdits.append( ui->lineEdit_2 );
    mEdits.append( ui->lineEdit_3 );
    mEdits.append( ui->lineEdit_4 );
    mEdits.append( ui->lineEdit_5 );
    mEdits.append( ui->lineEdit_6 );

    mLabels.append( ui->label );
    mLabels.append( ui->label_2 );
    mLabels.append( ui->label_3 );
    mLabels.append( ui->label_4 );
    mLabels.append( ui->label_5 );
    mLabels.append( ui->label_6 );
}

MegatronPref::~MegatronPref()
{
    delete ui;
}

void MegatronPref::setModelObj( mcModelObj *pObj )
{
    modelView::setModelObj( pObj );

    updateUi();
}

int MegatronPref::setApply()
{
    updateData();

    return 0;
}

void MegatronPref::updateData()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    pBase->mAxesConnectionName.clear();
    for ( int i = 0; i < pBase->axes(); i++ )
    {
        pBase->mAxesConnectionName<<mEdits[i]->text();
    }
}

void MegatronPref::updateUi()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    Q_ASSERT( pBase->axes() == mLabels.size() );
    for( int i = 0; i < pBase->axes(); i++ )
    { mLabels[i]->setText( pBase->mJointName.at(i) ); }

    Q_ASSERT( pBase->axes() == mEdits.size() );
    for( int i = 0; i < pBase->axes(); i++ )
    {
        mEdits[i]->setText( pBase->mAxesConnectionName[i] );
    }
}
