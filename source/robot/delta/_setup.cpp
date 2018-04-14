#include "delta.h"

int robotDelta::serialIn( QXmlStreamReader &reader )
{
    int ret = 0;
    while(reader.readNextStartElement())
    {
        if ( reader.name() == "raw" )
        { ret = serialInRaw( reader ); }
        else if ( reader.name() == "zero" )
        { ret = serialInZero(reader); }
        else if ( reader.name() == "angle" )
        { ret = serialInAngle(reader); }
        else if ( reader.name() == "arm" )
        { ret = serialInArm(reader); }
        else if ( reader.name() == "range" )
        { ret = serialInRange(reader); }
        else if ( reader.name() == "p0" )
        { ret = serialInP0(reader); }
        else if ( reader.name() == "a0" )
        { ret = serialInA0(reader); }
        else
        { reader.skipCurrentElement(); }
    }

    return ret;
}
int robotDelta::serialOut( QXmlStreamWriter &writer )
{
    int ret = 0;

    writer.writeStartElement("raw");
    ret = serialOutRaw( writer );
    writer.writeEndElement();

    writer.writeStartElement("zero");
    ret = serialOutZero(writer);
    writer.writeEndElement();

    writer.writeStartElement("angle");
    ret = serialOutAngle(writer);
    writer.writeEndElement();

    writer.writeStartElement("arm");
    ret = serialOutArm(writer);
    writer.writeEndElement();

    writer.writeStartElement("range");
    ret = serialOutRange(writer);
    writer.writeEndElement();

    writer.writeStartElement("p0");
    ret = serialOutP0(writer);
    writer.writeEndElement();

    writer.writeStartElement("a0");
    ret = serialOutA0(writer);
    writer.writeEndElement();

    return 0;
}

int robotDelta::serialOutZero( QXmlStreamWriter &writer)
{
    writer.writeTextElement( "time", QString::number(mZeroTime) );
    writer.writeTextElement( "angle", QString::number(mZeroAngle) );
    writer.writeTextElement( "speed", QString::number(mZeroSpeed) );

    return 0;
}
int robotDelta::serialInZero( QXmlStreamReader &reader )
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
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int robotDelta::serialOutAngle( QXmlStreamWriter &writer)
{
//    writer.writeTextElement( "value", QString::number(mInitAngles.at(0)) );
//    writer.writeTextElement( "value", QString::number(mInitAngles.at(1)) );

    return 0;
}
int robotDelta::serialInAngle( QXmlStreamReader &reader )
{
//    int id = 0;
//    while(reader.readNextStartElement())
//    {
//        if ( reader.name() == "value" )
//        {
//            mInitAngles[id++] = reader.readElementText().toDouble();
//        }
//        else
//        {}
//    }

    return 0;
}
int robotDelta::serialOutArm( QXmlStreamWriter &writer)
{
    for ( int i = 0; i < mArmLengths.size(); i++ )
    {
        writer.writeTextElement( "value", QString::number( mArmLengths.at(i) ) );
    }

    return 0;
}
int robotDelta::serialInArm( QXmlStreamReader &reader )
{
    int id = 0;
    while(reader.readNextStartElement())
    {
        if ( reader.name() == "value" )
        {
            mArmLengths[id++] = reader.readElementText().toDouble();
        }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int robotDelta::serialOutRange( QXmlStreamWriter &writer)
{
    for ( int i = 0; i < mAngleLimit.size(); i++ )
    {
        writer.writeTextElement( "value", QString::number( mAngleLimit.at(i) ) );
    }

    return 0;
}
int robotDelta::serialInRange( QXmlStreamReader &reader )
{
    int id = 0;
    while(reader.readNextStartElement())
    {
        if ( reader.name() == "value" )
        {
            mAngleLimit[id++] = reader.readElementText().toDouble();
        }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int robotDelta::serialOutP0( QXmlStreamWriter &writer)
{
    for ( int i = 0; i < mP0.size(); i++ )
    {
        writer.writeTextElement( "value", QString::number( mP0.at(i) ) );
    }

    return 0;
}
int robotDelta::serialInP0( QXmlStreamReader &reader )
{
    int id = 0;
    while(reader.readNextStartElement())
    {
        if ( reader.name() == "value" )
        {
            mP0[id++] = reader.readElementText().toDouble();
        }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int robotDelta::serialOutA0( QXmlStreamWriter &writer)
{
    for ( int i = 0; i < mA0.size(); i++ )
    {
        writer.writeTextElement( "value", QString::number( mA0.at(i) ) );
    }

    return 0;
}
int robotDelta::serialInA0( QXmlStreamReader &reader )
{
    int id = 0;
    while(reader.readNextStartElement())
    {
        if ( reader.name() == "value" )
        {
            mA0[id++] = reader.readElementText().toDouble();
        }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

