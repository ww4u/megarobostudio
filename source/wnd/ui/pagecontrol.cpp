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

void PageControl::changeEvent(QEvent * event)
{
    QWidget::changeEvent( event );

    if (event->type() == QEvent::LanguageChange)
    { ui->retranslateUi( this ); }
}

QComboBox *PageControl::getComb()
{ return ui->comboBox; }

void PageControl::setPage( int pageIndex )
{ ui->comboBox->setCurrentIndex( pageIndex ); }

int PageControl::page()
{ return ui->comboBox->currentIndex(); }
