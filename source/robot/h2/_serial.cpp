#include "h2.h"

int robotH2::serialIn( QXmlStreamReader &reader )
{
    while(reader.readNextStartElement())
    {
        if ( reader.name() == "raw" )
        { serialInRaw(reader); }
        else if ( reader.name() == "zero" )
        { serialInZero(reader); }
        else if ( reader.name() == "arm" )
        { serialInArm(reader); }
        else if ( reader.name() == "transfer" )
        { serialInTransfer(reader); }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}
int robotH2::serialOut( QXmlStreamWriter &writer )
{
    writer.writeStartElement("raw");
    serialOutRaw(writer);
    writer.writeEndElement();

    writer.writeStartElement("zero");
    serialOutZero(writer);
    writer.writeEndElement();

    writer.writeStartElement("arm");
    serialOutArm( writer );
    writer.writeEndElement();

    writer.writeStartElement("transfer");
    serialOutTransfer( writer );
    writer.writeEndElement();

    return 0;
}

int robotH2::serialOutZero( QXmlStreamWriter &writer)
{
    writer.writeTextElement( "time", QString::number(mZeroTime) );
    writer.writeTextElement( "distance", QString::number(mZeroDistance) );

    writer.writeTextElement( "gap_time", QString::number(mGapTime) );
    writer.writeTextElement( "gap_distance", QString::number(mGapDistance) );

    writer.writeTextElement( "gap_z_time", QString::number(mGapZTime) );
    writer.writeTextElement( "gap_z_distance", QString::number(mGapZDistance) );

    writer.writeTextElement( "x", QString::number(mZeroX) );
    writer.writeTextElement( "y", QString::number(mZeroY) );

    writer.writeTextElement( "movement", QString::number( mZeroMovement) );
    writer.writeTextElement( "corner", QString::number( mZeroCorner) );

    return 0;
}
int robotH2::serialInZero( QXmlStreamReader &reader )
{
    while(reader.readNextStartElement())
    {
        if ( reader.name() == "time" )
        { mZeroTime = reader.readElementText().toDouble(); }
        else if ( reader.name() == "distance" )
        { mZeroDistance = reader.readElementText().toDouble(); }
        else if ( reader.name() == "gap_time" )
        { mGapTime = reader.readElementText().toDouble(); }
        else if ( reader.name() == "gap_distance" )
        { mGapDistance = reader.readElementText().toDouble(); }
        else if ( reader.name() == "gap_z_time" )
        { mGapZTime = reader.readElementText().toDouble(); }
        else if ( reader.name() == "gap_z_distance" )
        { mGapZDistance = reader.readElementText().toDouble(); }
        else if ( reader.name() == "x" )
        { mZeroX = reader.readElementText().toDouble(); }
        else if ( reader.name() == "y" )
        { mZeroY = reader.readElementText().toDouble(); }
        else if ( reader.name() == "movement" )
        { mZeroMovement = (H2ZeroMovement)reader.readElementText().toInt(); }
        else if ( reader.name() == "corner" )
        { mZeroCorner = (H2ZeroCorner)reader.readElementText().toInt(); }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int robotH2::serialOutArm( QXmlStreamWriter &writer)
{
    for ( int i = 0; i < mArmLengths.size(); i++ )
    {
        writer.writeTextElement( "value", QString::number( mArmLengths.at(i) ) );
    }

    return 0;
}
int robotH2::serialInArm( QXmlStreamReader &reader )
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

int robotH2::serialOutTransfer( QXmlStreamWriter &writer)
{
//    writer.writeTextElement( "enable", QString::number( mbTransferAble ) );

    writer.writeStartElement("rotate");
        for ( int i = 0; i < sizeof_array( mTransferR ); i++ )
        {
            writer.writeTextElement( "r", QString::number( mTransferR[i] ) );
        }
    writer.writeEndElement();

    writer.writeStartElement("shift");
        for ( int i = 0; i < sizeof_array( mTransferS ); i++ )
        {
            writer.writeTextElement( "r", QString::number( mTransferS[i] ) );
        }
    writer.writeEndElement();

    writer.writeStartElement("rotate_inv");
        for ( int i = 0; i < sizeof_array( mTransferRInv ); i++ )
        {
            writer.writeTextElement( "r", QString::number( mTransferRInv[i] ) );
        }
    writer.writeEndElement();

    return 0;
}

int robotH2::serialInTransfer( QXmlStreamReader &reader)
{
    int ret;
    while ( reader.readNextStartElement() )
    {
//        if ( reader.name() == "enable" )
//        { mbTransferAble = toBool( reader ); }
//        else
        if ( reader.name() == "rotate" )
        {
            ret = serialInTransferR( reader );
            if ( ret != 0 )
            { return ret; }
        }
        else if ( reader.name() == "shift" )
        {
            ret = serialInTransferS( reader );
            if ( ret != 0 )
            { return ret; }
        }
        else if ( reader.name() == "rotate_inv" )
        {
            ret = serialInTransferRInv( reader );
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

int robotH2::serialInTransferR( QXmlStreamReader &reader)
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

int robotH2::serialInTransferRInv( QXmlStreamReader &reader)
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

int robotH2::serialInTransferS( QXmlStreamReader &reader)
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
