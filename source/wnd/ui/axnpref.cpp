#include "axnpref.h"
#include "ui_axnpref.h"

AxnPref::AxnPref( QWidget *parent) :
    modelView(parent),
    ui(new Ui::AxnPref)
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

AxnPref::~AxnPref()
{
    delete ui;
}

void AxnPref::setModelObj( mcModelObj *pObj )
{
    modelView::setModelObj( pObj );

    updateUi();
}

int AxnPref::setApply()
{
    updateData();

    return 0;
}

void AxnPref::updateScreen()
{
    updateUi();
}

void AxnPref::updateData()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    //! udpate data
    pBase->mAxesConnectionName.clear();
    for ( int i = 0; i < pBase->axes(); i++ )
    {
        pBase->mAxesConnectionName<<mEdits[i]->text();
    }
}

void AxnPref::updateUi()
{
    Q_ASSERT( m_pModelObj != NULL );
    VRobot *pBase = ( VRobot *)m_pModelObj;
    Q_ASSERT( NULL != pBase );

    //! visible
    for ( int i = pBase->axes(); i < mEdits.size(); i++ )
    {
        mEdits[i]->setVisible( false );
        mLabels[i]->setVisible( false );
    }

    //! set data
    Q_ASSERT( pBase->axes() <= mLabels.size() );
    for( int i = 0; i < pBase->axes(); i++ )
    { mLabels[i]->setText( pBase->mJointName.at(i) ); }

    Q_ASSERT( pBase->axes() <= mEdits.size() );
    for( int i = 0; i < pBase->axes(); i++ )
    {
        mEdits[i]->setText( pBase->mAxesConnectionName[i] );
    }
}
