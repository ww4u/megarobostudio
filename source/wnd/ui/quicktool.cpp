#include "quicktool.h"
#include "ui_quicktool.h"

QuickTool::QuickTool(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QuickTool)
{
    ui->setupUi(this);
}

QuickTool::~QuickTool()
{
    delete ui;
}

void QuickTool::changeEvent(QEvent * event)
{
    QWidget::changeEvent( event );

    if (event->type() == QEvent::LanguageChange)
    { ui->retranslateUi( this ); }
}
