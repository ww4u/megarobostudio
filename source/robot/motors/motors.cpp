
#include "motors.h"

static quint8 _megaimage[]=
{
    #include "./robot.cpp"
};

static quint8 _detail[]=
{
    #include "./detail.cpp"
};

robotMotors::robotMotors()
{
    mClass = "MRX-MOTORS";
    mId = robot_motors;

    //! default gpid
    mCanGroupId = group_id_from + (mId - robot_complex)*group_segment;

//    setAxes( 1 );
//    setAxesDefName( 1 );
//    setJointName( 1 );

    mJointName.clear();
    mJointName<<QObject::tr("Motor");

    mImage = QImage::fromData( _megaimage, sizeof(_megaimage) );

    mDetail = QString::fromLocal8Bit( (char*)_detail, sizeof_array(_detail) );

    //! joint name
    mJointName.clear();
    mJointName<<QObject::tr("Motor");

//    mJointCcwMask.clear();
//    mJointCcwMask<<true;

//    mJointZeroCcw.clear();
//    mJointZeroCcw<<true;

//    //! debug used
//    //! alter the axes name
//    Q_ASSERT( mAxesConnectionName.size() == 1 );
//    mAxesConnectionName[0] = "CH1@device1"; //! ax

//    mZeroTime = 5.0;
//    mZeroDistance = 5.0;

//    mGapTime = 1.0;
//    mGapDistance = 2.0;

//    mbZeroPosEn = true;
//    mbGapPosEn = true;
//    mPosIndex = 0;

//    m_pRoboTask = new MotorTask();
//    Q_ASSERT( NULL != m_pRoboTask );

    //! debug for models
//    mModel.addItem( "1", "hello" );
//    mModel.addItem( "2", "hello2" );
//    mModel.addItem( "3", "hello3" );
}

int robotMotors::serialIn( QXmlStreamReader &reader )
{
    while(reader.readNextStartElement())
    {
        if ( reader.name() == "raw" )
        { serialInRaw(reader); }
        else if ( reader.name() == "joints" )
        { serialInJoints(reader); }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int robotMotors::serialOut( QXmlStreamWriter &writer )
{
    writer.writeStartElement("raw");
    serialOutRaw(writer);
    writer.writeEndElement();

    writer.writeStartElement("joints");
    serialOutJoints(writer);
    writer.writeEndElement();

    return 0;
}

QAbstractTableModel *robotMotors::getModel()
{
    return &mModel;
}

int robotMotors::serialOutJoints( QXmlStreamWriter &writer)
{
    for ( int i = 0; i < mModel.mItems.size(); i++ )
    {
        writer.writeStartElement("joint");
        serialOutSubJoint( writer, mModel.mItems.at(i) );
        writer.writeEndElement();
    }

    return 0;
}
int robotMotors::serialInJoints( QXmlStreamReader &reader )
{
    Relation *pRel;
    while(reader.readNextStartElement())
    {
        if ( reader.name() == "joint" )
        {
            pRel = new Relation();
            serialInSubJoint( reader, pRel );
            mModel.mItems.append( pRel );
        }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int robotMotors::serialOutSubJoint( QXmlStreamWriter &writer, Relation *pRel )
{
    Q_ASSERT( NULL != pRel );
    writer.writeTextElement( "name", pRel->name() );
    writer.writeTextElement( "value", pRel->value() );

    return 0;
}
int robotMotors::serialInSubJoint( QXmlStreamReader &reader, Relation *pRel )
{
    Q_ASSERT( NULL != pRel );
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "name" )
        { pRel->setName( reader.readElementText()); }
        else if ( reader.name() == "value" )
        { pRel->setValue( reader.readElementText()); }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}
