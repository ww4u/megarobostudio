
#include "sinanju.h"

int robotSinanju::serialIn( QXmlStreamReader &reader )
{
    int ret;

    while(reader.readNextStartElement())
    {logDbg()<<reader.name();
        if ( reader.name() == "raw" )
        { ret = serialInRaw( reader ); }
        else if ( reader.name() == "angle" )
        { logDbg();ret = serialInAngle( reader ); }
        else if ( reader.name() == "hand_zero" )
        { logDbg();ret = serialInHandZero( reader ); }
        else if ( reader.name() == "arm" )
        { logDbg();ret = serialInArm( reader ); }
        else if ( reader.name() == "init_pos" )
        { logDbg();ret = serialInInitPos( reader ); }
        else if ( reader.name() == "hand" )
        { logDbg();ret = mHandActionModel.serialIn( reader ); }
        else
        {}
    }
    return ret;
}
int robotSinanju::serialOut( QXmlStreamWriter &writer )
{
    int ret;

    writer.writeStartElement("raw");
    ret = serialOutRaw( writer );
    writer.writeEndElement();

    writer.writeStartElement("angle");
    ret = serialOutAngle( writer );
    writer.writeEndElement();

    writer.writeStartElement("hand_zero");
    ret = serialOutHandZero( writer );
    writer.writeEndElement();

    writer.writeStartElement("arm");
    ret = serialOutArm( writer );
    writer.writeEndElement();

    writer.writeStartElement("init_pos");
    ret = serialOutInitPos( writer );
    writer.writeEndElement();

    writer.writeStartElement("hand");
    ret = mHandActionModel.serialOut( writer );
    writer.writeEndElement();

    return ret;
}

int robotSinanju::serialInAngle( QXmlStreamReader &reader )
{
//    while(reader.readNextStartElement())
    {
//        if ( reader.name() == "angle" )
        {
            while(reader.readNextStartElement())
            {
                if ( reader.name() == "init" )
                {
                    float fVal;
                    while(reader.readNextStartElement())
                    {
                        if ( reader.name() == "count" )
                        {
                            reader.readElementText();
                            mInitAngles.clear();
                        }
                        else if ( reader.name() == "value" )
                        {
                            fVal = reader.readElementText().toFloat();
                            mInitAngles.append( fVal );
                        }
                        else
                        {}
                    }
                }
                else if ( reader.name() == "invert" )
                {
                    int iVal;
                    while(reader.readNextStartElement())
                    {
                        if ( reader.name() == "count" )
                        {
                            reader.readElementText();
                            mAngleDir.clear();
                        }
                        else if ( reader.name() == "value" )
                        {
                            iVal = reader.readElementText().toInt();
                            mAngleDir.append( iVal > 0 );
                        }
                        else
                        {}
                    }
                }
            }
        }
    }

    return 0;
}
int robotSinanju::serialOutAngle( QXmlStreamWriter &writer )
{

    //! init
    writer.writeStartElement("init");

    writer.writeTextElement("count", QString::number(mInitAngles.size()));
    for ( int i = 0; i < mInitAngles.size(); i++ )
    {
        writer.writeTextElement("value", QString::number(mInitAngles.at(i)));
    }

    writer.writeEndElement();

    //! invert
    writer.writeStartElement("invert");

    writer.writeTextElement("count", QString::number(mAngleDir.size()));
    for ( int i = 0; i < mAngleDir.size(); i++ )
    {
        writer.writeTextElement("value", QString::number( mAngleDir.at(i) ));
    }

    writer.writeEndElement();

    return 0;
}


int robotSinanju::serialInHandZero( QXmlStreamReader &reader )
{
    while(reader.readNextStartElement())
    {
        if ( reader.name() == "time" )
        {
            mHandZeroTime = reader.readElementText().toDouble();
        }
        else if ( reader.name() == "angle" )
        {
            mHandZeroAngle = reader.readElementText().toDouble();
        }
        else if ( reader.name() == "speed" )
        {
            mHandZeroSpeed = reader.readElementText().toDouble();
        }
        else
        {}
    }

    return 0;
}

int robotSinanju::serialOutHandZero( QXmlStreamWriter &writer )
{
    writer.writeTextElement( "time", QString::number( mHandZeroTime));
    writer.writeTextElement( "angle", QString::number( mHandZeroAngle));
    writer.writeTextElement( "speed", QString::number( mHandZeroSpeed));
    return 0;
}

int robotSinanju::serialInArm( QXmlStreamReader &reader )
{
    while(reader.readNextStartElement())
    {
        if ( reader.name() == "base" )
        {
            mArmLengths[0] = reader.readElementText().toDouble();
        }
        else if ( reader.name() == "big_arm" )
        {
            mArmLengths[1] = reader.readElementText().toDouble();
        }
        else if ( reader.name() == "little_arm" )
        {
            mArmLengths[2] = reader.readElementText().toDouble();
        }
        else
        {}
    }

    return 0;
}
int robotSinanju::serialOutArm( QXmlStreamWriter &writer )
{
    writer.writeTextElement( "base", QString::number(mArmLengths.at(0)));
    writer.writeTextElement( "big_arm", QString::number(mArmLengths.at(1)));
    writer.writeTextElement( "little_arm", QString::number(mArmLengths.at(2)));

    return 0;
}

int robotSinanju::serialInInitPos( QXmlStreamReader &reader )
{
    while(reader.readNextStartElement())
    {
        if ( reader.name() == "x" )
        {
            mInitPos[0] = reader.readElementText().toDouble();
        }
        else if ( reader.name() == "y" )
        {
            mInitPos[1] = reader.readElementText().toDouble();
        }
        else if ( reader.name() == "z" )
        {
            mInitPos[2] = reader.readElementText().toDouble();
        }
        else
        {}
    }

    return 0;
}
int robotSinanju::serialOutInitPos( QXmlStreamWriter &writer )
{
    writer.writeTextElement( "x", QString::number(mInitPos.at(0)));
    writer.writeTextElement( "y", QString::number(mInitPos.at(1)));
    writer.writeTextElement( "z", QString::number(mInitPos.at(2)));

    return 0;
}
