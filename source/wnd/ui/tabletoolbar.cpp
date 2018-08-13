#include "tabletoolbar.h"
#include "ui_tabletoolbar.h"

TableToolbar::TableToolbar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TableToolbar)
{
    ui->setupUi(this);

    connect( ui->btnAdd,SIGNAL(clicked(bool)),
             this, SIGNAL(signal_add_clicked()));

    connect( ui->btnDel,SIGNAL(clicked(bool)),
             this, SIGNAL(signal_del_clicked()));

    connect( ui->btnClr,SIGNAL(clicked(bool)),
             this, SIGNAL(signal_clr_clicked()));
}

TableToolbar::~TableToolbar()
{
    delete ui;
}

void TableToolbar::setAddEnable( bool b )
{ ui->btnAdd->setEnabled(b);}
void TableToolbar::setDelEnable( bool b )
{ ui->btnDel->setEnabled(b);}
void TableToolbar::setclrEnable( bool b )
{ ui->btnClr->setEnabled(b);}
