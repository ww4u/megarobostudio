#include "warnprompt.h"
#include "ui_warnprompt.h"

WarnPrompt::WarnPrompt(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WarnPrompt)
{
    ui->setupUi(this);
}

WarnPrompt::~WarnPrompt()
{
    delete ui;
}

void WarnPrompt::setInfo( const QString &str )
{
    ui->listWidget->addItem( str );
    ui->listWidget->setCurrentRow( ui->listWidget->count() - 1 );
}

void WarnPrompt::addInfo( const QString &str )
{
    ui->listWidget->addItem( str );
    ui->listWidget->setCurrentRow( ui->listWidget->count() - 1 );
}

void WarnPrompt::hideEvent(QHideEvent *event)
{
    ui->listWidget->clear();
}
