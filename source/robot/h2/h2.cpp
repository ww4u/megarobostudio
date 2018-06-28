
#include "h2.h"

#include "../../device/board/_MRQ_enum.h"

static quint8 _megaimage[]=
{
    #include "./robot.cpp"
};

static quint8 _detail[]=
{
    #include "./detail.cpp"
};

robotH2::robotH2()
{
    mClass = QObject::tr("MRX-H2");
    mId = robot_h2;

    //! default gpid
    mCanGroupId = group_id_from + (mId - robot_complex)*group_segment;

    setAxes( 2 );
    setAxesDefName( 2 );
    setJointName( 2 );

    setPoseCount( 2 );
    mPoseTitles.clear();
    mPoseTitles<<"X"<<"Y";

    mDetail = QString::fromLocal8Bit( (char*)_detail, sizeof_array(_detail) );

    //! joint name
    mJointName.clear();
    mJointName<<QObject::tr("Left")
              <<QObject::tr("Right");

    mImage = QImage::fromData( _megaimage, sizeof(_megaimage) );

    mArmLengths.clear();
    mArmLengths<<13.4<<13.4<<802<<494<<52<<38;

    //! mask
    mJointCcwMask.clear();
    mJointCcwMask<<false<<false;

    mJointZeroCcw.clear();
    mJointZeroCcw<<true<<true;

    mZeroX = 32.7;
    mZeroY = 0;

    mAngleDir.clear();
//    mAngleDir<<true<<true;                 //! x:  y:
    mAngleDir<<false<<false;                 //! x:  y:

    mAxesDirs.clear();
    mAxesDirs<<-1<<-1;

    //! debug used
    //! alter the axes name
    mAxesConnectionName[0] = "CH1@device1"; //! Left
    mAxesConnectionName[1] = "CH2@device1"; //! Right

    mZeroTime = 1;
    mZeroDistance = 10;
    mZeroSpeed = 5;

    mGapTime = 1;
    mGapDistance = 10;        //! dist

    mGapZTime = 1;
    mGapZDistance = 10;

    m_pRoboTask = new H2Task();
    Q_ASSERT( NULL != m_pRoboTask );

    mLines = 1000;
    mEncoderDirs<<1<<1;
}

robotH2::~robotH2()
{

}

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

int robotH2::download( VRobot *pSetup )
{
    Q_ASSERT( NULL != pSetup );

    return 0;
}





