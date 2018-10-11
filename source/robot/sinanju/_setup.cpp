
#include "sinanju.h"

int robotSinanju::serialIn( QXmlStreamReader &reader )
{
    int ret;

    while(reader.readNextStartElement())
    {
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
        else if ( reader.name() == "joint_ccw_zero" )
        { ret = serialInJointCcw(reader); }
        else if ( reader.name() == "joint_factory" )
        { ret = serialInJointFactory(reader); }
        else if ( reader.name() == "transfer" )
        { ret = serialInTransfer( reader ); }
        else if ( reader.name() == "tcp" )
        { ret = serialInTcp( reader ); }
        else
        { reader.skipCurrentElement(); }
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

    writer.writeStartElement("joint_zero_ccw");
    ret = serialOutJointCcw( writer );
    writer.writeEndElement();

    writer.writeStartElement("joint_factory");
    ret = serialOutJointFactory( writer );
    writer.writeEndElement();

    writer.writeStartElement("transfer");
    ret = serialOutTransfer( writer );
    writer.writeEndElement();

    writer.writeStartElement("tcp");
    ret = serialOutTcp( writer );
    writer.writeEndElement();

    return ret;
}

int robotSinanju::serialInAngle( QXmlStreamReader &reader )
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
                { reader.skipCurrentElement(); }
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
                { reader.skipCurrentElement(); }
            }
        }
        else
        { reader.skipCurrentElement(); }
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
        if ( reader.name() == "hand" )
        {
            mbHandAble = reader.readElementText().toInt() > 0;
        }
        else if ( reader.name() == "time" )
        {
            mHandZeroTime = reader.readElementText().toDouble();
        }
        else if ( reader.name() == "angle" )
        {
            mHandZeroAngle = reader.readElementText().toDouble();
        }
        else if ( reader.name() == "gap_time" )
        {
            mGapTime = reader.readElementText().toDouble();
        }
        else if ( reader.name() == "gap_angle" )
        {
            mGapAngle = reader.readElementText().toDouble();
        }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int robotSinanju::serialOutHandZero( QXmlStreamWriter &writer )
{
    writer.writeTextElement( "hand", QString::number( mbHandAble) );
    writer.writeTextElement( "time", QString::number( mHandZeroTime));
    writer.writeTextElement( "angle", QString::number( mHandZeroAngle));

    writer.writeTextElement( "gap_time", QString::number( mGapTime));
    writer.writeTextElement( "gap_angle", QString::number( mGapAngle));

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
        { reader.skipCurrentElement(); }
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
        { reader.skipCurrentElement(); }
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

int robotSinanju::serialInJointCcw( QXmlStreamReader &reader )
{
    int iVal = 0;
    int id = 0;
    while(reader.readNextStartElement())
    {
        if ( reader.name() == "ccw" )
        {
            iVal = reader.readElementText().toInt();
            mJointZeroCcw[ id++ ] = iVal > 0;
        }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}
int robotSinanju::serialOutJointCcw( QXmlStreamWriter &writer )
{
    for ( int i = 0; i < axes(); i++ )
    {
        writer.writeTextElement( "ccw", QString::number(mJointZeroCcw.at(i)) );
    }

    return 0;
}

int robotSinanju::serialInJointFactory( QXmlStreamReader &reader )
{
    double fVal = 0;
    int id = 0;
    while(reader.readNextStartElement())
    {
        if ( reader.name() == "angle" )
        {
            fVal = reader.readElementText().toDouble();
            mJointFactoryList[ id++ ] = fVal;
        }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}
int robotSinanju::serialOutJointFactory( QXmlStreamWriter &writer )
{
    for ( int i = 0; i < 4; i++ )
    {
        writer.writeTextElement( "angle", QString::number(mJointFactoryList.at(i)) );
    }

    return 0;
}

int robotSinanju::serialInTransfer( QXmlStreamReader &reader )
{
    int ret;
    while ( reader.readNextStartElement() )
    {
        if ( reader.name() == "rotate" )
        {
            ret = serialInTransferR( reader );
            if ( ret != 0 )
            { return ret; }
        }
        else if ( reader.name() == "rotate_inv" )
        {
            ret = serialInTransferRInv( reader );
            if ( ret != 0 )
            { return ret; }
        }
        else if ( reader.name() == "shift" )
        {
            ret = serialInTransferS( reader );
            if ( ret != 0 )
            { return ret; }
        }
        else
        {
            reader.skipCurrentElement();
        }
    }

    return 0;
}

int robotSinanju::serialOutTransfer( QXmlStreamWriter &writer )
{
    writer.writeStartElement( "rotate" );
        for ( int i = 0; i < sizeof_array( mTransferR ); i++ )
        {
            writer.writeTextElement( "r", QString::number( mTransferR[i] ) );
        }
    writer.writeEndElement();

    writer.writeStartElement( "rotate_inv" );
        for ( int i = 0; i < sizeof_array( mTransferRInv ); i++ )
        {
            writer.writeTextElement( "r", QString::number( mTransferRInv[i] ) );
        }
    writer.writeEndElement();

    writer.writeStartElement( "shift" );
        for ( int i = 0; i < sizeof_array( mTransferS ); i++ )
        {
            writer.writeTextElement( "r", QString::number( mTransferS[i] ) );
        }
    writer.writeEndElement();

    return 0;
}

int robotSinanju::serialInTransferR( QXmlStreamReader &reader )
{
    int index = 0;
    while ( reader.readNextStartElement() )
    {
        if ( reader.name() == "r" )
        {
            Q_ASSERT( index < sizeof_array(mTransferR) );
            mTransferR[index++] = toDouble(reader);
        }
        else
        {
            reader.skipCurrentElement();
        }
    }

    return 0;
}

int robotSinanju::serialInTransferRInv( QXmlStreamReader &reader )
{
    int index = 0;
    while ( reader.readNextStartElement() )
    {
        if ( reader.name() == "r" )
        {
            Q_ASSERT( index < sizeof_array(mTransferRInv) );
            mTransferRInv[index++] = toDouble(reader);
        }
        else
        {
            reader.skipCurrentElement();
        }
    }

    return 0;
}

int robotSinanju::serialInTransferS( QXmlStreamReader &reader )
{
    int index = 0;
    while ( reader.readNextStartElement() )
    {
        if ( reader.name() == "r" )
        {
            Q_ASSERT( index < sizeof_array(mTransferS) );
            mTransferS[index++] = toDouble(reader);
        }
        else
        {
            reader.skipCurrentElement();
        }
    }

    return 0;
}

int robotSinanju::serialInTcp( QXmlStreamReader &reader )
{
    int index = 0;
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "p" )
        {
            index = 0;
            while( reader.readNextStartElement() )
            {
                if ( reader.name() == "v" )
                {
                    Q_ASSERT( index < sizeof_array(mTcpP) );

                    mTcpP[index++] = reader.readElementText().toDouble();
                }
                else
                { reader.skipCurrentElement(); }
            }
        }
        else if ( reader.name() == "r" )
        {
            index = 0;
            while( reader.readNextStartElement() )
            {
                if ( reader.name() == "v" )
                {
                    Q_ASSERT( index < sizeof_array(mTcpR) );

                    mTcpR[index++] = reader.readElementText().toDouble();
                }
                else
                { reader.skipCurrentElement(); }
            }
        }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int robotSinanju::serialOutTcp(QXmlStreamWriter &writer )
{
    writer.writeStartElement( "p" );
    for ( int i = 0; i < sizeof_array( mTcpP ); i++ )
    {
        writer.writeTextElement( "v", QString::number( mTcpP[i] ) );
    }
    writer.writeEndElement();

    writer.writeStartElement( "r" );
    for ( int i = 0; i < sizeof_array( mTcpR ); i++ )
    {
        writer.writeTextElement( "v", QString::number( mTcpR[i] ) );
    }
    writer.writeEndElement();

    return 0;
}
