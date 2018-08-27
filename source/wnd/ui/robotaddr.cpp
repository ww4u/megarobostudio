#include "robotaddr.h"
#include "ui_robotaddr.h"

RobotAddr::RobotAddr(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RobotAddr)
{
    ui->setupUi(this);
}

RobotAddr::~RobotAddr()
{
    delete ui;
}

void RobotAddr::setAlias( QString &str )
{ ui->edtAlias->setText(str); }
QString RobotAddr::alias()
{ return ui->edtAlias->text(); }

void RobotAddr::setGroupSel( int gp )
{ ui->cmbGp->setCurrentIndex(gp); }
int RobotAddr::groupSel()
{ return ui->cmbGp->currentIndex(); }

void RobotAddr::setGroupId( int id )
{ ui->spinBox->setValue(id); }
int RobotAddr::groupId()
{ return ui->spinBox->value(); }

void RobotAddr::setSubGroupIndex( int id )
{ ui->cmbGroup->setCurrentIndex(id); }
int RobotAddr::subGroupIndex()
{ return ui->cmbGroup->currentIndex(); }

void RobotAddr::setSubGroupId( int id )
{ ui->spinSubGpDec->setValue(id); }
int RobotAddr::subGroupId()
{ return ui->spinSubGpDec->value(); }
