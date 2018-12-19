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

void TestProp::changeEvent(QEvent * event)
{
    modelView::changeEvent( event );

    if (event->type() == QEvent::LanguageChange)
    { ui->retranslateUi( this ); }
}
