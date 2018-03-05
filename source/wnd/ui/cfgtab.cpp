#include "cfgtab.h"
#include "ui_cfgtab.h"

cfgTab::cfgTab(QWidget *parent) :
    QTabWidget(parent),
    ui(new Ui::cfgTab)
{
    ui->setupUi(this);
}

cfgTab::~cfgTab()
{
    delete ui;
}
