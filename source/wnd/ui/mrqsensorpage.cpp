#include "mrqsensorpage.h"
#include "ui_mrqsensorpage.h"
#include "modelview.h"

MrqSensorPage::MrqSensorPage(QWidget *parent) :
    modelView(parent),
    ui(new Ui::MrqSensorPage)
{
    ui->setupUi(this);

    mSubList.append( ui->tab );
    mSubList.append( ui->tab_2 );
    mSubList.append( ui->tab_3 );
    mSubList.append( ui->tab_4 );

    spyEdited();

    foreach( MrqSubUart *pSub, mSubList )
    {
        connect( pSub, SIGNAL(signal_live_changed()),
                 this, SLOT(slot_sub_live_changed()));
    }
}

MrqSensorPage::~MrqSensorPage()
{
    delete ui;
}

void MrqSensorPage::changeEvent(QEvent * event)
{
    modelView::changeEvent( event );

    if (event->type() == QEvent::LanguageChange)
    { ui->retranslateUi( this ); }
}

void MrqSensorPage::setUartConfig(
                 uartConfig &cfg
                 )
{
    ui->widget->setConfig( cfg );
}
void MrqSensorPage::getUartConfig(
        uartConfig &cfg
                 )
{
    ui->widget->getConfig( cfg );
}

void MrqSensorPage::setSubUartConfig(
                 int index,
                 subUartConfig &cfg
                 )
{
    Q_ASSERT( index < ui->tabWidget->count() && index >= 0 );

    ((MrqSubUart*)ui->tabWidget->widget( index ))->setConfig( cfg );
}
void MrqSensorPage::getSubUartConfig(
        int index,
        subUartConfig &cfg
                 )
{
    Q_ASSERT( index < ui->tabWidget->count() && index >= 0 );

    ((MrqSubUart*)ui->tabWidget->widget( index ))->getConfig( cfg );
}

int MrqSensorPage::sensCount()
{ return ui->tabWidget->count(); }

void MrqSensorPage::removeSens( int id )
{
    ui->tabWidget->removeTab( id );

    if ( ui->tabWidget->count() > 1 )
    { ui->chkAllEn->setVisible( true ); }
    else
    { ui->chkAllEn->setVisible( false ); }
}

void MrqSensorPage::setSensText( int id,
                                 const QString &tabTxt )
{
    Q_ASSERT( id >= 0 && id < ui->tabWidget->count() );
    ui->tabWidget->setTabText( id, tabTxt );
}

void MrqSensorPage::spyEdited()
{
    //! links
    LINK_MODIFIED( ui->widget );

    foreach (MrqSubUart *pSubUart, mSubList )
    {
        LINK_MODIFIED( pSubUart );
    }

    //! elements
    QGroupBox *gpBox[]=
    {
    };

    QCheckBox *checkBoxes[]=
    {
        ui->chkAllEn,

    };
    QRadioButton *radBoxes[] = {
    };
    QLineEdit *edits[]={

    };

    QSpinBox *spinBoxes[]={
    };
    QDoubleSpinBox *doubleSpinBoxes[]={
    };

    QComboBox *comboxes[]={
    };

    install_spy();
}

void MrqSensorPage::on_chkAllEn_clicked(bool checked)
{
    foreach (MrqSubUart *pSubUart, mSubList )
    {
        pSubUart->setLive( checked );
    }
}

void MrqSensorPage::slot_sub_live_changed( )
{
    bool bCheck;

    bCheck = true;

    foreach (MrqSubUart *pSubUart, mSubList )
    {
        bCheck = bCheck && pSubUart->isLive();
    }

    ui->chkAllEn->setChecked( bCheck );
}
