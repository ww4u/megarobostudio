#include "mrggraphmodel.h"

#include "../ui/robograph.h"    //! create factory

MrgGraphModel::MrgGraphModel()
{

}

int MrgGraphModel::load( const QString &fileName,
                         RoboGraph *parent )
{
    //! load from file
    QFile file( fileName );
    if ( file.open( QIODevice::ReadOnly ) )
    {}
    else
    { return -1; }

    //! read from file
    QXmlStreamReader reader( &file );
    int ret = doLoad( reader, parent );

    //! relink
    ret = crossLink();

    return ret;
}
int MrgGraphModel::save( const QString &outFileName )
{
    QFile file(outFileName);
    if ( file.open( QIODevice::WriteOnly ) )
    {}
    else
    { logDbg(); return -1; }

    QXmlStreamWriter writer( &file );

    writer.writeStartDocument();

    writer.writeStartElement("graph");
    for ( int i = 0; i < mChildWidgets.size(); i++ )
    {
        writer.writeStartElement("entity");
            writer.writeAttribute( "class", QString::number( (int)mChildWidgets.at(i)->entityType()));
            mChildWidgets.at(i)->serialOut( writer, mChildWidgets );
        writer.writeEndElement();
    }
    writer.writeEndElement();

    writer.writeEndDocument();

    file.close();

    return 0;
}

void MrgGraphModel::clean()
{
    delete_all( mChildWidgets );
}

int MrgGraphModel::doLoad( QXmlStreamReader &reader,
                           RoboGraph *parent )
{
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "graph" )
        {
            while( reader.readNextStartElement() )
            {
                if ( reader.name() == "entity" )
                {
                    //! valid class
                    if ( reader.attributes().hasAttribute("class") )
                    {
                        int clsId;
                        bool bOk;

                        clsId = reader.attributes().value("class").toInt( &bOk );
                        if ( bOk )
                        {
                            //! create the entity
                            EntityWidget *pWig = RoboGraph::createEntityWidget( (EntityWidget::EntityWidgetType)clsId,
                                                                                parent );

                            if ( NULL != pWig )
                            {
                                pWig->serialIn( reader );
                                mChildWidgets.append( pWig );
                            }
                            else
                            { reader.skipCurrentElement(); }
                        }
                        else
                        { reader.skipCurrentElement(); }
                    }
                    else
                    { reader.skipCurrentElement(); }
                }
                else
                { reader.skipCurrentElement(); }
            }
        }
        else
        {
            reader.skipCurrentElement();
        }
    }

    return 0;
}

//! cross link each widget
int MrgGraphModel::crossLink( )
{
    for ( int i = 0; i < mChildWidgets.size(); i++ )
    {
        mChildWidgets[i]->crossLink( mChildWidgets );
    }

    return 0;
}
