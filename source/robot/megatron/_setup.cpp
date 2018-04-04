
#include "megatron.h"

int robotMegatron::serialIn( QXmlStreamReader &reader )
{
    while(reader.readNextStartElement())
    {
        if ( reader.name() == "zero" )
        {
            serialInZero(reader);
        }
    }

    return 0;
}
int robotMegatron::serialOut( QXmlStreamWriter &writer )
{
    writer.writeStartElement("zero");

    serialOutZero(writer);

    writer.writeEndElement();

    return 0;
}

int robotMegatron::serialOutZero( QXmlStreamWriter &writer)
{
    writer.writeTextElement( "time", QString::number(mZeroTime) );
    writer.writeTextElement( "angle", QString::number(mZeroAngle) );
    writer.writeTextElement( "speed", QString::number(mZeroSpeed) );

    return 0;
}
int robotMegatron::serialInZero( QXmlStreamReader &reader )
{
    while(reader.readNextStartElement())
    {
        if ( reader.name() == "time" )
        { mZeroTime = reader.readElementText().toDouble(); }
        else if ( reader.name() == "angle" )
        { mZeroAngle = reader.readElementText().toDouble(); }
        if ( reader.name() == "speed" )
        { mZeroSpeed = reader.readElementText().toDouble(); }
        else
        {}
    }

    return 0;
}
