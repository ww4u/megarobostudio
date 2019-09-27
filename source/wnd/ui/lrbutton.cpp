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

void LRButton::changeEvent(QEvent * event)
{
    QWidget::changeEvent( event );

    if (event->type() == QEvent::LanguageChange)
    { ui->retranslateUi( this ); }
}

void LRButton::setStep( float stp )
{
    ui->singleSpin->setValue( stp );
}
float LRButton::step()
{ return ui->singleSpin->value(); }

void LRButton::setSingleStep( float singleStep )
{ ui->singleSpin->setSingleStep( singleStep); }

void LRButton::on_btnLeft_clicked()
{
    emit signal_step( -step() );
}

void LRButton::on_btnRight_clicked()
{
    emit signal_step( step() );
}

void LRButton::on_btnLeft_pressed()
{
    emit signal_step( -step(), true );
}

void LRButton::on_btnLeft_released()
{
    emit signal_step( -step(), false );
}
#include "../../include/mydebug.h"
void LRButton::on_btnRight_pressed()
{
    emit signal_step( step(), true );
}

void LRButton::on_btnRight_released()
{
    emit signal_step( step(), false );
}
