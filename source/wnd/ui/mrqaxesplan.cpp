#include "mrqaxesplan.h"
#include "ui_mrqaxesplan.h"

MrqAxesPlan::MrqAxesPlan(QWidget *parent) :
    mrqView(parent),
    ui(new Ui::MrqAxesPlan)
{
    ui->setupUi(this);

    spyEdited();
}

MrqAxesPlan::~MrqAxesPlan()
{
    delete ui;
}

void MrqAxesPlan::changeEvent(QEvent * event)
{
    mrqView::changeEvent( event );

    if (event->type() == QEvent::LanguageChange)
    { ui->retranslateUi( this ); }
}

int MrqAxesPlan::setApply()
{
    return apply();
}

void MrqAxesPlan::modelChanged()
{
    updateUi();logDbg();
}

void MrqAxesPlan::spyEdited()
{

}

int MrqAxesPlan::apply()
{
    return 0;
}
int MrqAxesPlan::updateUi()
{
    return 0;
}
int MrqAxesPlan::updateData()
{
    if ( NULL == m_pMrqModel )
    { return ERR_NULL_POINTER; }

    return 0;
}
