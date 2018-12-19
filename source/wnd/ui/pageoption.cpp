#include "pageoption.h"
#include "ui_pageoption.h"

PageOption::PageOption(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageOption)
{
    ui->setupUi(this);
}

PageOption::~PageOption()
{
    delete ui;
}

void PageOption::changeEvent(QEvent * event)
{
    QWidget::changeEvent( event );

    if (event->type() == QEvent::LanguageChange)
    { ui->retranslateUi( this ); }
}

QComboBox *PageOption::getComb()
{ return ui->comboBox; }

void PageOption::setPage( int pageIndex )
{ ui->comboBox->setCurrentIndex(pageIndex); }
int PageOption::page()
{ return ui->comboBox->currentIndex(); }
