#include "mfiledialog.h"
#include "ui_mfiledialog.h"
#include <QtWidgets>
MFileDialog::MFileDialog() :
    QDialog(),
    ui(new Ui::MFileDialog)
{
    ui->setupUi(this);
//    QLabel *plabel = new QLabel( "hello", this );
}

MFileDialog::~MFileDialog()
{

}
