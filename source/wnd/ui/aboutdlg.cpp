#include "aboutdlg.h"
#include "ui_aboutdlg.h"


aboutDlg::aboutDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::aboutDlg)
{
    ui->setupUi(this);
    ui->label_3->setText( qApp->applicationVersion() );
    ui->label_5->setText( tr("Build:") + __DATE__ + " " +  __TIME__ );
}

aboutDlg::~aboutDlg()
{
    delete ui;
}
