#include "deltapref.h"
#include "ui_deltapref.h"

DeltaPref::DeltaPref(QWidget *parent) :
    modelView(parent),
    ui(new Ui::deltapref)
{
    ui->setupUi(this);
}

DeltaPref::~DeltaPref()
{
    delete ui;
}
