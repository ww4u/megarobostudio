#include "runtool.h"
#include "ui_runtool.h"

runTool::runTool(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::runTool)
{
    ui->setupUi(this);
}

runTool::~runTool()
{
    delete ui;
}

void runTool::on_toolStop_clicked()
{
    emit sigForceStop();
}
