#include "distancealarmpage.h"
#include "ui_distancealarmpage.h"

DistanceAlarmPage::DistanceAlarmPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DistanceAlarmPage)
{
    ui->setupUi(this);
}

DistanceAlarmPage::~DistanceAlarmPage()
{
    delete ui;
}
