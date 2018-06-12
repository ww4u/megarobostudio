#include "lrbutton.h"
#include "ui_lrbutton.h"

LRButton::LRButton(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LRButton)
{
    ui->setupUi(this);
}

LRButton::~LRButton()
{
    delete ui;
}


void LRButton::setStep( float stp )
{
    ui->doubleSpinBox->setValue( stp );
}
float LRButton::step()
{ return ui->doubleSpinBox->value(); }

void LRButton::on_btnLeft_clicked()
{
    emit signal_step( -step() );
}

void LRButton::on_btnRight_clicked()
{
    emit signal_step( step() );
}
