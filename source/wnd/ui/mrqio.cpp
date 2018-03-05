#include "mrqio.h"
#include "ui_mrqio.h"

mrqIo::mrqIo(QWidget *parent) :
    mrqView(parent),
    ui(new Ui::mrqIo)
{
    ui->setupUi(this);

    setupUi();
}

mrqIo::~mrqIo()
{
    delete ui;

    desetupUi();
}

int mrqIo::setApply()
{
    return apply();
}

void mrqIo::modelChanged()
{ updateData(); }

void mrqIo::setupUi()
{}
void mrqIo::desetupUi()
{}

int mrqIo::apply()
{ return 0; }

int mrqIo::updateData()
{ return 0; }


