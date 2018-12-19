#include "rotmat3_3.h"
#include "ui_rotmat3_3.h"

RotMat3_3::RotMat3_3(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RotMat3_3)
{
    ui->setupUi(this);
}

RotMat3_3::~RotMat3_3()
{
    delete ui;
}

void RotMat3_3::changeEvent(QEvent * event)
{
    QWidget::changeEvent( event );

    if (event->type() == QEvent::LanguageChange)
    { ui->retranslateUi( this ); }
}

void RotMat3_3::setRotateMatrix( double rot[3*3] )
{}
void RotMat3_3::roateMatrix( double rot[3*3] )
{}
