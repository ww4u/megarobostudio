#include "mrvprop.h"

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
//    //! three btns
//    mbtnEnableSnap.append( false );
//    mbtnEnableSnap.append( false );
//    mbtnEnableSnap.append( false );
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

void MrvProp::setupUi()
{
    //! pages
    {
        mViewPages.append( new mrvInfo() );
        mTitles<<"Info";
        mIcons<<":/res/image/icon2/info.png";

        for ( int i = 0; i < m_pRefModel->axes(); i++ )
        {
            mViewPages.append( new MrvCh() );
            mTitles<<QString("CH%1").arg(i+1);
            mIcons<<":/res/image/icon2/focus.png";

            ( (mrvView*)mViewPages.at( i ) )->setAxesId( i );
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


