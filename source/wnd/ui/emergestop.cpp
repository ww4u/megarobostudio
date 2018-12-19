#include "emergestop.h"
#include "ui_emergestop.h"

EmergeStop::EmergeStop(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EmergeStop)
{
    ui->setupUi(this);
}

EmergeStop::~EmergeStop()
{
    delete ui;
}
