#include "roboconfig.h"
#include "ui_roboconfig.h"

RoboConfig::RoboConfig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoboConfig)
{
    ui->setupUi(this);
}

RoboConfig::~RoboConfig()
{
    delete ui;
}

void RoboConfig::changeEvent(QEvent * event)
{
    QWidget::changeEvent( event );

    if (event->type() == QEvent::LanguageChange)
    { ui->retranslateUi( this ); }
}
