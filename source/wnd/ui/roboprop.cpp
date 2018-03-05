#include "roboprop.h"
#include "ui_mrqproperty.h"

#include "../../robot/robotfact.h"

roboProp::roboProp(QWidget *parent) :
    modelView(parent),
    ui(new Ui::mrqProperty)
{
    mFilePattern<<setup_desc<<setup_ext;

    setupUi();

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

    pObj->setGc( false );logDbg();
    m_pInfoPage->setModelObj( pObj );logDbg();
    m_pPrefPage->setModelObj( pObj );logDbg();
    m_pHandPage->setModelObj( pObj );logDbg();
}

void roboProp::setMcModel( mcModel *pMcModel )
{
    Q_ASSERT( NULL != pMcModel );

    modelView::setMcModel( pMcModel );

    m_pInfoPage->setMcModel( pMcModel );
    m_pPrefPage->setMcModel( pMcModel );
    m_pHandPage->setMcModel( pMcModel );

}

int roboProp::setApply()
{
    return m_pPrefPage->setApply();
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

void roboProp::setupUi()
{
    //! new
    m_pInfoPage = new roboInfo();
    m_pPrefPage = new roboPref();
    m_pHandPage = new RoboHand();

    //! setup
    ui->setupUi(this);

    //! add to page
    ui->stackedWidget->addWidget( m_pInfoPage );
    ui->stackedWidget->addWidget( m_pPrefPage );
    ui->stackedWidget->addWidget( m_pHandPage );

    //! list
    QListWidgetItem *pItem;
    pItem = new QListWidgetItem();
    pItem->setText( tr("info") );
    pItem->setIcon( QIcon(":/res/image/roboinfo.png") );
    ui->listWidget->addItem( pItem );

    pItem = new QListWidgetItem();
    pItem->setText( tr("pref") );
    pItem->setIcon( QIcon(":/res/image/robopref.png") );
    ui->listWidget->addItem( pItem );

    pItem = new QListWidgetItem();
    pItem->setText( tr("hand") );
    pItem->setIcon( QIcon(":/res/image/robopref.png") );
    ui->listWidget->addItem( pItem );
}

void roboProp::desetupUi()
{
    delete ui;
}

void roboProp::buildConnection()
{
    connect( ui->listWidget, SIGNAL(currentRowChanged(int)),
                  ui->stackedWidget, SLOT(setCurrentIndex(int)));
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
