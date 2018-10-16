#include "dlghelp.h"
#include "ui_dlghelp.h"

DlgHelp::DlgHelp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgHelp)
{
    ui->setupUi(this);
}

DlgHelp::~DlgHelp()
{
    delete ui;
}

void DlgHelp::showHelp( const QString &info,
                        const QString &img )
{
    //! text
    if ( info.length() < 1 )
    {
        ui->txt->setPlainText( info);
        ui->txt->setVisible( true );
    }
    else
    { ui->txt->setVisible( false ); }

    //! img
    if ( img.length() > 0 )
    {
        ui->img->setPixmap( QPixmap( img) );
    }
    else
    { ui->img->setVisible( false ); }
}

void DlgHelp::setShowAgain( bool b)
{ ui->chkShowAgain->setChecked(b); }
bool DlgHelp::showAgain()
{ return ui->chkShowAgain->isChecked(); }
