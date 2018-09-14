#include "igus_delta.h"

int robotIgusDelta::serialIn( QXmlStreamReader &reader )
{
    int ret = 0;
    while(reader.readNextStartElement())
    {
        if ( reader.name() == "raw" )
        { ret = serialInRaw( reader ); }
        else if ( reader.name() == "zero" )
        { ret = serialInZero(reader); }
        else if ( reader.name() == "init" )
        { ret = serialInInit(reader); }

        else if ( reader.name() == "arm" )
        { ret = serialInArm(reader); }
        else if ( reader.name() == "offset" )
        { ret = serialInOffset(reader); }

        else if ( reader.name() == "p0" )
        { ret = serialInP0(reader); }

        else if ( reader.name() == "poslim" )
        { ret = serialInPosLim(reader); }

        else if ( reader.name() == "misc" )
        { ret = serialInMisc(reader); }
        else
        { reader.skipCurrentElement(); }
    }

    return ret;
}
int robotIgusDelta::serialOut( QXmlStreamWriter &writer )
{
    int ret = 0;

    writer.writeStartElement("raw");
    ret = serialOutRaw( writer );
    writer.writeEndElement();

    writer.writeStartElement("zero");
    ret = serialOutZero(writer);
    writer.writeEndElement();

    writer.writeStartElement("init");
    ret = serialOutInit(writer);
    writer.writeEndElement();

    writer.writeStartElement("arm");
    ret = serialOutArm(writer);
    writer.writeEndElement();

    writer.writeStartElement("offset");
    ret = serialOutOffset(writer);
    writer.writeEndElement();

    writer.writeStartElement("p0");
    ret = serialOutP0(writer);
    writer.writeEndElement();

    writer.writeStartElement("poslim");
    ret = serialOutPosLim(writer);
    writer.writeEndElement();

    writer.writeStartElement("misc");
    ret = serialOutMisc(writer);
    writer.writeEndElement();

    return 0;
}

int robotIgusDelta::serialOutZero( QXmlStreamWriter &writer)
{
    writer.writeTextElement( "time", QString::number(mZeroTime) );
    writer.writeTextElement( "angle", QString::number(mZeroAngle) );
//    writer.writeTextElement( "speed", QString::number(mZeroSpeed) );

    return 0;
}
int robotIgusDelta::serialInZero( QXmlStreamReader &reader )
{
    QString str;
    while(reader.readNextStartElement())
    {
        if ( reader.name() == "time" )
        { mZeroTime = reader.readElementText().toDouble(); }
        else if ( reader.name() == "angle" )
        { mZeroAngle = reader.readElementText().toDouble(); }
//        else if ( reader.name() == "speed" )
//        { mZeroSpeed = reader.readElementText().toDouble(); }
        else
        { reader.skipCurrentElement(); }

//        if ( reader.name() == "time" )
//        {
//            str = reader.readElementText();
//            mZeroTime = str.toDouble();
//            logDbg()<<str;
//        }
//        else if ( reader.name() == "angle" )
//        {
//            str = reader.readElementText();
//            mZeroAngle = str.toDouble();
//            logDbg()<<str;

////            mZeroAngle = reader.readElementText().toDouble();
//        }
//        else if ( reader.name() == "speed" )
//        {
//            str = reader.readElementText();
//            mZeroSpeed = str.toDouble();
//            logDbg()<<str;
////            mZeroSpeed = reader.readElementText().toDouble();
//        }
//        else
//        { reader.skipCurrentElement(); }
    }

    return 0;
}

int robotIgusDelta::serialOutInit( QXmlStreamWriter &writer)
{
    writer.writeTextElement( "t", QString::number(mInitT) );
    writer.writeTextElement( "l", QString::number(mInitL) );
    writer.writeTextElement( "r", QString::number(mInitR) );
    writer.writeTextElement( "y", QString::number(mInitY) );
    writer.writeTextElement( "h", QString::number(mInitH) );


    return 0;
}
int robotIgusDelta::serialInInit( QXmlStreamReader &reader )
{
    while(reader.readNextStartElement())
    {
        if ( reader.name() == "t" )
        { mInitT = reader.readElementText().toDouble(); }
        else if ( reader.name() == "l" )
        { mInitL = reader.readElementText().toDouble(); }
        else if ( reader.name() == "r" )
        { mInitR = reader.readElementText().toDouble(); }
        else if ( reader.name() == "y" )
        { mInitY = reader.readElementText().toDouble(); }
        else if ( reader.name() == "h" )
        { mInitH = reader.readElementText().toDouble(); }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int robotIgusDelta::serialOutArm( QXmlStreamWriter &writer)
{
    for ( int i = 0; i < mArmLengths.size(); i++ )
    {
        writer.writeTextElement( "value", QString::number( mArmLengths.at(i) ) );
    }

    return 0;
}
int robotIgusDelta::serialInArm( QXmlStreamReader &reader )
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

int robotIgusDelta::serialOutOffset( QXmlStreamWriter &writer)
{
    for ( int i = 0; i < mOffset.size(); i++ )
    {
        writer.writeTextElement( "value", QString::number( mOffset.at(i) ) );
    }

    return 0;
}
int robotIgusDelta::serialInOffset( QXmlStreamReader &reader )
{
    int id = 0;
    while(reader.readNextStartElement())
    {
        if ( reader.name() == "value" )
        {
            mOffset[id++] = reader.readElementText().toDouble();
        }
        else
        {}
    }

    return 0;
}

int robotIgusDelta::serialOutP0( QXmlStreamWriter &writer)
{
    for ( int i = 0; i < mP0.size(); i++ )
    {
        writer.writeTextElement( "value", QString::number( mP0.at(i) ) );
    }

    return 0;
}
int robotIgusDelta::serialInP0( QXmlStreamReader &reader )
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

int robotIgusDelta::serialOutPosLim( QXmlStreamWriter &writer)
{
    for ( int i = 0; i < mPosLim.size(); i++ )
    {
        writer.writeTextElement( "value", QString::number( mPosLim.at(i) ) );
    }

    return 0;
}
int robotIgusDelta::serialInPosLim( QXmlStreamReader &reader )
{
    int id = 0;
    while(reader.readNextStartElement())
    {
        if ( reader.name() == "value" )
        {
            mPosLim[id++] = reader.readElementText().toDouble();
        }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int robotIgusDelta::serialOutMisc( QXmlStreamWriter &writer)
{
    {
        writer.writeTextElement( "scal", QString::number( mScal ) );
        writer.writeTextElement( "vm", QString::number( mVm ) );
    }

    return 0;
}
int robotIgusDelta::serialInMisc( QXmlStreamReader &reader )
{
    while(reader.readNextStartElement())
    {
        if ( reader.name() == "scal" )
        {
            mScal = reader.readElementText().toDouble();
        }
        else if ( reader.name() == "vm" )
        {
            mVm = reader.readElementText().toDouble();
        }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

