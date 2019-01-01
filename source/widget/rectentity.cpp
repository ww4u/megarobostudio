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

int RectEntity::serialOut( QXmlStreamWriter &writer, QList<EntityWidget *> &refWidgets )
{
    int ret = 0;
    writer.writeStartElement( "base" );
    ret = EntityWidget::serialOut( writer, refWidgets );
    writer.writeEndElement();

    writer.writeStartElement( "sub" );

        writer.writeTextElement( "label", ui->lineEdit->text() );

    writer.writeEndElement();

    return ret;
}
int RectEntity::serialIn( QXmlStreamReader &reader )
{
    int ret = 0;
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "base" )
        {
            ret = EntityWidget::serialIn( reader );
        }
        else if ( reader.name() == "sub" )
        {
            while( reader.readNextStartElement() )
            {
                if ( reader.name() == "label" )
                { ui->lineEdit->setText( reader.readElementText()); }
                else
                { reader.skipCurrentElement(); }
            }
        }
        else
        { reader.skipCurrentElement(); }
    }

    return ret;
}

QString RectEntity::description()
{ return ui->lineEdit->text(); }

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
