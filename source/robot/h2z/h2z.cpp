
#include "h2z.h"

#include "../../device/board/_MRQ_enum.h"

static quint8 _megaimage[]=
{
    #include "./robot.cpp"
};

static quint8 _detail[]=
{
    #include "./detail.cpp"
};

robotH2Z::robotH2Z()
{
    mClass = QObject::tr("MRX-H2Z");
    mId = robot_h2z;

    //! default gpid
    mCanGroupId = group_id_from + (mId - robot_complex)*group_segment;

    setAxes( 3 );
    setAxesDefName( 3 );
    setJointName( 3 );

    setPoseCount( 2 );
    mPoseTitles.clear();
    mPoseTitles<<"X"<<"Y";

    mDetail = QString::fromLocal8Bit( (char*)_detail, sizeof_array(_detail) );

    //! joint name
    mJointName.clear();
    mJointName<<QObject::tr("Left")
              <<QObject::tr("Right")
              <<QObject::tr("Z");

    mImage = QImage::fromData( _megaimage, sizeof(_megaimage) );

    mArmLengths.clear();
    mArmLengths<<13.4<<13.4<<580<<494<<52<<38;

    mJointCcwMask[0] = false;
    mJointCcwMask[1] = false;
    mJointCcwMask[2] = true;

    mJointZeroCcw.clear();
    mJointZeroCcw<<true<<true<<false;

    mZeroX = 32.7;
    mZeroY = 0;

    mAngleDir.clear();
//    mAngleDir<<false<<true<<true;                 //! x:  y:
    mAngleDir<<true<<false<<true;

    mAxesDirs.clear();
    mAxesDirs<<-1<<-1<<1;

    //! debug used
    //! alter the axes name
    mAxesConnectionName[0] = "CH1@device1"; //! x
    mAxesConnectionName[1] = "CH2@device1"; //! y
    mAxesConnectionName[2] = "CH3@device1"; //! z

    mZeroTime = 1;
    mZeroDistance = 50;
    mZeroSpeed = 5;

    mGapTime = 1;
    mGapDistance = 10;      //! dist

    if ( NULL != m_pRoboTask )
    { delete m_pRoboTask; }
    m_pRoboTask = new H2ZTask();
    Q_ASSERT( NULL != m_pRoboTask );

    mLines = 1000;          //! for encoder
    mEncoderDirs.clear();
    mEncoderDirs<<1<<1<<1;
}

robotH2Z::~robotH2Z()
{

}

//int robotH2Z::serialIn( QXmlStreamReader &reader )
//{
//    while(reader.readNextStartElement())
//    {
//        if ( reader.name() == "raw" )
//        { serialInRaw(reader); }
//        else if ( reader.name() == "zero" )
//        { serialInZero(reader); }
//        else if ( reader.name() == "arm" )
//        { serialInArm(reader); }
//        else
//        { reader.skipCurrentElement(); }
//    }

//    return 0;
//}
//int robotH2Z::serialOut( QXmlStreamWriter &writer )
//{
//    writer.writeStartElement("raw");
//    serialOutRaw(writer);
//    writer.writeEndElement();

//    writer.writeStartElement("zero");
//    serialOutZero(writer);
//    writer.writeEndElement();

//    writer.writeStartElement("arm");
//    serialOutArm( writer );
//    writer.writeEndElement();

//    return 0;
//}

//int robotH2Z::serialOutZero( QXmlStreamWriter &writer)
//{
//    writer.writeTextElement( "time", QString::number(mZeroTime) );
//    writer.writeTextElement( "distance", QString::number(mZeroDistance) );

//    writer.writeTextElement( "gap_time", QString::number(mGapTime) );
//    writer.writeTextElement( "gap_distance", QString::number(mGapDistance) );

//    writer.writeTextElement( "x", QString::number(mZeroX) );
//    writer.writeTextElement( "y", QString::number(mZeroY) );

//    return 0;
//}
//int robotH2Z::serialInZero( QXmlStreamReader &reader )
//{
//    while(reader.readNextStartElement())
//    {
//        if ( reader.name() == "time" )
//        { mZeroTime = reader.readElementText().toDouble(); }
//        else if ( reader.name() == "distance" )
//        { mZeroDistance = reader.readElementText().toDouble(); }
//        else if ( reader.name() == "gap_time" )
//        { mGapTime = reader.readElementText().toDouble(); }
//        else if ( reader.name() == "gap_distance" )
//        { mGapDistance = reader.readElementText().toDouble(); }
//        else if ( reader.name() == "x" )
//        { mZeroX = reader.readElementText().toDouble(); }
//        else if ( reader.name() == "y" )
//        { mZeroY = reader.readElementText().toDouble(); }
//        else
//        { reader.skipCurrentElement(); }
//    }

//    return 0;
//}

//int robotH2Z::serialOutArm( QXmlStreamWriter &writer)
//{
//    for ( int i = 0; i < mArmLengths.size(); i++ )
//    {
//        writer.writeTextElement( "value", QString::number( mArmLengths.at(i) ) );
//    }

//    return 0;
//}
//int robotH2Z::serialInArm( QXmlStreamReader &reader )
//{
//    int id = 0;
//    while(reader.readNextStartElement())
//    {
//        if ( reader.name() == "value" )
//        {
//            mArmLengths[id++] = reader.readElementText().toDouble();
//        }
//        else
//        { reader.skipCurrentElement(); }
//    }

//    return 0;
//}

//int robotH2Z::download( VRobot *pSetup )
//{
//    Q_ASSERT( NULL != pSetup );

//    return 0;
//}





