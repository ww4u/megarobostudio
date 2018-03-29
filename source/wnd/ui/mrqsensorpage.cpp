#include "mrqsensorpage.h"
#include "ui_mrqsensorpage.h"

MrqSensorPage::MrqSensorPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MrqSensorPage)
{
    ui->setupUi(this);
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
