#include "notice.h"
#include "ui_notice.h"
#include <QFile>
Notice::Notice(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Notice)
{
    ui->setupUi(this);

}

Notice::~Notice()
{
    delete ui;
}

void Notice::changeEvent(QEvent * event)
{
    QDialog::changeEvent( event );

    if (event->type() == QEvent::LanguageChange)
    { ui->retranslateUi( this ); }
}

bool Notice::load( QString &str )
{
    QFile file( str );

    if ( file.open( QIODevice::ReadOnly ) )
    {}
    else
    { return false; }

    QByteArray ary;
    ary = file.readAll();
    if ( ary.length() > 0 )
    {  }
    else
    { return false; }

    ui->textEdit->setText( ary );

    return true;
}

bool Notice::nextShowAble()
{ return ui->checkBox->isChecked(); }
