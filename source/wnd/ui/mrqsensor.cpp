#include "mrqsensor.h"
#include "ui_mrqsensor.h"

mrqSensor::mrqSensor(QWidget *parent) :
    mrqView(parent),
    ui(new Ui::mrqSensor)
{
    ui->setupUi(this);

    setupUi();
}

mrqSensor::~mrqSensor()
{
    delete ui;

    desetupUi();
}

int mrqSensor::setApply()
{
    return apply();
}

void mrqSensor::modelChanged()
{ updateData(); }

void mrqSensor::setupUi()
{}
void mrqSensor::desetupUi()
{}

int mrqSensor::apply()
{ return 0; }

int mrqSensor::updateData()
{ return 0; }
