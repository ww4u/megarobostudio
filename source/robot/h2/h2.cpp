
#include "h2.h"

#include "../../device/board/_MRQ_enum.h"

static quint8 _megaimage[]=
{
    #include "./robot1.cpp"
};

static quint8 _detail[]=
{
    #include "./detail.cpp"
};

robotH2::robotH2()
{
    mClass = QObject::tr("H2");
    mId = robot_h2;

    //! default gpid
    mCanGroupId = group_id_from + mId - robot_complex;

    setAxes( 2 );
    setAxesDefName( 2 );
    setJointName( 2 );

    mDetail = QString::fromLocal8Bit( (char*)_detail, sizeof_array(_detail) );

    //! joint name
    mJointName.clear();
    mJointName<<QObject::tr("X")
              <<QObject::tr("Y");

    mImage = QImage::fromData( _megaimage, sizeof(_megaimage) );

    //! debug used
    //! alter the axes name
    mAxesConnectionName[0] = "CH1@device1"; //! x
    mAxesConnectionName[1] = "CH1@device2"; //! y

    mZeroTime = 5;
    mZeroSpeed = 5;
    mZeroAngle = 100;
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

    return 0;
}

int robotH2::serialOutZero( QXmlStreamWriter &writer)
{
    writer.writeTextElement( "time", QString::number(mZeroTime) );
    writer.writeTextElement( "angle", QString::number(mZeroAngle) );
    writer.writeTextElement( "speed", QString::number(mZeroSpeed) );

    return 0;
}
int robotH2::serialInZero( QXmlStreamReader &reader )
{
    while(reader.readNextStartElement())
    {
        if ( reader.name() == "time" )
        { mZeroTime = reader.readElementText().toDouble(); }
        else if ( reader.name() == "angle" )
        { mZeroAngle = reader.readElementText().toDouble(); }
        else if ( reader.name() == "speed" )
        { mZeroSpeed = reader.readElementText().toDouble(); }
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





