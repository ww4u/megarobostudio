#include "aboutdlg.h"
#include "ui_aboutdlg.h"

aboutDlg::aboutDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::aboutDlg)
{
    ui->setupUi(this);
    ui->label_3->setText( qApp->applicationVersion() );
}

aboutDlg::~aboutDlg()
{
    delete ui;
}
