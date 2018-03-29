#include "encoderalarmpage.h"
#include "ui_encoderalarmpage.h"

EncoderAlarmPage::EncoderAlarmPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EncoderAlarmPage)
{
    ui->setupUi(this);
}

EncoderAlarmPage::~EncoderAlarmPage()
{
    delete ui;
}
