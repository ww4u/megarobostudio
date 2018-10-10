#include "motors.h"

int robotMotors::serialIn( QXmlStreamReader &reader )
{
    while(reader.readNextStartElement())
    {
        if ( reader.name() == "raw" )
        { serialInRaw(reader); }
        else if ( reader.name() == "joints" )
        { serialInJoints(reader); }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int robotMotors::serialOut( QXmlStreamWriter &writer )
{
    writer.writeStartElement("raw");
    serialOutRaw(writer);
    writer.writeEndElement();

    writer.writeStartElement("joints");
    serialOutJoints(writer);
    writer.writeEndElement();

    return 0;
}

QAbstractTableModel *robotMotors::getModel()
{
    return &mModel;
}

int robotMotors::serialOutJoints( QXmlStreamWriter &writer)
{
    for ( int i = 0; i < mModel.mItems.size(); i++ )
    {
        writer.writeStartElement("joint");
        serialOutSubJoint( writer, mModel.mItems.at(i) );
        writer.writeEndElement();
    }

    return 0;
}
int robotMotors::serialInJoints( QXmlStreamReader &reader )
{
    Relation *pRel;
    while(reader.readNextStartElement())
    {
        if ( reader.name() == "joint" )
        {
            pRel = new Relation();
            serialInSubJoint( reader, pRel );
            mModel.mItems.append( pRel );
        }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int robotMotors::serialOutSubJoint( QXmlStreamWriter &writer, Relation *pRel )
{
    Q_ASSERT( NULL != pRel );
    writer.writeTextElement( "name", pRel->name() );
    writer.writeTextElement( "value", pRel->value() );

    return 0;
}
int robotMotors::serialInSubJoint( QXmlStreamReader &reader, Relation *pRel )
{
    Q_ASSERT( NULL != pRel );
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "name" )
        { pRel->setName( reader.readElementText()); }
        else if ( reader.name() == "value" )
        { pRel->setValue( reader.readElementText()); }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}
