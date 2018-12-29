#include "rectentity.h"
#include "ui_rectentity.h"


RectEntity::RectEntity(QWidget *parent) :
    EntityWidget(parent),
    ui(new Ui::RectEntity)
{
    ui->setupUi(this);

    mEntityType = EntityWidget::entity_widget_rect;
}

RectEntity::~RectEntity()
{
    delete ui;
}

void RectEntity::setLabel( const QString &lab )
{
    ui->lineEdit->setText( lab );
}
QString RectEntity::label()
{ return ui->lineEdit->text(); }

//void RectEntity::paintEvent( QPaintEvent *event )
//{
//    QPainter painter(this);

//    paintFrame( painter );
//}
