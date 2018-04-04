
#include "sinanju.h"

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

