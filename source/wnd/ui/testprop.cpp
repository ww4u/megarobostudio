#include "testprop.h"
#include "ui_testprop.h"

TestProp::TestProp(QWidget *parent) :
    modelView(parent),
    ui(new Ui::TestProp)
{
    ui->setupUi(this);
}

TestProp::~TestProp()
{
    delete ui;
}
