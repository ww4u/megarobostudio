#include "mrvprop.h"
#include "megamessagebox.h"

#include "ui_mrqproperty.h"

MrvProp::MrvProp( VRobot *pMrqRobo,
                  QWidget *parent ) :
                mrvView(parent),
                ui(new Ui::mrqProperty)
{
    //! setup
    ui->setupUi(this);

    Q_ASSERT( NULL != pMrqRobo );
    m_pRefModel = pMrqRobo;

    mFilePattern<<setup_desc<<setup_ext;

    setupUi();

    buildConnection();
    //! three btns
    mbtnEnableSnap.append( false );
    mbtnEnableSnap.append( false );
    mbtnEnableSnap.append( false );
}

MrvProp::~MrvProp()
{
    delete ui;
}

void MrvProp::slot_page_changed( int index )
{
    Q_ASSERT( index < ui->stackedWidget->count() );

    Q_ASSERT( NULL != ui->stackedWidget->widget(index) );

    modelView *pView = ((modelView*)ui->stackedWidget->widget(index));
    Q_ASSERT( NULL != pView );
    ui->btnCancel->setEnabled( pView->isCanceAble() );
    ui->btnOK->setEnabled( pView->isOkAble() );
    ui->btnApply->setEnabled( pView->isApplyAble() );

    ui->btnCancel->setFocus();      //! focus on cancel
}

void MrvProp::on_btnApply_clicked()
{ post_request( msg_mrv_property_apply, MrvProp, Apply ); }
void MrvProp::on_btnOK_clicked()
{ post_request( msg_mrv_property_ok, MrvProp, Ok ); }
void MrvProp::on_btnCancel_clicked()
{ emit sigClose( this ); }
void MrvProp::on_btnReset_clicked()
{
    //! prompt to reset
    if ( QMessageBox::Ok !=
         MegaMessageBox::question( this, QString(tr("Confirm") ),
                                   QString( tr("Confirm to reset") ),
                                   QMessageBox::Ok, QMessageBox::Cancel ) )
    { return; }
}

void MrvProp::setModelObj( mcModelObj *pObj )
{
    //! \todo pobj memory leak

    //! foreach apply
    foreach( modelView *pView, mViewPages )
    {
        Q_ASSERT( NULL != pView );
        pView->setModelObj(pObj);
    }

    mrvView::setModelObj( pObj );
}

void MrvProp::setMcModel( mcModel *pMcModel )
{
    mrvView::setMcModel( pMcModel );

    //! foreach apply
    foreach( modelView *pView, mViewPages )
    {
        Q_ASSERT( NULL != pView );
        pView->setMcModel(pMcModel);
    }
}

void MrvProp::updateScreen()
{
    for( int i = 0; i < ui->stackedWidget->count(); i++ )
    {
        Q_ASSERT( NULL != ui->stackedWidget->widget(i) );

        ((mrvView*)ui->stackedWidget->widget(i))->modelChanged();
    }
}

int MrvProp::save( QString &outFileName )
{
    if ( getModelObj()->getPath().isEmpty() )
    {
        outFileName = QDir::currentPath() +
                      QDir::separator() + getModelObj()->getName();
    }
    else
    {
        outFileName = getModelObj()->getPath() +
                QDir::separator() + getModelObj()->getName();
    }
    outFileName = QDir::toNativeSeparators( outFileName );

    slotModified( false );

    return ((VRobot*)getModelObj())->save( outFileName + setup_d_ext );
}
int MrvProp::saveAs( QString &outFileName )
{
    slotModified( false );

    return ((VRobot*)getModelObj())->save( outFileName );
}

void MrvProp::setupUi()
{
    //! pages
    {
        mViewPages.append( new mrvInfo() );
        mTitles<<"Info";
        mIcons<<":/res/image/icon2/info.png";

        MrvCh *pMrvCh;
        for ( int i = 0; i < m_pRefModel->axes(); i++ )
        {
            pMrvCh = new MrvCh();
            Q_ASSERT( NULL != pMrvCh );
            mViewPages.append( pMrvCh );
            mTitles<<QString("CH%1").arg(i+1);
            mIcons<<":/res/image/icon2/focus.png";

            pMrvCh->setAxesId( i );
        }

        //! sys
        mViewPages.append( new MrvSystem() );
        mTitles<<QString("System");
        mIcons<<":/res/image/icon2/settings_light.png";
    }

    //! stack widget
    for ( int i =0; i < mViewPages.size(); i++ )
    { ui->stackedWidget->addWidget( mViewPages.at(i)); }

    //! list widget
    QListWidgetItem *pItem;
    for( int i = 0; i < mViewPages.size(); i++ )
    {
        pItem = new QListWidgetItem();
        Q_ASSERT( NULL != pItem );

        pItem->setText( mTitles.at(i) );
        pItem->setIcon( QIcon(mIcons.at(i)) );
        ui->listWidget->addItem( pItem );
    }

    //! post
    slot_page_changed( ui->stackedWidget->currentIndex() );

    //! modified
    modelView *pView;
    for ( int i = 0; i < ui->stackedWidget->count(); i++ )
    {
        pView = (modelView*)ui->stackedWidget->widget(i);
        connect( pView, SIGNAL(sigModified(bool)),
                 this,  SLOT(slotModified(bool)) );
    }
}

void MrvProp::buildConnection()
{
    connect( ui->listWidget, SIGNAL(currentRowChanged(int)),
             ui->stackedWidget, SLOT(setCurrentIndex(int)));

    connect( ui->stackedWidget, SIGNAL(currentChanged(int)),
             this, SLOT(slot_page_changed(int)));
}

int MrvProp::postApply( appMsg msg, void *pPara )
{
//    //! check device
//    MegaDevice::deviceMRQ *pDevice;
//    pDevice = getDevice();
//    if ( NULL == pDevice )
//    {
//        sysPrompt( tr("Invalid device") );
//        return ERR_INVALID_DEVICE_NAME;
//    }

    m_pMRV->preSet();

    //! foreach apply
    int id = 0;
    foreach( modelView *pView, mViewPages )
    {
        Q_ASSERT( NULL != pView );
        sysProgress( id++, pView->name(), mViewPages.size(), 0 );
        sysProgress( true );
        pView->setApply();

        sysProgress( id, pView->name(), mViewPages.size(), 0 );
    }

    m_pMRV->postSet();

    return 0;
}
void MrvProp::beginApply( void *pPara)
{
    sysProgress( 0, tr("Begin apply") );
    sysProgress( true );

    saveBtnSnap();
}
void MrvProp::endApply( int ret, void *pPara )
{
    sysProgress( false );

    slotModified( false );

    restoreBtnSnap();
}

int MrvProp::postOk( appMsg msg, void *pPara )
{
    return postApply( msg_mrv_property_apply, pPara );

}
void MrvProp::beginOk( void *pPara)
{
    beginApply( pPara );
}
void MrvProp::endOk( int ret, void *pPara )
{
    endApply( ret, pPara );

    slotModified( false );

    emit sigClose( this );
}

void MrvProp::saveBtnSnap( bool bNow )
{
    //! save
    mbtnEnableSnap[0] = ui->btnApply->isEnabled();
    mbtnEnableSnap[1] = ui->btnOK->isEnabled();
    mbtnEnableSnap[2] = ui->btnCancel->isEnabled();

    //! config
    ui->btnApply->setEnabled( bNow );
    ui->btnOK->setEnabled( bNow );
    ui->btnCancel->setEnabled( bNow );
}
void MrvProp::restoreBtnSnap()
{
    //! restore
    ui->btnApply->setEnabled( mbtnEnableSnap[0] );
    ui->btnOK->setEnabled( mbtnEnableSnap[1] );
    ui->btnCancel->setEnabled( mbtnEnableSnap[2] );
}


