#include "login.h"
#include "ui_login.h"

LogIn::LogIn(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogIn)
{
    ui->setupUi(this);
}

LogIn::~LogIn()
{
    delete ui;
}

int LogIn::getUserRole()
{
    return ui->comboBox->currentIndex();
}

bool LogIn::getAutoLogin()
{ return ui->chkAutoLogin->isChecked(); }
