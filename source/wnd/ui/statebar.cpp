#include "statebar.h"
#include "ui_statebar.h"

stateBar::stateBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::stateBar)
{
    ui->setupUi(this);
}

stateBar::~stateBar()
{
    delete ui;
}

void stateBar::showState( const QString &str )
{
    ui->label->setText( str );
}
