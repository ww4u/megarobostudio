#include "mrqproperty.h"
#include "ui_mrqproperty.h"

#include "../../robot/robotfact.h"

mrqProperty::mrqProperty( VRobot *pMrqRobo,
                          QWidget *parent) :
    mrqView(parent),
    ui(new Ui::mrqProperty)
{
    //! setup
    ui->setupUi(this);

    Q_ASSERT( NULL != pMrqRobo );
    m_pRefModel = pMrqRobo;

    mFilePattern<<setup_desc<<setup_ext;

    setupUi();

    buildConnection( );

    //! three btns
    mbtnEnableSnap.append( false );
    mbtnEnableSnap.append( false );
    mbtnEnableSnap.append( false );
}

mrqProperty::~mrqProperty()
{
    desetupUi();
    logDbg();
}

void mrqProperty::slot_page_changed( int index )
{
    Q_ASSERT( index < ui->stackedWidget->count() );

    Q_ASSERT( NULL != ui->stackedWidget->widget(index) );

    modelView *pView = ((modelView*)ui->stackedWidget->widget(index));
    Q_ASSERT( NULL != pView );
    ui->btnCancel->setEnabled( pView->isCanceAble() );
    ui->btnOK->setEnabled( pView->isOkAble() );
    ui->btnApply->setEnabled( pView->isApplyAble() );
}

void mrqProperty::on_btnApply_clicked()
{
    setApply();

    slotModified( false );
}
void mrqProperty::on_btnOK_clicked()
{
    setApply();

    slotModified( false );

    emit sigClose( this );
}

void mrqProperty::on_btnCancel_clicked()
{
    emit sigClose( this );
}

void mrqProperty::showEvent(QShowEvent *event)
{
    Q_ASSERT( NULL != m_pModelObj );

    if ( m_pModelObj->isFilled() )
    {}
    else
    {}
}

void mrqProperty::setModelObj( mcModelObj *pObj )
{
    mrqView::setModelObj( pObj );

    //! \todo pobj memory leak

    //! foreach apply
    foreach( modelView *pView, mViewPages )
    {
        Q_ASSERT( NULL != pView );
        pView->setModelObj(pObj);
    }
}

int mrqProperty::save( QString &outFileName )
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

int  mrqProperty::saveAs( QString &outFileName )
{
    slotModified( false );

    return ((VRobot*)getModelObj())->save( outFileName );
}

void mrqProperty::setMcModel( mcModel *pMcModel )
{
    mrqView::setMcModel( pMcModel );

    //! foreach apply
    foreach( modelView *pView, mViewPages )
    {
        Q_ASSERT( NULL != pView );
        pView->setMcModel(pMcModel);
    }
}

int mrqProperty::setApply()
{
//    //! check device
//    MegaDevice::deviceMRQ *pDevice;
//    pDevice = getDevice();
//    if ( NULL == pDevice )
//    {
//        return ERR_INVALID_DEVICE_NAME;
//    }

//    //! foreach apply
//    int id = 0;
//    foreach( modelView *pView, mViewPages )
//    {
//        Q_ASSERT( NULL != pView );
//        sysProgress( id++, pView->name(), mViewPages.size(), 0 );
//        sysProgress( true );
//        pView->setApply();
//        sysProgress( id, pView->name(), mViewPages.size(), 0 );
//    }

//    sysProgress( false );

    post_request( msg_mrq_property_apply, mrqProperty, Apply );

    return 0;
}

void mrqProperty::updateScreen()
{
    for( int i = 0; i < ui->stackedWidget->count(); i++ )
    {
        Q_ASSERT( NULL != ui->stackedWidget->widget(i) );

        ((mrqView*)ui->stackedWidget->widget(i))->modelChanged();
    }
}

void mrqProperty::setupUi()
{
    //! new
    m_pInfoPage = new mrqInfo();
    Q_ASSERT( NULL != m_pInfoPage );
    mViewPages.append( m_pInfoPage );

    for ( int i = 0; i < m_pRefModel->axes(); i++ )
    {
        mMrqAxesPages.append( new MrqAxesPage() );
        Q_ASSERT( mMrqAxesPages.at(i) != NULL );
        mMrqAxesPages.at(i)->setAxesId( i );

        mViewPages.append( mMrqAxesPages.at(i) );
    }

    for ( int i = 0; i < m_pRefModel->dcAxes(); i++ )
    {
        mDcAxesPages.append( new MrqDcAxes() );
        Q_ASSERT( mDcAxesPages.at(i) != NULL );
        mDcAxesPages.at(i)->setAxesId( i );

        mViewPages.append( mDcAxesPages.at(i) );
    }

    m_pIoPage = new mrqIo();
    Q_ASSERT( NULL != m_pIoPage );
    mViewPages.append( m_pIoPage );

    m_pSensorPage = new mrqSensor();
    Q_ASSERT( NULL != m_pSensorPage );
    mViewPages.append( m_pSensorPage );

    if ( m_pRefModel->alarms() > 0 )
    {
        m_pAlarmPage = new MrqAlarm();
        Q_ASSERT( NULL != m_pAlarmPage );
        mViewPages.append( m_pAlarmPage );
    }

    m_pSysPage = new mrqSys();
    Q_ASSERT( NULL != m_pSysPage );
    mViewPages.append( m_pSysPage );

    //! add to page
    ui->stackedWidget->addWidget( m_pInfoPage );

    for ( int i = 0; i < m_pRefModel->axes(); i++ )
    {
        ui->stackedWidget->addWidget( mMrqAxesPages[i] );
    }

    for ( int i = 0; i < m_pRefModel->dcAxes(); i++ )
    {
        ui->stackedWidget->addWidget( mDcAxesPages[i] );
    }

    ui->stackedWidget->addWidget( m_pIoPage );
    ui->stackedWidget->addWidget( m_pSensorPage );

    if ( m_pRefModel->alarms() > 0 )
    {
        ui->stackedWidget->addWidget( m_pAlarmPage );
    }
    ui->stackedWidget->addWidget( m_pSysPage );

    //! list
    QListWidgetItem *pItem;
    pItem = new QListWidgetItem();
    Q_ASSERT( NULL != pItem );
    pItem->setText( tr("Info") );
    pItem->setIcon( QIcon(":/res/image/icon2/info.png") );
    ui->listWidget->addItem( pItem );

    QString str;
    QString strAxes = tr("Axes");
    for ( int i = 0; i < m_pRefModel->axes(); i++ )
    {
        //! axes 0
        str = strAxes + QString("%1").arg(i+1);
        pItem = new QListWidgetItem();
        Q_ASSERT( NULL != pItem );
        pItem->setText( str );
        pItem->setIcon( QIcon(":/res/image/icon2/focus.png") );
        ui->listWidget->addItem( pItem );
    }

    for ( int i = 0; i < m_pRefModel->dcAxes(); i++ )
    {
        //! dc axes
        str = QString("DC Axes%1 ").arg(i+1);
        pItem = new QListWidgetItem();
        Q_ASSERT( NULL != pItem );
        pItem->setText( str );
        pItem->setIcon( QIcon(":/res/image/icon2/focus.png") );
        ui->listWidget->addItem( pItem );
    }

    pItem = new QListWidgetItem();
    Q_ASSERT( NULL != pItem );
    pItem->setText( tr("IO") );
    pItem->setIcon( QIcon(":/res/image/icon2/link.png") );
    ui->listWidget->addItem( pItem );

    pItem = new QListWidgetItem();
    Q_ASSERT( NULL != pItem );
    pItem->setText( tr("Sensor") );
    pItem->setIcon( QIcon(":/res/image/icon2/pick.png") );
    ui->listWidget->addItem( pItem );

    if ( m_pRefModel->alarms() > 0 )
    {
        pItem = new QListWidgetItem();
        Q_ASSERT( NULL != pItem );
        pItem->setText( tr("Alarm") );
        pItem->setIcon( QIcon(":/res/image/icon2/settings_light.png") );
        ui->listWidget->addItem( pItem );
    }

    pItem = new QListWidgetItem();
    Q_ASSERT( NULL != pItem );
    pItem->setText( tr("System") );
    pItem->setIcon( QIcon(":/res/image/icon2/settings_light.png") );
    ui->listWidget->addItem( pItem );

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

void mrqProperty::desetupUi()
{
    delete ui;
}

void mrqProperty::buildConnection()
{
    connect( ui->listWidget, SIGNAL(currentRowChanged(int)),
                  ui->stackedWidget, SLOT(setCurrentIndex(int)));

    connect( ui->stackedWidget, SIGNAL(currentChanged(int)),
             this, SLOT(slot_page_changed(int)));
}

int mrqProperty::postApply( appMsg msg, void *pPara )
{
    //! check device
    MegaDevice::deviceMRQ *pDevice;
    pDevice = getDevice();
    if ( NULL == pDevice )
    {
        sysPrompt( tr("Invalid device") );
        return ERR_INVALID_DEVICE_NAME;
    }

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

    return 0;
}
void mrqProperty::beginApply( void *pPara)
{
    sysProgress( 0, tr("Begin apply") );
    sysProgress( true );

    saveBtnSnap();
}
void mrqProperty::endApply( int ret, void *pPara )
{
    sysProgress( false );

    restoreBtnSnap();
}

void mrqProperty::saveBtnSnap( bool bNow )
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
void mrqProperty::restoreBtnSnap()
{
    //! restore
    ui->btnApply->setEnabled( mbtnEnableSnap[0] );
    ui->btnOK->setEnabled( mbtnEnableSnap[1] );
    ui->btnCancel->setEnabled( mbtnEnableSnap[2] );
}
