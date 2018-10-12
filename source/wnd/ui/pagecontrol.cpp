#include "pagecontrol.h"
#include "ui_pagecontrol.h"

PageControl::PageControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageControl)
{
    ui->setupUi(this);

    //! connection
    connect( ui->toolRun, SIGNAL(clicked(bool)),
             this, SIGNAL(signal_run_clicked()) );

    connect( ui->toolStop, SIGNAL(clicked(bool)),
             this, SIGNAL(signal_stop_clicked()) );
}

PageControl::~PageControl()
{
    delete ui;
}

QComboBox *PageControl::getComb()
{ return ui->pageOption->getComb(); }

void PageControl::setPage( int pageIndex )
{ ui->pageOption->setPage( pageIndex); }

int PageControl::page()
{ return ui->pageOption->page(); }
