#include "dlgapp.h"
#include "ui_dlgapp.h"

DlgApp::DlgApp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgApp)
{
    ui->setupUi(this);
}

DlgApp::~DlgApp()
{
    delete ui;
}
