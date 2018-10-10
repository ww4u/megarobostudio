#include "progresselement.h"
#include "ui_progresselement.h"

#include "../../com/comassist.h"

ProgressElement::ProgressElement(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProgressElement)
{
    mId = 0;

    ui->setupUi(this);

    mLatestProg = -1;
}

ProgressElement::~ProgressElement()
{
    delete ui;
}

void ProgressElement::hideEvent(QHideEvent *event )
{
    mLatestProg = -1;

    ui->labelProg->clear();
}

void ProgressElement::set( const QString &name, int id )
{
    mName = name;
    mId = id;
}
QString ProgressElement::name()
{ return mName; }
int ProgressElement::Id()
{ return mId; }

bool ProgressElement::match( const QString &name, int id )
{
    if ( mName == name && mId == id )
    { return true; }
    else
    { return false; }
}

void ProgressElement::progressInfo( const QString &str )
{ ui->labInfo->setText( str ); }
void ProgressElement::progressProg( int n )
{
    ui->progress->setValue( n );

    //! set the time
    if ( mLatestProg == -1 )
    { ui->labelProg->clear(); }
    else if ( n == mLatestProg )
    { return; }
    //! guess the time
    else
    {
        float gTime = ( ui->progress->maximum() - n ) * mLatestTime.elapsed() / ( n - mLatestProg );

        if ( gTime > 0 )
        {
            ui->labelProg->setText( ( comAssist::msToHmsz(gTime) ) );
        }
    }

    mLatestProg = n;
    mLatestTime.restart();
}
void ProgressElement::progressRange( int a, int b )
{ ui->progress->setRange( a, b ); }

void ProgressElement::on_btnCancel_clicked()
{
    emit sigCancel( mName, mId );
}
