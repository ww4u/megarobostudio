#include "h2pref.h"
#include "ui_h2pref.h"

H2Pref::H2Pref(QWidget *parent) :
    modelView(parent),
    ui(new Ui::H2Pref)
{
    ui->setupUi(this);

    //! edits
    mEdits.append( ui->lineEdit );
    mEdits.append( ui->lineEdit_2 );
    mEdits.append( ui->lineEdit_3 );

    mLabels.append( ui->label );
    mLabels.append( ui->label_2 );
    mLabels.append( ui->label_3 );
}

H2Pref::~H2Pref()
{
    delete ui;
}

void H2Pref::setModelObj( mcModelObj *pObj )
{
    modelView::setModelObj( pObj );

    updateUi();
}

int H2Pref::setApply()
{
    updateData();

    return 0;
}

void H2Pref::updateData()
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

void H2Pref::updateUi()
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
