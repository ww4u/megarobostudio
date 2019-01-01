#include "imageentity.h"
#include "ui_imageentity.h"

#include "../robot/robotfact.h"

ImageEntity::ImageEntity(QWidget *parent) :
    EntityWidget(parent),
    ui( new Ui::ImageEntity )
{
    ui->setupUi(this);

    mEntityType = EntityWidget::entity_widget_image;
}

ImageEntity::~ImageEntity()
{
    delete ui;
}


int ImageEntity::serialOut( QXmlStreamWriter &writer, QList<EntityWidget *> &refWidgets )
{
    int ret = 0;
    writer.writeStartElement( "base" );
    ret = EntityWidget::serialOut( writer, refWidgets );
    writer.writeEndElement();

    writer.writeStartElement( "sub" );

        writer.writeTextElement( "label", ui->lineEdit->text() );
        writer.writeTextElement( "image", mIconName );
        writer.writeTextElement( "data_class", mDataClass );
    writer.writeEndElement();

    return ret;
}

QString ImageEntity::description()
{ return ui->lineEdit->text(); }

int ImageEntity::serialIn( QXmlStreamReader &reader )
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
                else if ( reader.name() == "image" )
                {  setImage( reader.readElementText()); }
                else if ( reader.name() == "data_class" )
                {  setDataClass( reader.readElementText()); }
                else
                { reader.skipCurrentElement(); }
            }
        }
        else
        { reader.skipCurrentElement(); }
    }

    return ret;
}

void ImageEntity::setImage( const QString &imgName )
{
    mIconName = imgName;
    if ( mIconName.length() > 0 )
    { ui->label->setPixmap( QPixmap::fromImage( QImage(imgName) ) ); }
}

void ImageEntity::setDataClass( const QString &dataCls )
{
    mDataClass = dataCls;
    if ( mDataClass.length() > 0 )
    {
         VRobot *pNewRobot = robotFact::createRobot( mDataClass );
         if ( NULL != pNewRobot )
         {
             ui->label->setPixmap( QPixmap::fromImage( pNewRobot->getImage() ) );
             delete pNewRobot;
         }
         else
         {}
    }
}

void ImageEntity::setLabel( const QString &lab )
{ ui->lineEdit->setText( lab); }
QString ImageEntity::label()
{ return ui->lineEdit->text(); }

