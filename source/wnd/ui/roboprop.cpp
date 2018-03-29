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

int roboProp::setApply()
{
    int ret;
    foreach( modelView *pView, mPrefPages )
    {
        Q_ASSERT( NULL != pView );

        ret = pView->setApply();
        if ( ret != 0 )
        { return ret; }
    }

    return ret;
}

int roboProp::save( QString &outFileName )
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

    return ((VRobot*)getModelObj())->save( outFileName + setup_d_ext);
}

int  roboProp::saveAs( QString &outFileName )
{
    return ((VRobot*)getModelObj())->save( outFileName );
}


#define new_widget( widget, icon, name )  ( names.append(name), \
                                            icons.append(icon), \
                                            mPrefPages.append( new widget()),\
                                            (widget*)mPrefPages.last() )
void roboProp::setupUi( int id )
{
    QStringList icons,names;

    //! new
//    m_pInfoPage = new roboInfo();
//    m_pComPref = new RoboComPref();
    m_pInfoPage = new_widget( roboInfo, ":/res/image/icon2/info.png", tr("Info") );
    m_pDetailPage = new_widget( RoboDesc, ":/res/image/icon2/info.png", tr("Detail") );
    m_pComPref  = new_widget( RoboComPref, ":/res/image/icon2/settings_light.png", tr("Pref") );

    if ( VRobot::robot_delta == id )
    {
        m_pDeltaPref  = new_widget( DeltaPref, ":/res/image/icon2/settings_light.png", tr("Pref") );
    }
    else if ( VRobot::robot_megatron == id )
    {
        m_pMegatronPref  = new_widget( MegatronPref, ":/res/image/icon2/settings_light.png", tr("Pref") );
    }
    else if ( VRobot::robot_sinanju == id )
    {
        m_pSinanjuPref  = new_widget( SinanjuPref, ":/res/image/icon2/settings_light.png", tr("Pref") );
        m_pHandPage  = new_widget( RoboHand, ":/res/image/icon2/activity.png", tr("Action") );
    }
    else if ( VRobot::robot_h2 == id )
    {
        m_pH2Pref = new_widget( H2Pref, ":/res/image/icon2/settings_light.png", tr("Pref") );
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

    //! post
    on_page_changed( ui->stackedWidget->currentIndex() );
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
             this, SLOT(on_page_changed(int)));
}

void roboProp::on_page_changed( int index )
{
    Q_ASSERT( index < ui->stackedWidget->count() );

    Q_ASSERT( NULL != ui->stackedWidget->widget(index) );

    modelView *pView = ((modelView*)ui->stackedWidget->widget(index));
    Q_ASSERT( NULL != pView );
    ui->btnCancel->setEnabled( pView->isCanceAble() );
    ui->btnOK->setEnabled( pView->isOkAble() );
    ui->btnApply->setEnabled( pView->isApplyAble() );
}

void roboProp::on_btnOK_clicked()
{
    //! apply
    setApply();

    //! save
    QString outFileName;

    int ret = save( outFileName );
    if ( ret != 0 )
    { return; }

    emit sigClose( this );
}

void roboProp::on_btnCancel_clicked()
{
    emit sigClose( this );
}

void roboProp::on_btnApply_clicked()
{
    //! apply
    setApply();

    //! save
    QString outFileName;

    int ret = save( outFileName );
    if ( ret != 0 )
    { return; }
}
