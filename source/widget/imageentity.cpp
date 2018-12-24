#include "imageentity.h"
#include "ui_imageentity.h"

ImageEntity::ImageEntity(QWidget *parent) :
    EntityWidget(parent),
    ui(new Ui::ImageEntity)
{
    ui->setupUi(this);

    setGeometry( 0,0,100,100);
}

ImageEntity::~ImageEntity()
{
    delete ui;
}

void ImageEntity::setImage( const QImage &img )
{
    ui->label->setPixmap( QPixmap::fromImage( img) );
}

void ImageEntity::setLabel( const QString &lab )
{ ui->lineEdit->setText( lab); }
QString ImageEntity::label()
{ return ui->lineEdit->text(); }

