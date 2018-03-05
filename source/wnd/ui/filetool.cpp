#include "filetool.h"
#include "ui_filetool.h"

fileTool::fileTool(QWidget *parent) :
    QToolBar(parent),
    ui(new Ui::fileTool)
{
    ui->setupUi(this);
}

fileTool::~fileTool()
{
    delete ui;
}
