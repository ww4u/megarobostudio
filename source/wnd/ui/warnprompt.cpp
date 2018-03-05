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
    ui->labInfo->setText( str );
}
