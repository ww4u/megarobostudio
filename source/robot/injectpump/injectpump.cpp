
#include "injectpump.h"

#include "../../device/board/_MRQ_enum.h"

static quint8 _megaimage[]=
{
    #include "./robot.cpp"
};

static quint8 _detail[]=
{
    #include "./detail.cpp"
};

robotInject::robotInject()
{
    mClass = QObject::tr("MRX-INJ2");
    mId = robot_ip;

    //! default gpid
    mCanGroupId = group_id_from + (mId - robot_complex)*group_segment;

    setAxes( 2 );
    setAxesDefName( 2 );
    setJointName( 2 );

    mDetail = QString::fromLocal8Bit( (char*)_detail, sizeof_array(_detail) );

    //! joint name
    mJointName.clear();
    mJointName<<QObject::tr("Valve")
              <<QObject::tr("Injection");

    mImage = QImage::fromData( _megaimage, sizeof(_megaimage) );

    mArmLengths.clear();

    mJointCcwMask.clear();      //! visible
    mJointCcwMask<<true<<true;

    mJointZeroCcw.clear();
    mJointZeroCcw<<true<<true;

    mAngleDir.clear();
    mAngleDir<<true<<true;                 //! x:  y:

    mSuctionDirs.clear();
    mSuctionDirs<<-1<<1;

    //! debug used
    //! alter the axes name
    mAxesConnectionName[0] = "CH1@device1"; //! x
    mAxesConnectionName[1] = "CH2@device1"; //! y

    mZeroTime = 1;
    mZeroDistance = 5;
    mZeroSpeed = 5;

    mVGapTime = 1;
    mVGapDistance = 10;        //! dist
    mIGapTime = 1;
    mIGapDistance = 5;

    m_pRoboTask = new IJTask();
    Q_ASSERT( NULL != m_pRoboTask );

}

robotInject::~robotInject()
{

}

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

int robotInject::download( VRobot *pSetup )
{
    Q_ASSERT( NULL != pSetup );

    return 0;
}





