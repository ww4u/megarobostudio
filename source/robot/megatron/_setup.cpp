
#include "megatron.h"

int robotMegatron::serialIn( QXmlStreamReader &reader )
{
    while(reader.readNextStartElement())
    {
        if ( reader.name() == "raw" )
        { serialInRaw(reader); }
        else if ( reader.name() == "zero" )
        { serialInZero(reader); }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}
int robotMegatron::serialOut( QXmlStreamWriter &writer )
{
    writer.writeStartElement("raw");
    serialOutRaw(writer);
    writer.writeEndElement();

    writer.writeStartElement("zero");
    serialOutZero(writer);
    writer.writeEndElement();

    return 0;
}

int robotMegatron::serialOutZero( QXmlStreamWriter &writer)
{
    writer.writeTextElement( "time", QString::number(mZeroTime) );
    writer.writeTextElement( "angle", QString::number(mZeroAngle) );

    writer.writeTextElement( "gap_time", QString::number(mGapTime) );
    writer.writeTextElement( "gap_angle", QString::number(mGapDistance) );
    writer.writeTextElement( "gap_speed", QString::number(mGapSpeed) );

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

        else if ( reader.name() == "gap_time" )
        { mGapTime = reader.readElementText().toDouble(); }
        else if ( reader.name() == "gap_angle" )
        { mGapDistance = reader.readElementText().toDouble(); }
        else if ( reader.name() == "gap_speed" )
        { mGapSpeed = reader.readElementText().toDouble(); }

        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}
