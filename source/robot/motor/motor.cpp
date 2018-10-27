
#include "motor.h"

static quint8 _megaimage[]=
{
    #include "./robot.cpp"
};

static quint8 _detail[]=
{
    #include "./detail.cpp"
};

robotMotor::robotMotor()
{
    mClass = "MRX-MOTOR";
    mId = robot_motor;

    //! default gpid
    mCanGroupId = group_id_from + (mId - robot_complex)*group_segment;

    setAxes( 1 );
    setAxesDefName( 1 );
    setJointName( 1 );

    mImage = QImage::fromData( _megaimage, sizeof(_megaimage) );
    setJointPanelAble( true );
    setRoboPanelAble( false );

    mDetail = QString::fromLocal8Bit( (char*)_detail, sizeof_array(_detail) );

    //! joint name
    mJointName.clear();
    mJointName<<QObject::tr("Motor");

    mJointCcwMask.clear();
    mJointCcwMask<<true;

    mJointZeroCcw.clear();
    mJointZeroCcw<<true;

    //! debug used
    //! alter the axes name
    Q_ASSERT( mAxesConnectionName.size() == 1 );
    mAxesConnectionName[0] = "CH1@device1"; //! ax

    mZeroTime = 5.0;
    mZeroDistance = 5.0;

    mGapTime = 1.0;
    mGapDistance = 2.0;

    mbZeroPosEn = true;
    mbGapPosEn = true;
    mPosIndex = 0;

    m_pRoboTask = new MotorTask();
    Q_ASSERT( NULL != m_pRoboTask );
}

int robotMotor::serialIn( QXmlStreamReader &reader )
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

int robotMotor::serialOut( QXmlStreamWriter &writer )
{
    writer.writeStartElement("raw");
    serialOutRaw(writer);
    writer.writeEndElement();

    writer.writeStartElement("zero");
    serialOutZero(writer);
    writer.writeEndElement();

    return 0;
}

int robotMotor::serialOutZero( QXmlStreamWriter &writer)
{
    writer.writeTextElement( "time", QString::number(mZeroTime) );
    writer.writeTextElement( "distance", QString::number(mZeroDistance) );

    writer.writeTextElement( "gap_time", QString::number(mGapTime) );
    writer.writeTextElement( "gap_distance", QString::number(mGapDistance) );

    writer.writeTextElement( "pos", QString::number(mPosIndex) );
    writer.writeTextElement( "pos_zero_en", QString::number(mbZeroPosEn) );
    writer.writeTextElement( "pos_gap_en", QString::number(mbGapPosEn) );

    return 0;
}
int robotMotor::serialInZero( QXmlStreamReader &reader )
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

        else if ( reader.name() == "pos" )
        { mPosIndex = reader.readElementText().toInt(); }
        else if ( reader.name() == "pos_zero_en" )
        { mbZeroPosEn = reader.readElementText().toInt() > 0; }
        else if ( reader.name() == "pos_gap_en" )
        { mbGapPosEn = reader.readElementText().toInt() > 0; }

        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}
