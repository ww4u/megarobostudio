#include "statebar.h"
#include "ui_statebar.h"

#include "../../include/mydebug.h"

stateBar::stateBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::stateBar)
{
    ui->setupUi(this);

    ui->progressBar->hide();
    ui->labeIInfo->hide();
    ui->downloadBar->hide();
}

stateBar::~stateBar()
{
    delete ui;
}

void stateBar::changeEvent(QEvent * event)
{
    QWidget::changeEvent( event );

    if (event->type() == QEvent::LanguageChange)
    { ui->retranslateUi( this ); }
}

void stateBar::showInfo( const QString &str )
{ ui->labeIInfo->setText(str); }

void stateBar::showProgress( int from, int to, int now,
                   const QString &info )
{
    ui->progressBar->setRange(from,to);
    ui->progressBar->setValue( now );
}
void stateBar::showState( const QString &str )
{ ui->labelState->setText( str ); }

void stateBar::downloadProgress( bool b, int from, int to , int now )
{
    if ( b )
    {
        ui->downloadBar->setRange( from, to );
        ui->downloadBar->setValue( now );
        ui->downloadBar->show();
    }
    else
    { ui->downloadBar->hide(); }
}

QProgressBar *stateBar::progressBar()
{ return ui->progressBar; }
QLabel *stateBar::progressInfo()
{ return ui->labeIInfo; }
QLabel *stateBar::statusLabel()
{ return ui->labelState; }

MProgressBar * stateBar::downloadBar()
{ return ui->downloadBar; }

