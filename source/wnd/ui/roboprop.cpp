#include "roboprop.h"
#include "ui_mrqproperty.h"

#include "../../robot/robotfact.h"
#include "../../device/vrobot.h"

roboProp::roboProp( int id , QWidget *parent) :
    modelView(parent),
    ui(new Ui::mrqProperty)
{
    mPrefId = id;

    mFilePattern<<setup_desc<<setup_ext;

    setupUi( id );

    buildConnection();

    //! three btns
    mbtnEnableSnap.append( false );
    mbtnEnableSnap.append( false );
    mbtnEnableSnap.append( false );

    ui->btnReset->setVisible( false );
}

roboProp::~roboProp()
{
    desetupUi();
}

void roboProp::setModelObj( mcModelObj *pObj )
{
    Q_ASSERT( NULL != pObj );

    //! base
    modelView::setModelObj( pObj );

    //! \todo memory leak

    pObj->setGc( false );

    foreach( modelView *pView, mPrefPages )
    {
        Q_ASSERT( NULL != pView );
        pView->setModelObj( pObj );
    }
}

void roboProp::setMcModel( mcModel *pMcModel )
{
    Q_ASSERT( NULL != pMcModel );

    modelView::setMcModel( pMcModel );

    foreach( modelView *pView, mPrefPages )
    {
        Q_ASSERT( NULL != pView );
        pView->setMcModel( pMcModel );
    }
}

int roboProp::save( QString &outFileName )
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

    return ((VRobot*)getModelObj())->save( outFileName + setup_d_ext);
}

int roboProp::saveAs( QString &outFileName )
{
    slotModified( false );

    return ((VRobot*)getModelObj())->save( outFileName );
}

void roboProp::updateScreen()
{
    foreach( modelView *pView, mPrefPages )
    {
        Q_ASSERT( NULL != pView );
        pView->updateScreen();
    }
}
 void roboProp::updateModel()
{}

#define new_widget( widget, icon, name )  ( names.append(name), \
                                            icons.append(icon), \
                                            mPrefPages.append( new widget()),\
                                            (widget*)mPrefPages.last() )
void roboProp::setupUi( int id )
{
    QStringList icons,names;

    //! new
    m_pInfoPage = new_widget( roboInfo, ":/res/image/icon2/info.png", tr("Info") );
    m_pDetailPage = new_widget( RoboDesc, ":/res/image/icon2/info.png", tr("Detail") );

    //! pref
    if ( VRobot::robot_motors == id )
    { m_pComPref = new_widget( MotorsPref, ":/res/image/icon2/settings_light.png", tr("Option") ); }
    else
    { m_pComPref  = new_widget( RoboComPref, ":/res/image/icon2/settings_light.png", tr("Option") ); }

    //! special prop
    if ( VRobot::robot_delta == id )
    {
        m_pDeltaPref  = new_widget( DeltaPref, ":/res/image/icon2/settings_light.png", tr("Zero") );
        m_pDeltaConfig   = new_widget( DeltaConfig, ":/res/image/icon2/settings_light.png", tr("Pref") );
    }
    else if ( VRobot::robot_megatron == id )
    {
        m_pMegatronPref  = new_widget( MegatronPref, ":/res/image/icon2/settings_light.png", tr("Zero") );
    }
    //! h2 && h2z
    else if ( VRobot::robot_h2 == id
              || VRobot::robot_h2_m == id
              || VRobot::robot_h2z == id
              )
    {
        m_pSketch = new_widget( RoboSketch, ":/res/image/icon2/settings_light.png", tr("Icon") );
        m_pSketch->setSketch( ":/res/image/joint/mrx-h2_geo.png");
        m_pH2Pref = new_widget( H2Pref, ":/res/image/icon2/settings_light.png", tr("Zero") );
        m_pH2Config = new_widget( H2Config, ":/res/image/icon2/settings_light.png", tr("Pref") );
    }
    else if ( VRobot::robot_sinanju == id )
    {
        m_pSketch = new_widget( RoboSketch, ":/res/image/icon2/settings_light.png", tr("Icon") );
        m_pSketch->setSketch( ":/res/image/joint/sinanju_pn_256px_nor@2x.png");

        m_pSinanjuPref  = new_widget( SinanjuPref, ":/res/image/icon2/settings_light.png", tr("Zero") );
        m_pSinanjuConfig = new_widget( SinanjuConfig, ":/res/image/icon2/settings_light.png", tr("Pref") );
    }
    else if ( VRobot::robot_motor == id )
    {
        m_pMotorPref = new_widget( MotorPref, ":/res/image/icon2/settings_light.png", tr("Zero") );
    }
    else if (
              VRobot::robot_slide == id
              )
    {
        m_pAxnPref = new_widget( AxnPref, ":/res/image/icon2/settings_light.png", tr("Pref") );
    }
    else if ( VRobot::robot_ip == id )
    {
        m_pIpPref = new_widget( IPPref, ":/res/image/icon2/settings_light.png", tr("Pref") );
        m_pIpConfig = new_widget( IPConfig, ":/res/image/icon2/settings_light.png", tr("Config") );
    }
    else if ( VRobot::robot_igus_delta == id )
    {
        m_pIgusPref = new_widget( IgusDeltaPref, ":/res/image/icon2/settings_light.png", tr("Zero") );
        m_pIgusConfig = new_widget( IgusConfig, ":/res/image/icon2/settings_light.png", tr("Pref") );
    }
    else
    {}

    //! setup
    ui->setupUi(this);

    //! add to page
    QListWidgetItem *pItem;
    for ( int i = 0; i < mPrefPages.size(); i++ )
    {
        Q_ASSERT( NULL != mPrefPages.at(i) );

        ui->stackedWidget->addWidget( mPrefPages.at(i) );

        pItem = new QListWidgetItem();
        Q_ASSERT( NULL != pItem );

        pItem->setText( names.at(i) );
        pItem->setIcon( QIcon( icons.at(i) ) );
        ui->listWidget->addItem( pItem );
    }

    //! modified
    foreach( modelView* pView, mPrefPages )
    {
        connect( pView, SIGNAL(sigModified(bool)),
                 this,  SLOT(slotModified(bool)) );
    }

    //! to pref
    if ( ui->stackedWidget->count() > 3 )
    { ui->stackedWidget->setCurrentIndex( 3 ); }
    else
    { ui->stackedWidget->setCurrentWidget( m_pComPref ); }
}

void roboProp::desetupUi()
{
    delete ui;
}

void roboProp::buildConnection()
{
    connect( ui->listWidget, SIGNAL(currentRowChanged(int)),
             ui->stackedWidget, SLOT(setCurrentIndex(int)));

    connect( ui->stackedWidget, SIGNAL(currentChanged(int)),
             this, SLOT(slot_page_changed(int)));
}

int roboProp::postApply( appMsg msg, void *pPara )
{
    int ret,id;
    ret = 0;
    id = 0;
    foreach( modelView *pView, mPrefPages )
    {
        Q_ASSERT( NULL != pView );

        sysProgress( id++, pView->name(), mPrefPages.size(), 0 );
        sysProgress( true );
        ret = pView->setApply();
        sysProgress( id, pView->name(), mPrefPages.size(), 0 );
        if ( ret != 0 )
        { break; }
    }

    return ret;
}
void roboProp::beginApply( void *pPara)
{
    sysProgress( 0, tr("Begin apply") );
    sysProgress(true);

    saveBtnSnap();
}
void roboProp::endApply( int ret, void *pPara )
{
    sysProgress(false);

    restoreBtnSnap();

    slotModified( false );

    emit sigSaveRequest( this );
}

int roboProp::postOk( appMsg msg, void *pPara )
{
    return postApply( msg_robo_property_apply, pPara );
}
void roboProp::beginOk( void *pPara)
{ beginApply(pPara); }
void roboProp::endOk( int ret, void *pPara )
{
    endApply( ret, pPara);

    slotModified( false );

    emit sigSaveRequest( this );

    emit sigClose( this );
}

void roboProp::saveBtnSnap( bool bNow )
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
void roboProp::restoreBtnSnap()
{
    //! restore
    ui->btnApply->setEnabled( mbtnEnableSnap[0] );
    ui->btnOK->setEnabled( mbtnEnableSnap[1] );
    ui->btnCancel->setEnabled( mbtnEnableSnap[2] );
}

void roboProp::slot_page_changed( int index )
{
    Q_ASSERT( index < ui->stackedWidget->count() );

    Q_ASSERT( NULL != ui->stackedWidget->widget(index) );

    modelView *pView = ((modelView*)ui->stackedWidget->widget(index));
    Q_ASSERT( NULL != pView );
    ui->btnCancel->setEnabled( pView->isCanceAble() );
    ui->btnOK->setEnabled( pView->isOkAble() );
    ui->btnApply->setEnabled( pView->isApplyAble() );

    ui->btnCancel->setFocus();
}

void roboProp::on_btnOK_clicked()
{
    post_request( msg_robo_property_ok, roboProp, Ok );
}

void roboProp::on_btnCancel_clicked()
{
    emit sigClose( this );
}

void roboProp::on_btnApply_clicked()
{
    post_request( msg_robo_property_apply, roboProp, Apply );
}
