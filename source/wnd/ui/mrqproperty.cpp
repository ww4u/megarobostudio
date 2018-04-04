#include "mrqproperty.h"
#include "ui_mrqproperty.h"

#include "../../robot/robotfact.h"

mrqProperty::mrqProperty( VRobot *pMrqRobo,
                          QWidget *parent) :
    mrqView(parent),
    ui(new Ui::mrqProperty)
{
    Q_ASSERT( NULL != pMrqRobo );
    m_pRefModel = pMrqRobo;

    mFilePattern<<setup_desc<<setup_ext;

    setupUi( );

    buildConnection( );
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
}
void mrqProperty::on_btnOK_clicked()
{
    setApply();

    emit sigClose( this );
}

void mrqProperty::on_btnCancel_clicked()
{
    emit sigClose( this );
}

void mrqProperty::setModelObj( mcModelObj *pObj )
{
    mrqView::setModelObj( pObj );

    //! \todo pobj memory leak

    //! foreach apply
    m_pInfoPage->setModelObj( pObj );

    for ( int i = 0; i < m_pRefModel->axes(); i++ )
    {
        mAxesPages[i]->setModelObj(pObj);
        mAxesPage2s[i]->setModelObj(pObj);
        mAxesPage3s[i]->setModelObj(pObj);
        mAxesPlans[i]->setModelObj(pObj);
    }

    for ( int i = 0; i < m_pRefModel->dcAxes(); i++ )
    {
        mDcAxesPages[i]->setModelObj(pObj);
    }

    m_pIoPage->setModelObj(pObj);
    m_pSensorPage->setModelObj(pObj);
    m_pSysPage->setModelObj(pObj);
}

int mrqProperty::save( QString &outFileName )
{
    if ( getModelObj()->getPath().isEmpty() )
    {
        outFileName = QDir::currentPath() +
                      "/" + getModelObj()->getName();
    }
    else
    {
        outFileName = getModelObj()->getPath() +
                "/" + getModelObj()->getName();
        logDbg()<<outFileName;
    }

    return ((VRobot*)getModelObj())->save( outFileName + setup_d_ext );
}

int  mrqProperty::saveAs( QString &outFileName )
{
    return ((VRobot*)getModelObj())->save( outFileName );
}

void mrqProperty::setMcModel( mcModel *pMcModel )
{
    mrqView::setMcModel( pMcModel );

    //! foreach apply
    m_pInfoPage->setMcModel( pMcModel );

    for ( int i = 0; i < m_pRefModel->axes(); i++)
    {
        mAxesPages[i]->setMcModel(pMcModel);
        mAxesPage2s[i]->setMcModel(pMcModel);
        mAxesPage3s[i]->setMcModel(pMcModel);
        mAxesPlans[i]->setMcModel( pMcModel );
    }

    for ( int i = 0; i < m_pRefModel->dcAxes(); i++)
    {
        mDcAxesPages[i]->setMcModel(pMcModel);
    }

    m_pIoPage->setMcModel(pMcModel);
    m_pSensorPage->setMcModel(pMcModel);
    m_pSysPage->setMcModel(pMcModel);
}

int mrqProperty::setApply()
{
    //! check device
    MegaDevice::deviceMRQ *pDevice;
    pDevice = getDevice();
    if ( NULL == pDevice )
    {
        return ERR_INVALID_DEVICE_NAME;
    }

    //! foreach apply
    for ( int i = 0; i < m_pRefModel->axes(); i++)
    {
        mAxesPages[i]->setApply();
        mAxesPage2s[i]->setApply();
        mAxesPage3s[i]->setApply();
        mAxesPlans[i]->setApply();
    }

    for ( int i = 0; i < m_pRefModel->dcAxes(); i++)
    {
        mDcAxesPages[i]->setApply();
    }

    m_pIoPage->setApply();
    m_pSensorPage->setApply();
    m_pSysPage->setApply();

    logDbg();

    return 0;
}

void mrqProperty::setupUi()
{
    //! new
    m_pInfoPage = new mrqInfo();

    for ( int i = 0; i < m_pRefModel->axes(); i++ )
    {
        mAxesPages.append( new mrqAxes() );
        mAxesPages[i]->setAxesId( i );

        mAxesPage2s.append( new mrqAxes2() );
        mAxesPage2s[i]->setAxesId( i );

        mAxesPage3s.append( new MrqTrigPage() );
        mAxesPage3s[i]->setAxesId( i );

        mAxesPlans.append( new MrqAxesPlan() );
        mAxesPlans[i]->setAxesId( i );
    }

    for ( int i = 0; i < m_pRefModel->dcAxes(); i++ )
    {
        mDcAxesPages.append( new MrqDcAxes() );
        mDcAxesPages[i]->setAxesId( i );
    }

    m_pIoPage = new mrqIo();
    m_pSensorPage = new mrqSensor();

    m_pAlarmPage = new MrqAlarm();
    m_pSysPage = new mrqSys();

    //! setup
    ui->setupUi(this);

    //! add to page
    ui->stackedWidget->addWidget( m_pInfoPage );

    for ( int i = 0; i < m_pRefModel->axes(); i++ )
    {
        ui->stackedWidget->addWidget( mAxesPages[i] );
        ui->stackedWidget->addWidget( mAxesPage2s[i] );
        ui->stackedWidget->addWidget( mAxesPage3s[i] );
        ui->stackedWidget->addWidget( mAxesPlans[i] );
    }

    for ( int i = 0; i < m_pRefModel->dcAxes(); i++ )
    {
        ui->stackedWidget->addWidget( mDcAxesPages[i] );
    }

    ui->stackedWidget->addWidget( m_pIoPage );
    ui->stackedWidget->addWidget( m_pSensorPage );

    ui->stackedWidget->addWidget( m_pAlarmPage );
    ui->stackedWidget->addWidget( m_pSysPage );

    //! list
    QListWidgetItem *pItem;
    pItem = new QListWidgetItem();
    pItem->setText( tr("Info") );
    pItem->setIcon( QIcon(":/res/image/icon2/info.png") );
    ui->listWidget->addItem( pItem );

    QString str;
    QString strAxes = tr("Axes");
    for ( int i = 0; i < m_pRefModel->axes(); i++ )
    {
        //! axes 0
        str = strAxes + QString("%1 1/4").arg(i+1);
        pItem = new QListWidgetItem();
        pItem->setText( str );
        pItem->setIcon( QIcon(":/res/image/icon2/focus.png") );
        ui->listWidget->addItem( pItem );

        //! axes 1
        str = strAxes + QString("%1 2/4").arg(i+1);
        pItem = new QListWidgetItem();
        pItem->setText( str );
        pItem->setIcon( QIcon(":/res/image/icon2/focus.png") );
        ui->listWidget->addItem( pItem );

        //! axes 2
        str = strAxes + QString("%1 3/4").arg(i+1);
        pItem = new QListWidgetItem();
        pItem->setText( str );
        pItem->setIcon( QIcon(":/res/image/icon2/focus.png") );
        ui->listWidget->addItem( pItem );

        //! axes 3
        str = strAxes + QString("%1 4/4").arg(i+1);
        pItem = new QListWidgetItem();
        pItem->setText( str );
        pItem->setIcon( QIcon(":/res/image/icon2/focus.png") );
        ui->listWidget->addItem( pItem );
    }

    for ( int i = 0; i < m_pRefModel->dcAxes(); i++ )
    {
        //! dc axes
        str = QString("DC Axes%1 ").arg(i+1);
        pItem = new QListWidgetItem();
        pItem->setText( str );
        pItem->setIcon( QIcon(":/res/image/icon2/focus.png") );
        ui->listWidget->addItem( pItem );
    }

    pItem = new QListWidgetItem();
    pItem->setText( tr("IO") );
    pItem->setIcon( QIcon(":/res/image/icon2/link.png") );
    ui->listWidget->addItem( pItem );

    pItem = new QListWidgetItem();
    pItem->setText( tr("Sensor") );
    pItem->setIcon( QIcon(":/res/image/icon2/pick.png") );
    ui->listWidget->addItem( pItem );

    pItem = new QListWidgetItem();
    pItem->setText( tr("Alarm") );
    pItem->setIcon( QIcon(":/res/image/icon2/settings_light.png") );
    ui->listWidget->addItem( pItem );

    pItem = new QListWidgetItem();
    pItem->setText( tr("System") );
    pItem->setIcon( QIcon(":/res/image/icon2/settings_light.png") );
    ui->listWidget->addItem( pItem );

    //! post
    slot_page_changed( ui->stackedWidget->currentIndex() );
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
