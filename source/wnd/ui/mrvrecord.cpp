#include "mrvrecord.h"
#include "ui_mrvrecord.h"

MrvRecord::MrvRecord(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MrvRecord)
{
    ui->setupUi(this);
}

MrvRecord::~MrvRecord()
{
    delete ui;
}
