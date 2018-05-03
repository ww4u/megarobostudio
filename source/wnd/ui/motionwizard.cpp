#include "motionwizard.h"
#include "ui_motionwizard.h"

MotionWizard::MotionWizard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MotionWizard)
{
    ui->setupUi(this);

    ui->listWidget->setCurrentRow( 0 );
}

MotionWizard::~MotionWizard()
{
    delete ui;
}

QString MotionWizard::motionName()
{
    return ui->listWidget->currentItem()->text().toLower();
}

void MotionWizard::on_listWidget_currentTextChanged(const QString &currentText)
{
    ui->label->setPixmap( QPixmap( QString(":/res/image/joint/%1.png").arg(currentText.toLower())) );
}
