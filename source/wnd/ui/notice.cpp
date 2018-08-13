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
{ ui->checkBox->isChecked(); }
