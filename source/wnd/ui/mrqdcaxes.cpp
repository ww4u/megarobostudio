#include "mrqdcaxes.h"
#include "ui_mrqdcaxes.h"

MrqDcAxes::MrqDcAxes(QWidget *parent) :
    mrqView(parent),
    ui(new Ui::MrqDcAxes)
{
    ui->setupUi(this);

    spyEdited();
}

MrqDcAxes::~MrqDcAxes()
{
    delete ui;
}

void MrqDcAxes::changeEvent(QEvent * event)
{
    mrqView::changeEvent( event );

    if (event->type() == QEvent::LanguageChange)
    { ui->retranslateUi( this ); }
}

void MrqDcAxes::spyEdited()
{}
