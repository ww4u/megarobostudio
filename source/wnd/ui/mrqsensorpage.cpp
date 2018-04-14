#include "mrqsensorpage.h"
#include "ui_mrqsensorpage.h"
#include "modelview.h"

MrqSensorPage::MrqSensorPage(QWidget *parent) :
    modelView(parent),
    ui(new Ui::MrqSensorPage)
{
    ui->setupUi(this);

    spyEdited();
}

MrqSensorPage::~MrqSensorPage()
{
    delete ui;
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
void MrqSensorPage::spyEdited()
{
    LINK_MODIFIED( ui->widget );

    LINK_MODIFIED( ui->tab );
    LINK_MODIFIED( ui->tab_2 );
    LINK_MODIFIED( ui->tab_3 );
    LINK_MODIFIED( ui->tab_4 );
}
