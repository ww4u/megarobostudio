#include "mrqalarm.h"
#include "ui_mrqalarm.h"
#include "distancealarmpage.h"
#include "encoderalarmpage.h"
MrqAlarm::MrqAlarm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MrqAlarm)
{
    ui->setupUi(this);

    for ( int i = 0; i < 4; i++ )
    {
        ui->tabWidget->insertTab( i, new EncoderAlarmPage(), QString("%1%2").arg( tr("S")).arg(i+1) );
    }

    for ( int i = 0; i < 4; i++ )
    {
        ui->tabWidget_2->insertTab( i, new DistanceAlarmPage(), QString("%1%2").arg( tr("S")).arg(i+1) );
    }
}

MrqAlarm::~MrqAlarm()
{
    delete ui;
}
