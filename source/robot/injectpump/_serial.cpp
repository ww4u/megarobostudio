#include "injectpump.h"

int robotInject::serialIn( QXmlStreamReader &reader )
{
    while(reader.readNextStartElement())
    {
        if ( reader.name() == "raw" )
        { serialInRaw(reader); }
        else if ( reader.name() == "zero" )
        { serialInZero(reader); }
        else if ( reader.name() == "prop" )
        { serialInProp(reader); }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}
int robotInject::serialOut( QXmlStreamWriter &writer )
{
    writer.writeStartElement("raw");
    serialOutRaw(writer);
    writer.writeEndElement();

    writer.writeStartElement("zero");
    serialOutZero(writer);
    writer.writeEndElement();

    writer.writeStartElement("prop");
    serialOutProp(writer);
    writer.writeEndElement();

    return 0;
}

int robotInject::serialOutZero( QXmlStreamWriter &writer)
{
    writer.writeTextElement( "time", QString::number(mZeroTime) );
    writer.writeTextElement( "distance", QString::number(mZeroDistance) );

    writer.writeTextElement( "vgap_time", QString::number(mVGapTime) );
    writer.writeTextElement( "vgap_distance", QString::number(mVGapDistance) );

    writer.writeTextElement( "igap_time", QString::number(mIGapTime) );
    writer.writeTextElement( "igap_distance", QString::number(mIGapDistance) );

    return 0;
}
int robotInject::serialInZero( QXmlStreamReader &reader )
{
    while(reader.readNextStartElement())
    {
        if ( reader.name() == "time" )
        { mZeroTime = reader.readElementText().toDouble(); }
        else if ( reader.name() == "distance" )
        { mZeroDistance = reader.readElementText().toDouble(); }
        else if ( reader.name() == "vgap_time" )
        { mVGapTime = reader.readElementText().toDouble(); }
        else if ( reader.name() == "vgap_distance" )
        { mVGapDistance = reader.readElementText().toDouble(); }
        else if ( reader.name() == "igap_time" )
        { mIGapTime = reader.readElementText().toDouble(); }
        else if ( reader.name() == "igap_distance" )
        { mIGapDistance = reader.readElementText().toDouble(); }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int robotInject::serialOutProp( QXmlStreamWriter &writer)
{
    writer.writeTextElement( "door_time", QString::number(mProp.mDoor.mTime) );
    writer.writeTextElement( "door_distance", QString::number(mProp.mDoor.mDist) );

    writer.writeTextElement( "drink_time", QString::number(mProp.mDrink.mTime) );
    writer.writeTextElement( "drink_distance", QString::number(mProp.mDrink.mDist) );

    writer.writeTextElement( "spit_time", QString::number(mProp.mSpit.mTime) );
    writer.writeTextElement( "spit_distance", QString::number(mProp.mSpit.mDist) );

    return 0;
}
int robotInject::serialInProp( QXmlStreamReader &reader )
{
    while(reader.readNextStartElement())
    {
        if ( reader.name() == "door_time" )
        { mProp.mDoor.mTime = reader.readElementText().toDouble(); }
        else if ( reader.name() == "door_distance" )
        { mProp.mDoor.mDist = reader.readElementText().toDouble(); }

        else if ( reader.name() == "drink_time" )
        { mProp.mDrink.mTime = reader.readElementText().toDouble(); }
        else if ( reader.name() == "drink_distance" )
        { mProp.mDrink.mDist = reader.readElementText().toDouble(); }

        else if ( reader.name() == "spit_time" )
        { mProp.mSpit.mTime = reader.readElementText().toDouble(); }
        else if ( reader.name() == "spit_distance" )
        { mProp.mSpit.mDist = reader.readElementText().toDouble(); }

        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}
