#include "devicemgr.h"
#include "ui_devicemgr.h"

#include "../../../include/mcstd.h"
#include "../../sys/sysapi.h"

#include "deviceconsole.h"

deviceMgr::deviceMgr(QWidget *parent) :
    modelView(parent),
    ui(new Ui::deviceMgr)
{
    init();

    ui->setupUi(this);
    ui->btnTest->hide();        //! debug

    setupUi();

    buildConnection();

}

deviceMgr::~deviceMgr()
{
    deinit();

    desetupUi();

    delete ui;
}

void deviceMgr::setMcModel( mcModel *pMcModel )
{
    modelView::setMcModel( pMcModel );

    if ( pMcModel->mSysPref.mbSearchOnOpen )
    { ui->pushButton->animateClick(); }
}

void deviceMgr::init()
{
    m_pDeviceMenu = NULL;

    m_pMgr = NULL;
    m_pDeviceDb = NULL;
    m_pPref = NULL;

    m_pRobo = NULL;
    m_pMRQ = NULL;

    m_pProgress = NULL;

    mAxesCount = 0;
}
void deviceMgr::deinit()
{
}

void deviceMgr::setupUi()
{
    //! device menu
    m_pDeviceMenu = new QMenu( this );

    m_pDeviceMenu->addAction( QIcon( ":/res/image/icon/219.png" ),
                              tr("Export setup..."),
                              this,
                              SLOT(context_export()) );
    m_pDeviceMenu->addAction( QIcon( ":/res/image/icon/218.png" ),
                              tr("Import setup..."),
                              this,
                              SLOT(context_import()) );
    m_pDeviceMenu->addSeparator();
    m_pDeviceMenu->addAction( QIcon( ":/res/image/icon/332.png" ),
                              tr("Upload from device"),
                              this,
                              SLOT(context_upload()) );
    m_pDeviceMenu->addSeparator();
    m_pDeviceMenu->addAction( QIcon( ":/res/image/icon/54.png" ),
                              tr("Alias..."),
                              this,
                              SLOT( context_mrq_alias() ) );
    m_pDeviceMenu->addSeparator();
    m_pDeviceMenu->addAction( QIcon( ":/res/image/icon/55.png" ),
                              tr("Console..."),
                              this,
                              SLOT( context_mrq_console() ) );

    //! robo menu
    m_pRoboMenu = new QMenu( this );
    m_pRoboMenu->addAction( QIcon( ":/res/image/icon/54.png" ),
                              tr("Alias..."),
                              this,
                              SLOT( context_robo_alias() ) );
    m_pRoboMenu->addSeparator();
    m_pRoboMenu->addAction( QIcon( ":/res/image/icon/55.png" ),
                              tr("Console..."),
                              this,
                              SLOT( context_robo_console() ) );
}

void deviceMgr::desetupUi()
{}

void deviceMgr::buildConnection()
{
    connect( ui->btnPanel, SIGNAL(clicked(bool)),
             this, SIGNAL(signal_btnState_clicked()));
}

//! fill info from the db
void deviceMgr::matchDevices()
{
    //! iter each device
//    MegaDevice::CANBus *pBus;
//    VRoboList *pDevices;
//    m_pMgr->getBusHub( &pBus, &pDevices );
}

void deviceMgr::updateData()
{
    //! remove the before
    ui->treeWidget->clear();

    //! add again
    DeviceTree devTree = m_pMgr->getDeviceTree();
    logDbg()<<devTree.size();

    foreach( VRoboList *pList, devTree )
    {logDbg();
        Q_ASSERT( NULL != pList );
        Q_ASSERT( NULL != pList->bus() );

        if ( is_phy_bus( pList->bus()->busType() ) )
        {
            updatePhyBusTree( pList );
        }
        else
        {logDbg();
            updateVirBusTree( pList );logDbg();
        }
    }
}

void deviceMgr::updatePhyBusTree( VRoboList *pRoboList )
{
    Q_ASSERT( NULL != pRoboList );

    //! bus
    QTreeWidgetItem *pItemBus;
    pItemBus = new QTreeWidgetItem( ui->treeWidget );
    Q_ASSERT( NULL != pItemBus );
    Q_ASSERT( pRoboList->bus() != NULL );
    pItemBus->setText( 0, pRoboList->bus()->name() );
    pItemBus->setIcon( 0, QIcon(":/res/image/icon2/order.png") );

    //! device
    QTreeWidgetItem *pItemDev;
    QTreeWidgetItem *pItemAxes;
    MegaDevice::MRQ_model *pMrqModel;

    //! robot
    foreach( VRobot *pDev, *pRoboList )
    {
        Q_ASSERT( NULL != pDev );

        pItemDev = new QTreeWidgetItem( pItemBus );
        Q_ASSERT( NULL != pItemDev );

        //! model
        pMrqModel = ((MegaDevice::deviceMRQ *)pDev)->getModel();
        Q_ASSERT( NULL != pMrqModel );
logDbg()<<QString::number( (quint32)pMrqModel, 16 );
        pItemDev->setText( 0, pMrqModel->getFullDesc() );
        pItemDev->setToolTip( 0, pDev->name() );

        //! obj type
        pDev->setType( mcModelObj::model_device );

        pItemDev->setData( 0, Qt::UserRole, QVariant::fromValue(pDev) );
        pItemDev->setIcon( 0, QIcon(":/res/image/icon2/mobile.png") );

        //! foreach axes
        QString strName;
        for ( int i = 0; i < pDev->axes(); i++ )
        {
            pItemAxes = new QTreeWidgetItem( pItemDev );
            Q_ASSERT( NULL != pItemAxes );

            strName = QString("CH%1").arg( i + 1 );
            pItemAxes->setText( 0, strName );
            pItemAxes->setToolTip( 0, strName );

            pItemAxes->setIcon( 0, QIcon(":/res/image/icon2/focus.png") );

            mAxesCount++;
        }
    }

}

void deviceMgr::updateVirBusTree( VRoboList *pRoboList )
{
    Q_ASSERT( NULL != pRoboList );

    //! bus
    QTreeWidgetItem *pItemBus;
    pItemBus = new QTreeWidgetItem( ui->treeWidget );
    Q_ASSERT( NULL != pItemBus );
    Q_ASSERT( pRoboList->bus() != NULL );

    pItemBus->setText( 0, pRoboList->bus()->name() );
    pItemBus->setIcon( 0, QIcon(":/res/image/icon2/circle.png") );

    //! device
    QTreeWidgetItem *pItemDev;

    //! robot
    foreach( VRobot *pDev, *pRoboList )
    {
        Q_ASSERT( NULL != pDev );

        pItemDev = new QTreeWidgetItem( pItemBus );
        Q_ASSERT( NULL != pItemDev );

        pItemDev->setText( 0, pDev->name() );
        pItemDev->setToolTip( 0, pDev->name() + "-" + pDev->getClass() );

        //! obj type
        pDev->setType( mcModelObj::model_device );

        pItemDev->setData( 0, Qt::UserRole, QVariant::fromValue(pDev) );
        pItemDev->setIcon( 0, QIcon( QPixmap::fromImage( pDev->getImage() ) ) );
    }
}

int deviceMgr::postSearchDevice( appMsg msg, void *pPara )
{
    Q_ASSERT( NULL != m_pMgr );

    m_pMgr->probeBus();

    return 0;
}

void deviceMgr::beginSearchDevice( void *pPara )
{
    ui->pushButton->setEnabled( false );
    mAxesCount = 0;

    emit signal_instmgr_changed( false, m_pMgr );

    sysLog( tr("begin search") );
    sysProgress( 0, tr("begin search") );
    sysProgress( true );
}
void deviceMgr::endSearchDevice( int ret, void *pPara )
{
    ui->pushButton->setEnabled( true );

    sysLog( tr("end search") );
    sysProgress( false );
    if ( ret != 0 )
    { return; }

    updateUi();

    //! debug
//    if ( mAxesCount == 0 || mAxesCount == 15 )
//    {}
//    else
//    { logDbg()<<mAxesCount; Q_ASSERT(false); }
//    ui->pushButton->animateClick();
}

int deviceMgr::postLoadOn( appMsg msg, void *pPara )
{
    Q_ASSERT( m_pMRQ != NULL );

    int ret = m_pMRQ->uploadSetting();

    return ret;
//    return 0;
}
void deviceMgr::beginLoadOn( void *pPara )
{
    emit signalReport( 0, tr("start upload") );

//    if ( m_pProgress == NULL )
//    {
//        m_pProgress = new QProgressDialog(this);
//        Q_ASSERT( NULL != m_pProgress );
//        m_pProgress->setWindowTitle( tr("Progress") );
//    }

//    m_pProgress->show();
}
void deviceMgr::endLoadOn( int ret, void *pPara )
{
//    if ( m_pProgress )
//    {
//        m_pProgress->hide();
//        delete m_pProgress;
//        m_pProgress = NULL;
//    }

    if ( ret != 0 )
    {
        emit signalReport( ret, tr("upload fail") );
        sysError( tr("upload fail") );
    }
    else
    {
        emit signalReport( ret, tr("success") );
        sysLog( tr("upload success") );
    }
}

int deviceMgr::doTest( appMsg msg, void *pPara )
{
    logDbg()<<QString::number( (quint32)pPara, 16 );
    return msg;
}

void deviceMgr::on_btnTest_clicked()
{
//    post_request( msg_mgr_motor_test, deviceMgr, MotorTest );
    post_request( msg_mgr_motor_test, deviceMgr, RoboTest );
//    execRoboTest();
}

void deviceMgr::on_pushButton_clicked()
{
    post_request( msg_search_device, deviceMgr, SearchDevice );
}

void deviceMgr::on_treeWidget_itemActivated(QTreeWidgetItem *item, int column)
{
    if ( item == 0 || column < 0 )
    { return; }

    QVariant var = item->data( 0, Qt::UserRole );

    VRobot *pRobot = var.value<VRobot*>();
    if ( NULL != pRobot )
    {
        emit itemXActivated( (mcModelObj*)pRobot );
    }
}

//! quick
void deviceMgr::context_import()
{
    QFileDialog fDlg;

    fDlg.setAcceptMode( QFileDialog::AcceptOpen );
    fDlg.setNameFilter( tr("Device setup (*.stp)") );
    if ( QDialog::Accepted != fDlg.exec() )
    { return; }

    Q_ASSERT( m_pMRQ != NULL );
    m_pMRQ->getModel()->load( fDlg.selectedFiles().first() );

    sysLog( fDlg.selectedFiles().first(), tr("load success") );
}
void deviceMgr::context_export()
{
    QFileDialog fDlg;

    fDlg.setAcceptMode( QFileDialog::AcceptSave );
    fDlg.setNameFilter( tr("Device setup (*.stp)") );
    fDlg.selectFile( m_pMRQ->name() + ".stp" );
    if ( QDialog::Accepted != fDlg.exec() )
    { return; }

    Q_ASSERT( m_pMRQ != NULL );
    m_pMRQ->getModel()->save( fDlg.selectedFiles().first() );

    sysLog( fDlg.selectedFiles().first(), tr("save success") );
}

void deviceMgr::context_upload()
{
    post_request( msg_upload_device, deviceMgr, LoadOn );
}

void deviceMgr::context_mrq_alias()
{
    Q_ASSERT( NULL != m_pMRQ );
    Q_ASSERT( NULL != m_pCurrTreeItem );

    bool ok;
    QString text = QInputDialog::getText(this,
                                         tr("Alias"),
                                         tr("Alias"),
                                         QLineEdit::Normal,
                                         m_pMRQ->name(),
                                         &ok);
    if (ok && !text.isEmpty())
    {
        m_pMRQ->setName( text );        //! must be override
        m_pCurrTreeItem->setText( 0, m_pMRQ->getModel()->getFullDesc( ) );
        m_pCurrTreeItem->setToolTip( 0, m_pMRQ->getModel()->name() );
    }
}

void deviceMgr::context_robo_alias()
{
    Q_ASSERT( NULL != m_pRobo );
    Q_ASSERT( NULL != m_pCurrTreeItem );

    bool ok;
    QString text = QInputDialog::getText(this,
                                         tr("Alias"),
                                         tr("Alias"),
                                         QLineEdit::Normal,
                                         m_pRobo->name(),
                                         &ok);
    if (ok && !text.isEmpty())
    {
        m_pRobo->setName( text );
        m_pCurrTreeItem->setText( 0, text );
        m_pCurrTreeItem->setToolTip( 0, text );
    }
}

void deviceMgr::context_mrq_console()
{
    deviceConsole *pConsole;

    pConsole = new deviceConsole(this);
    if ( NULL == pConsole )
    { return; }

    pConsole->setMrq( true );

    //! title name
    pConsole->setWindowTitle( m_pMRQ->getModel()->getFullDesc( mCurrentAxes ) );
    pConsole->setShell( m_pMRQ );

    pConsole->show();
}

void deviceMgr::context_robo_console()
{
    deviceConsole *pConsole;

    pConsole = new deviceConsole(this);
    if ( NULL == pConsole )
    { return; }

    pConsole->setMrq( false );

    Q_ASSERT( NULL != m_pRobo );

    //! title name
    pConsole->setWindowTitle( m_pRobo->name() );
    pConsole->setShell( m_pRobo );

    pConsole->show();
}

void deviceMgr::contextMenuEvent(QContextMenuEvent *event)
{
    QPoint pt = event->pos();

    //! to parent
    pt = ui->treeWidget->mapFrom( this, event->pos() );

    QTreeWidgetItem * pItem = ui->treeWidget->itemAt( pt );

    //! no item
    if ( pItem == NULL )
    { return; }

    //! get pointer
    VRobot *pObj;
    QVariant var;
    var = pItem->data( 0, Qt::UserRole );
    pObj = var.value<VRobot *>();
    if ( NULL == pItem || NULL == pObj )
    { return; }

    //! current device
    if ( pObj->Type() == mcModelObj::model_device )
    {
        m_pRobo = ( pObj );
        m_pCurrTreeItem = pItem;
        mCurrentAxes = -1;

        //! check robot or mrq
        if ( robot_is_mrq( m_pRobo->getId() ) )
        {
            m_pMRQ = (MegaDevice::deviceMRQ*)( m_pRobo );

            m_pDeviceMenu->popup( mapToGlobal( event->pos() ) );
            event->accept();
        }
        //! robot
        else if ( robot_is_robot( m_pRobo->getId() ) )
        {
            m_pRoboMenu->popup( mapToGlobal( event->pos() ) );
            event->accept();
        }
        else
        {
            m_pMRQ = NULL;
        }
    }
    else if ( pObj->Type() == mcModelObj::model_tpv )
    {
    }
    else
    {}
}

void deviceMgr::setInstMgr( MegaDevice::InstMgr *pMgr )
{
    Q_ASSERT( NULL != pMgr );

    m_pMgr = pMgr;
}

void deviceMgr::setDeviceDb( deviceDb *pDb )
{
    Q_ASSERT( NULL != pDb );
    m_pDeviceDb = pDb;
}

void deviceMgr::setSysPref( modelSysPref *pref )
{
    Q_ASSERT( NULL != pref );

    m_pPref = pref;
}

void deviceMgr::updateUi()
{
    matchDevices();

    updateData();

    ui->treeWidget->expandAll();

    emit signal_instmgr_changed( true, m_pMgr );
}


