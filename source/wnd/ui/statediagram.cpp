#include "statediagram.h"
#include "ui_statediagram.h"

StateDiagram::StateDiagram(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StateDiagram)
{
    ui->setupUi(this);
}

StateDiagram::~StateDiagram()
{
    delete ui;
}

void StateDiagram::changeEvent(QEvent * event)
{
    QDialog::changeEvent( event );

    if (event->type() == QEvent::LanguageChange)
    { ui->retranslateUi( this ); }
}
