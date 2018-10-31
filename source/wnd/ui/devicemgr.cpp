#include "devicemgr.h"
#include "ui_devicemgr.h"

#include "../../../include/mcstd.h"
#include "../../sys/sysapi.h"
#include "../widget/megamessagebox.h"

#include "deviceconsole.h"

#include "axesknob.h"
#include "roboaxes.h"

//! panel createor
#include "t4panel.h"
#include "h2panel.h"
#include "h2zpanel.h"
#include "igusdrylinpanel.h"

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

QSize deviceMgr::sizeHint() const
{ return QSize(200,0);}

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
    m_pDiagram = NULL;
    mAxesCount = 0;
}
void deviceMgr::deinit()
{
}

void deviceMgr::setupUi()
{
    //! device menu
    m_pDeviceMenu = new QMenu( this );
    Q_ASSERT( NULL != m_pDeviceMenu );

    m_pDeviceMenu->addAction( QIcon( ":/res/image/icon/219.png" ),
                              tr("Export setup..."),
                              this,
                              SLOT(context_export()) );
    m_pDeviceImportAction = m_pDeviceMenu->addAction( QIcon( ":/res/image/icon/218.png" ),
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
    m_pDeviceMenu->addSeparator();
    m_pDeviceMenu->addAction( QIcon( ":/res/image/icon/205.png" ),
                              tr("Prop..."),
                              this,
                              SLOT( context_mrq_prop() ) );

    //! mrv menu
    m_pMRVMenu = new QMenu( this );
    Q_ASSERT( NULL != m_pMRVMenu );

    m_pMRVMenu->addAction( QIcon( ":/res/image/icon/219.png" ),
                              tr("Export setup..."),
                              this,
                              SLOT(context_mrv_export()) );
    /*m_pDeviceImportAction =*/ m_pMRVMenu->addAction( QIcon( ":/res/image/icon/218.png" ),
                              tr("Import setup..."),
                              this,
                              SLOT(context_mrv_import()) );
    m_pMRVMenu->addSeparator();
    m_pMRVMenu->addAction( QIcon( ":/res/image/icon/332.png" ),
                              tr("Upload from device"),
                              this,
                              SLOT(context_mrv_upload()) );
    m_pMRVMenu->addSeparator();
    m_pMRVMenu->addAction( QIcon( ":/res/image/icon/54.png" ),
                              tr("Alias..."),
                              this,
                              SLOT( context_mrv_alias() ) );
    m_pMRVMenu->addSeparator();
    m_pMRVMenu->addAction( QIcon( ":/res/image/icon/55.png" ),
                              tr("Console..."),
                              this,
                              SLOT( context_mrv_console() ) );
    m_pMRVMenu->addSeparator();
    m_pMRVMenu->addAction( QIcon( ":/res/image/icon/205.png" ),
                              tr("Prop..."),
                              this,
                              SLOT( context_mrv_prop() ) );

    //! axes menu
    m_pAxesMenu = new QMenu(this);
    m_pAxesMenu->addAction( QIcon( ":/res/image/icon2/mobile.png" ),
                              tr("Panel..."),
                              this,
                              SLOT( context_mrq_panel() ) );


    //! robo menu
    m_pRoboMenu = new QMenu( this );
//    m_pRoboMenu->addAction( QIcon( ":/res/image/icon/54.png" ),
//                              tr("Alias..."),
//                              this,
//                              SLOT( context_robo_alias() ) );
//    m_pRoboMenu->addSeparator();
    m_pRoboMenu->addAction( QIcon( ":/res/image/icon/55.png" ),
                              tr("Console..."),
                              this,
                              SLOT( context_robo_console() ) );
    m_pRoboMenu->addSeparator();
    m_pRoboJoiontAction = m_pRoboMenu->addAction( QIcon( ":/res/image/icon2/circle.png" ),
                              tr("Joint..."),
                              this,
                              SLOT( context_robo_axes() ) );
    m_pRoboPanelAction = m_pRoboMenu->addAction( QIcon( ":/res/image/icon2/mobile.png" ),
                              tr("Panel..."),
                              this,
                              SLOT( context_robo_panel() ) );
    m_pRoboMenu->addAction( QIcon( ":/res/image/icon/205.png" ),
                              tr("Prop..."),
                              this,
                              SLOT( context_robo_prop() ) );

    //! hub menu
    m_pHubMenu = new QMenu( this );
    m_pHubMenu->addAction( QIcon( ":/res/image/icon/54.png" ),
                              tr("Alias..."),
                              this,
                              SLOT( context_hub_alias() ) );
    m_pHubMenu->addSeparator();
    m_pHubMenu->addAction( QIcon( ":/res/image/icon/55.png" ),
                              tr("Console..."),
                              this,
                              SLOT( context_hub_console() ) );

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
}

void deviceMgr::updateData()
{
    //! remove the before
    ui->treeWidget->clear();

    //! add again
    DeviceTree devTree = m_pMgr->getDeviceTree();
    logDbg()<<devTree.size();

    foreach( VRoboList *pList, devTree )
    {
        Q_ASSERT( NULL != pList );
        Q_ASSERT( NULL != pList->bus() );

        if ( is_phy_bus( pList->bus()->busType() ) )
        {
            updatePhyBusTree( pList );
        }
        else
        {
            updateVirBusTree( pList );
        }
    }
}

void deviceMgr::updatePhyBusTree( VRoboList *pRoboList )
{
    Q_ASSERT( NULL != pRoboList );

    QString toolTips;

    //! bus
    QTreeWidgetItem *pItemBus;
    pItemBus = new QTreeWidgetItem( ui->treeWidget );
    Q_ASSERT( NULL != pItemBus );
    Q_ASSERT( pRoboList->bus() != NULL );
    pItemBus->setText( 0, pRoboList->bus()->name() );

    QStringList strList,typeList;
    strList<<":/res/image/megacan.png"
           <<":/res/image/can2.png"
           <<":/res/image/mrh-t.png"
           <<":/res/image/miniusbcan.png"
           <<":/res/image/mcp251x.png"
           <<":/res/image/rs232.png"
           <<":/res/image/usb.png";
    typeList<<"MRH-E"
           <<"USBCAN-II"
           <<"MRH-T"
           <<"MRH-U"
           <<"MCP"
           <<"RS232"
           <<"USB";
    int pId = pRoboList->bus()->pId();
    if (  pId >=0 && pId < strList.size() )
    {
        pItemBus->setIcon( 0, QIcon( strList.at(pId) ) );
        pItemBus->setToolTip( 0, QString("%1(%2)").arg( typeList.at(pId)).arg( pRoboList->bus()->devId() ) );
    }
    else
    { pItemBus->setIcon( 0, QIcon(":/res/image/icon2/order.png") ); }

    toolTips= QString("DeviceId:%1").arg( pRoboList->bus()->devId() );
    pItemBus->setToolTip( 0, toolTips );

    //! device
    QTreeWidgetItem *pItemDev;
    QTreeWidgetItem *pItemAxes;

    //! robot
    QList<int> ids;
    foreach( VRobot *pDev, *pRoboList )
    {
        Q_ASSERT( NULL != pDev );

        //! dev
        pItemDev = new QTreeWidgetItem( pItemBus );
        Q_ASSERT( NULL != pItemDev );

        pItemDev->setText( 0, pDev->deviceFullDesc() );

        if ( pDev->getType() == mcModelObj::model_device )
        {
            ids = pDev->deviceIds();
            Q_ASSERT(ids.size()>=4);

            toolTips= QString("S:0X%1/R:0X%2/G1:0X%3/G2:0X%4")
                    .arg( ids[0], 0, 16 )
                    .arg( ids[1], 0, 16 )
                    .arg( ids[2], 0, 16 )
                    .arg( ids[3], 0, 16 )
                    .toUpper();

            pItemDev->setToolTip( 0, toolTips );

            //! obj type
            pItemDev->setData( 0, Qt::UserRole, QVariant::fromValue(pDev) );
            pItemDev->setIcon( 0, QIcon( QPixmap::fromImage( pDev->getImage() ) ) );

            //! foreach axes
            QString strName;
            QString strToolTip;

            for ( int i = 0; i < pDev->axes(); i++ )
            {
                pItemAxes = new QTreeWidgetItem( pItemDev );
                Q_ASSERT( NULL != pItemAxes );

                QList<int> subIds  = pDev->subIDs( i );

                strName = QString("CH%1").arg( i + 1 );
                pItemAxes->setText( 0, strName );

                strToolTip = subGroupString( subIds ).toUpper();

                pItemAxes->setToolTip( 0, strToolTip );
                pItemAxes->setData( 0, Qt::UserRole, QVariant(i) );     //! type is int,and axes

                pItemAxes->setIcon( 0, QIcon(":/res/image/icon2/focus.png") );

                mAxesCount++;
            }
        }
        else if ( pDev->getType() == mcModelObj::model_hub )
        {
            pItemDev->setData( 0, Qt::UserRole, QVariant::fromValue(pDev) );
            pItemDev->setIcon( 0,
                               QIcon( QPixmap::fromImage( pDev->getImage() ) )
                               );
        }
        else
        {

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
    QString toolTip;
    foreach( VRobot *pDev, *pRoboList )
    {
        Q_ASSERT( NULL != pDev );

        pItemDev = new QTreeWidgetItem( pItemBus );
        Q_ASSERT( NULL != pItemDev );

        pItemDev->setText( 0, pDev->name() );
        toolTip = QString("G:0X%1/%2").arg( pDev->canGroupId(), 0, 16 )
                                    .arg( subGroupString( pDev->subGroup(), pDev->subGroupId() ) );
        toolTip = toolTip.toUpper();
        pItemDev->setToolTip(0, toolTip);

        //! obj type
        pDev->setType( mcModelObj::model_composite_device );

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

    ui->chkOnLine->setEnabled( false );
    ui->chkOnLine->setChecked( false );
    mAxesCount = 0;

    emit signal_instmgr_changed( false, m_pMgr );

    sysLog( tr("begin search") );
    sysProgress( 0, tr("begin search") );
    sysProgress( true );
}
void deviceMgr::endSearchDevice( int ret, void *pPara )
{
    ui->pushButton->setEnabled( true );
    sysOpc( 1 );

    sysLog( tr("end search") );
    sysProgress( false );
    if ( ret != 0 )
    { return; }

    updateUi();

    ui->chkOnLine->setEnabled( true );
    ui->chkOnLine->setChecked( true );
}

int deviceMgr::postLoadOn( appMsg msg, void *pPara )
{
    Q_ASSERT( m_pRobo != NULL );

    int ret = m_pRobo->uploadSetting();

    return ret;
}
void deviceMgr::beginLoadOn( void *pPara )
{
    ui->pushButton->setEnabled( false );

    sysProgress( 0, tr("Begin load") );
    sysProgress( true );

    emit signalReport( 0, tr("start upload") );
}
void deviceMgr::endLoadOn( int ret, void *pPara )
{
    ui->pushButton->setEnabled( true );

    sysProgress( false );

    if ( ret != 0 )
    {
        emit signalReport( ret, tr("upload fail") );
        sysError( tr("upload fail") );
    }
    else
    {
        emit signalReport( ret, tr("success") );
        sysLog( tr("upload success") );

        Q_ASSERT( NULL != m_pRobo );
        emit signalModelUpdated( m_pRobo );
    }
}

int deviceMgr::postImport( appMsg msg, void *pPara )
{
    Q_ASSERT( m_pRobo != NULL );
    int ret = m_pRobo->load( mImportFileName );
    if ( ret != 0 )
    {
        sysError(tr("load fail"));
        return -1;
    }
    emit signalModelUpdated( m_pRobo );
    sysLog( mImportFileName, tr("load success") );

    ret = m_pRobo->applySetting();
    if ( ret != 0 )
    {
        sysError(tr("apply fail"));
        return -1;
    }
    sysLog( mImportFileName, tr("apply success") );
    m_pRobo->setFilled( true );

    return 0;
}

void deviceMgr::beginImport( void *pPara )
{
    ui->pushButton->setEnabled( false );

    Q_ASSERT( NULL != m_pDeviceImportAction );
    m_pDeviceImportAction->setEnabled( false );
}
void deviceMgr::endImport( int ret, void *pPara )
{
    ui->pushButton->setEnabled( true );

    if ( ret != 0 )
    { sysError( tr("Import fail") ); }

    Q_ASSERT( NULL != m_pDeviceImportAction );
    m_pDeviceImportAction->setEnabled( true );
}

int deviceMgr::doTest( appMsg msg, void *pPara )
{
    return msg;
}

void deviceMgr::on_btnTest_clicked()
{
    post_request( msg_mgr_motor_test, deviceMgr, RoboTest );
}

void deviceMgr::on_pushButton_clicked()
{
    sysOpc( 0 );
    post_request( msg_search_device, deviceMgr, SearchDevice );
}

void deviceMgr::on_treeWidget_itemActivated(QTreeWidgetItem *item, int column)
{
    if ( item == 0 || column < 0 )
    { return; }

    QVariant var;
    VRobot *pObj;
    //! robo node
    if ( item->data(0,Qt::UserRole).type() == (QVariant::Type)QMetaType::User )
    {
        var = item->data( 0, Qt::UserRole );
        VRobot *pRobot = var.value<VRobot*>();

        //! default to panel
        if ( pRobot->roboPanelAble() )
        {
            m_pRobo = pRobot;
            mCurrNodeName = item->parent()->text(0);

            context_robo_panel();
        }
        else
        {
            //! try load setup?
            pRobot->tryLoad();

            emit itemXActivated( (mcModelObj*)pRobot );
        }
    }
    //! mrq axes
    else if ( item->data(0,Qt::UserRole).type() == (QVariant::Type)QMetaType::Int )
    {
        Q_ASSERT( item->parent() != NULL );
        //! get parent
        var = item->parent()->data(0, Qt::UserRole);
        pObj = var.value<VRobot *>();
        if ( NULL == item || NULL == pObj )
        { return; }

        //! panel
        m_pRobo = ( pObj );
        m_pMRQ = (MegaDevice::deviceMRQ*)( m_pRobo );
        mCurrentAxes = item->data(0,Qt::UserRole).toInt();

        context_mrq_panel();
    }
    else
    {}
}

//! quick
void deviceMgr::context_import()
{
    QFileDialog fDlg;

    fDlg.setAcceptMode( QFileDialog::AcceptOpen );
    fDlg.setNameFilter( tr("Device setup (*.stp)") );
    if ( QDialog::Accepted != fDlg.exec() )
    { return; }

    mImportFileName = fDlg.selectedFiles().first();
    post_request( msg_import_device, deviceMgr, Import );
}
void deviceMgr::context_export()
{
    Q_ASSERT( NULL != m_pRobo );

    if ( !m_pRobo->isFilled() &&
         QMessageBox::Cancel == MegaMessageBox::question(this,
                                 tr("question"),
                                 tr("Device setting has not been updated, sure to export?"),
                                 QMessageBox::Ok, QMessageBox::Cancel) )
    { return; }

    QFileDialog fDlg;

    fDlg.setAcceptMode( QFileDialog::AcceptSave );
    fDlg.setNameFilter( tr("Device setup (*.stp)") );
    fDlg.selectFile( m_pRobo->name() + ".stp" );
    if ( QDialog::Accepted != fDlg.exec() )
    { return; }

    m_pRobo->save( fDlg.selectedFiles().first() );

    sysLog( fDlg.selectedFiles().first(), tr("save success") );
}

void deviceMgr::context_upload()
{
    post_request( msg_upload_device, deviceMgr, LoadOn );
}

void deviceMgr::context_mrq_alias()
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
        m_pRobo->setName( text );        //! must be override
        m_pCurrTreeItem->setText( 0, m_pRobo->getFullDesc( ) );

        Q_ASSERT( NULL != m_pRobo );
        emit signalModelUpdated( m_pRobo );
    }
}

void deviceMgr::context_mrq_console()
{
    deviceConsole *pConsole;

    pConsole = new deviceConsole(this);
    if ( NULL == pConsole )
    { return; }

    connect( this, SIGNAL(signal_instmgr_changed(bool,MegaDevice::InstMgr*)),
             pConsole, SLOT(slot_device_changed()));

    pConsole->setMrq( true );
    pConsole->setClassName( "mrq" );

    //! title name
    pConsole->setWindowTitle( m_pRobo->getFullDesc( mCurrentAxes ) );
    pConsole->setShell( m_pRobo );

    pConsole->show();
}

void deviceMgr::context_mrq_panel()
{
    Q_ASSERT( NULL != m_pMRQ );
    Q_ASSERT( NULL != m_pmcModel );

    axesKnob *pKnob = new axesKnob( m_pmcModel,
                                    QString("CH%1@%2").arg(mCurrentAxes+1).arg(m_pMRQ->name() ),
                                    this );
    if ( NULL == pKnob )
    { return; }

    connect( this, SIGNAL(signal_instmgr_changed(bool,MegaDevice::InstMgr*)),
             pKnob, SLOT(slot_device_changed()));

    pKnob->show();
}

void deviceMgr::context_mrq_prop()
{
    Q_ASSERT( NULL != m_pRobo );

    emit itemXActivated( (mcModelObj*)m_pRobo );
}

//! mrv
void deviceMgr::context_mrv_import()
{ }
void deviceMgr::context_mrv_export()
{ context_export(); }
void deviceMgr::context_mrv_upload()
{}
void deviceMgr::context_mrv_alias()
{ context_mrq_alias(); }

void deviceMgr::context_mrv_console()
{ context_mrq_console(); }
void deviceMgr::context_mrv_panel()
{}
void deviceMgr::context_mrv_prop()
{ context_mrq_prop(); }

//! robo
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

        Q_ASSERT( NULL != m_pRobo );
        emit signalModelUpdated( m_pRobo );
    }
}
void deviceMgr::context_robo_console()
{
    deviceConsole *pConsole;

    pConsole = new deviceConsole(this);
    if ( NULL == pConsole )
    { return; }

    connect( this, SIGNAL(signal_instmgr_changed(bool,MegaDevice::InstMgr*)),
             pConsole, SLOT(slot_device_changed()));

    pConsole->setMrq( false );

    Q_ASSERT( NULL != m_pRobo );
    pConsole->setClassName( m_pRobo->getClass().toLower() );

    //! title name
    pConsole->setWindowTitle( m_pRobo->name() );
    pConsole->setShell( m_pRobo );

    pConsole->show();
}

void deviceMgr::context_robo_axes()
{
    Q_ASSERT( NULL != m_pmcModel );
    Q_ASSERT( NULL != m_pRobo );

    roboAxes *pRoboPanel;

    pRoboPanel = new roboAxes( m_pmcModel,
                               QString("%1@%2").arg(m_pRobo->name()).arg( mCurrNodeName ),
                               this );
    if ( pRoboPanel == NULL )
    { return; }

    connect( this, SIGNAL(signal_instmgr_changed(bool,MegaDevice::InstMgr*)),
             pRoboPanel, SLOT(slot_device_changed()) );
    pRoboPanel->show();
}
void deviceMgr::context_robo_panel()
{
    Q_ASSERT( NULL != m_pmcModel );
    Q_ASSERT( NULL != m_pRobo );

    //! creator
    RoboPanel *pRoboPanel = NULL;

    VRobot::robotEnum rId;
    rId = m_pRobo->getId();

    QString roboName = QString("%1@%2").arg(m_pRobo->name()).arg( mCurrNodeName );
    logDbg()<<roboName;
    if ( rId == VRobot::robot_sinanju )
    {
        pRoboPanel = new T4Panel( m_pmcModel,
                                   roboName,
                                   this );
    }
    else if ( rId == VRobot::robot_h2
              || rId == VRobot::robot_h2_m )
    {
        pRoboPanel = new H2Panel( m_pmcModel,
                                   roboName,
                                   this );
    }
    else if ( rId == VRobot::robot_h2z )
    {
        pRoboPanel = new H2ZPanel( m_pmcModel,
                                   roboName,
                                   this );
    }
    else if ( rId == VRobot::robot_igus_delta )
    {
        pRoboPanel = new IgusDrylinPanel( m_pmcModel,
                                          roboName,
                                          this );
    }
    else
    { return; }

    if ( pRoboPanel == NULL )
    { return; }

    //! on changed
    connect( this, SIGNAL(signal_instmgr_changed(bool,MegaDevice::InstMgr*)),
             pRoboPanel, SLOT(slot_device_changed()) );
    pRoboPanel->show();
}

void deviceMgr::context_robo_prop()
{
    Q_ASSERT( NULL != m_pmcModel );
    Q_ASSERT( NULL != m_pRobo );

    emit itemXActivated( (mcModelObj*)m_pRobo );
}

void deviceMgr::context_hub_alias()
{
    context_robo_alias();
}
void deviceMgr::context_hub_console()
{
    context_robo_console();
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

    //! robo node
    if ( pItem->data(0,Qt::UserRole).type() == (QVariant::Type)QMetaType::User )
    {}
    //! mrq axes
    //! \todo mrv axes?
    else if ( pItem->data(0,Qt::UserRole).type() == (QVariant::Type)QMetaType::Int )
    {
        Q_ASSERT( pItem->parent() != NULL );
        //! get parent
        var = pItem->parent()->data(0, Qt::UserRole);
        pObj = var.value<VRobot *>();
        if ( NULL == pItem || NULL == pObj )
        { return; }

        m_pRobo = ( pObj );
        if ( robot_is_mrq( m_pRobo->mId ) )
        {}
        else
        {
            event->accept();
            return;
        }

        m_pMRQ = (MegaDevice::deviceMRQ*)( m_pRobo );
        mCurrentAxes = pItem->data(0,Qt::UserRole).toInt();

        m_pAxesMenu->popup( mapToGlobal( event->pos() ) );

        event->accept();
        return;
    }
    else
    { return; }

    var = pItem->data( 0, Qt::UserRole );
    pObj = var.value<VRobot *>();
    if ( NULL == pItem || NULL == pObj )
    { return; }

    //! current device
    if ( pObj->Type() == mcModelObj::model_device
         || pObj->Type() == mcModelObj::model_composite_device
         )
    {

        m_pRobo = ( pObj );
        m_pCurrTreeItem = pItem;
        mCurrentAxes = -1;

        //! mrq
        if ( robot_is_mrq( m_pRobo->getId() ) )
        {
            m_pMRQ = (MegaDevice::deviceMRQ*)( m_pRobo );

            m_pDeviceMenu->popup( mapToGlobal( event->pos() ) );
            event->accept();
        }
        //! mrv
        else if ( robot_is_mrv( m_pRobo->getId() ) )
        {
            m_pDeviceMenu->popup( mapToGlobal( event->pos() ) );
            event->accept();
        }
        //! robot
        else if ( robot_is_robot( m_pRobo->getId() ) )
        {
            Q_ASSERT( NULL != pItem->parent() );
            mCurrNodeName = pItem->parent()->text(0);

            //! panel for some robo
            m_pRoboPanelAction->setVisible( m_pRobo->roboPanelAble() );

            //! motors has no joints
            m_pRoboJoiontAction->setVisible( m_pRobo->jointPanelAble() );

            m_pRoboMenu->popup( mapToGlobal( event->pos() ) );
            event->accept();
        }
        else
        {
            m_pMRQ = NULL;
        }
    }
    else if ( pObj->Type() == mcModelObj::model_hub )
    {
        m_pRobo = ( pObj );
        m_pCurrTreeItem = pItem;
        mCurrentAxes = -1;

        m_pHubMenu->popup( mapToGlobal( event->pos() ) );
        event->accept();

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

    Q_ASSERT( NULL != m_pmcModel );
    if ( m_pmcModel->mSysPref.mAutoExpand )
    {
        ui->treeWidget->expandAll();
    }

    emit signal_instmgr_changed( true, m_pMgr );
}

void deviceMgr::openBus()
{
    if ( ui->chkOnLine->isEnabled() && !ui->chkOnLine->isChecked() )
    {
        on_chkOnLine_clicked( true );
    }
}
void deviceMgr::closeBus()
{
    if ( ui->chkOnLine->isEnabled() && ui->chkOnLine->isChecked() )
    {
        on_chkOnLine_clicked( false );
        ui->chkOnLine->setChecked( false );
    }
}

void deviceMgr::requestUploadSetting( mcModelObj *pObj )
{
    Q_ASSERT(NULL != pObj );

    m_pRobo = (VRobot*)pObj;

    //! \todo mrv?
    context_upload();
}

void deviceMgr::on_toolState_clicked()
{
    if ( m_pDiagram != NULL )
    {}
    else
    {
        m_pDiagram = new StateDiagram(this);
    }

    if ( NULL == m_pDiagram )
    { return; }

    m_pDiagram->setVisible( !m_pDiagram->isVisible() );
}

void deviceMgr::on_chkOnLine_clicked(bool checked)
{
    if ( checked )
    { on_pushButton_clicked(); }
    else
    {
        Q_ASSERT( NULL != m_pMgr );
        m_pMgr->closeBus();
    }
}

