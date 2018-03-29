#include "mrqdcaxes.h"
#include "ui_mrqdcaxes.h"

MrqDcAxes::MrqDcAxes(QWidget *parent) :
    mrqView(parent),
    ui(new Ui::MrqDcAxes)
{
    ui->setupUi(this);
}

MrqDcAxes::~MrqDcAxes()
{
    delete ui;
}
