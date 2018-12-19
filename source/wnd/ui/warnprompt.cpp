#include "warnprompt.h"
#include "ui_warnprompt.h"

WarnPrompt::WarnPrompt(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WarnPrompt)
{
    ui->setupUi(this);
    mCapcity = 128;
}

WarnPrompt::~WarnPrompt()
{
    delete ui;
}

void WarnPrompt::changeEvent(QEvent * event)
{
    QDialog::changeEvent( event );

    if (event->type() == QEvent::LanguageChange)
    { ui->retranslateUi( this ); }
}

void WarnPrompt::setInfo( const QString &str )
{
    while ( ui->listWidget->count() >= mCapcity )
    { delete ui->listWidget->takeItem(0); }

    ui->listWidget->addItem( str );
    ui->listWidget->setCurrentRow( ui->listWidget->count() - 1 );
}

void WarnPrompt::addInfo( const QString &str )
{
    while ( ui->listWidget->count() >= mCapcity )
    { delete ui->listWidget->takeItem(0); }

    ui->listWidget->addItem( str );
    ui->listWidget->setCurrentRow( ui->listWidget->count() - 1 );
}

void WarnPrompt::hideEvent(QHideEvent *event)
{
    ui->listWidget->clear();
}
