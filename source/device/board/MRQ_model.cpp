#include "MRQ_model.h"
namespace MegaDevice
{
MRQ_model::MRQ_model()
{
    mSignature = 0;

    mFanInfo.mDuty = 50;
    mFanInfo.mFreq = 1000;

    for ( int i = 0; i < 4; i++ )
    {
        mLedInfo[i].mDuty = 50;
        mLedInfo[i].mFreq = 100;
    }
}

//int MRQ_model::_serialOut( QXmlStreamWriter &writer )
//{
//    writer.writeStartElement("mrq");

//    //! link
//    writer.writeTextElement( "mLINK_INTFC", toString(mLINK_INTFC) );
//    writer.writeTextElement( "mSYSTEM_WORKMODE", toString(mSYSTEM_WORKMODE) );

//    //! system
//    writer.writeTextElement( "mSYSTEM_POWERON", toString(mSYSTEM_POWERON) );

//    writer.writeTextElement( "desc", mDesc );
//    writer.writeTextElement( "sn", mSn );
//    writer.writeTextElement( "sw", mSwVer );
//    writer.writeTextElement( "hw", mHwVer );

//    writer.writeTextElement( "fw", mFwVer );
//    writer.writeTextElement( "bw", mBtVer );

//    //! 232
//    writer.writeTextElement( "mRS232_BAUD", toString(mRS232_BAUD) );
//    writer.writeTextElement( "mRS232_WORDLEN", toString(mRS232_WORDLEN) );
//    writer.writeTextElement( "mRS232_FLOWCTL", toString(mRS232_FLOWCTL) );
//    writer.writeTextElement( "mRS232_PARITY", toString(mRS232_PARITY) );

//    writer.writeTextElement( "mRS232_STOPBIT", toString(mRS232_STOPBIT) );

//    //! can
//    writer.writeTextElement( "mCAN_TYPE", toString(mCAN_TYPE) );
//    writer.writeTextElement( "mCAN_BAUD", toString(mCAN_BAUD) );
//    writer.writeTextElement( "mCAN_GROUP", DeviceModel::toString(mCAN_GROUP) );
//    writer.writeTextElement( "mCAN_SENDID", DeviceModel::toString(mCAN_SENDID) );

//    writer.writeTextElement( "mCAN_RECEIVEID", DeviceModel::toString(mCAN_RECEIVEID) );
//    writer.writeTextElement( "mCAN_GROUPID1", DeviceModel::toString(mCAN_GROUPID1) );
//    writer.writeTextElement( "mCAN_GROUPID2", DeviceModel::toString(mCAN_GROUPID2) );
//    writer.writeTextElement( "mCAN_BROADCASTID", DeviceModel::toString(mCAN_BROADCASTID) );

//    writer.writeTextElement( "mCAN_NETMANAGESTATE", toString(mCAN_NETMANAGESTATE) );

//    writer.writeTextElement( "mCAN_NETMANAGEID_0", DeviceModel::toString(mCAN_NETMANAGEID[0]) );
//    writer.writeTextElement( "mCAN_NETMANAGEID_1", DeviceModel::toString(mCAN_NETMANAGEID[1]) );
//    writer.writeTextElement( "mCAN_NETMANAGEID_2", DeviceModel::toString(mCAN_NETMANAGEID[2]) );

//    writer.writeTextElement( "mCAN_NETMANAGEHASH", DeviceModel::toString(mCAN_NETMANAGEHASH) );
////    writer.writeTextElement( "mCAN_NETMANAGESIGNATURESIZE", DeviceModel::toString(mCAN_NETMANAGESIGNATURESIZE) );
//    writer.writeTextElement( "mCLOCK_FREQUENCY", DeviceModel::toString(mCLOCK_FREQUENCY) );

//    writer.writeTextElement( "mCLOCK_SYNCREGISTER", toString(mCLOCK_SYNCREGISTER) );
//    writer.writeTextElement( "mCLOCK_STARTTYPE", toString(mCLOCK_STARTTYPE) );
//    writer.writeTextElement( "mCLOCK_SYNCSTATE", toString(mCLOCK_SYNCSTATE) );
//    writer.writeTextElement( "mCLOCK_COUNT", DeviceModel::toString(mCLOCK_COUNT) );

//    //! motors
//    for ( int i = 0; i < axes(); i++ )
//    {
//        save_motor( i, writer );
//    }

//    writer.writeEndElement();

//    return 0;
//}

int MRQ_model::serialOut( QXmlStreamWriter &writer )
{
    writer.writeStartElement("mrq");

        writer.writeStartElement("link");
        saveLink( writer );
        writer.writeEndElement();

        writer.writeStartElement("system");
        saveSystem( writer );
        writer.writeEndElement();

        writer.writeStartElement("rs232");
        saveRs232( writer );
        writer.writeEndElement();

        writer.writeStartElement("can");
        saveCan( writer );
        writer.writeEndElement();

        writer.writeStartElement("clock");
        saveClock( writer );
        writer.writeEndElement();

        writer.writeStartElement("motion");
        saveMotion( writer );
        writer.writeEndElement();

        writer.writeStartElement("identity");
        saveIdentity( writer );
        writer.writeEndElement();

        writer.writeStartElement("motor");
        saveMotor( writer );
        writer.writeEndElement();

        writer.writeStartElement("encoder");
        saveEncoder( writer );
        writer.writeEndElement();

        writer.writeStartElement("motionplan");
        saveMotionPlan( writer );
        writer.writeEndElement();

        writer.writeStartElement("report");
        saveReport( writer );
        writer.writeEndElement();

        writer.writeStartElement("trigger");
        saveTrigger( writer );
        writer.writeEndElement();

        writer.writeStartElement("driver");
        saveDriver( writer );
        writer.writeEndElement();

        writer.writeStartElement("do");
        saveDo( writer );
        writer.writeEndElement();

        writer.writeStartElement("isoo");
        saveISOo( writer );
        writer.writeEndElement();

        writer.writeStartElement("sensoruart");
        saveSensorUART( writer );
        writer.writeEndElement();

        writer.writeStartElement("isoi");
        saveISOi( writer );
        writer.writeEndElement();

        writer.writeStartElement("ai");
        saveAi( writer );
        writer.writeEndElement();

        writer.writeStartElement("anglealarm");
        saveAngleAlarm( writer );
        writer.writeEndElement();

        writer.writeStartElement("distancealarm");
        saveDistanceAlarm( writer );
        writer.writeEndElement();

        writer.writeStartElement("otp");
        saveOtp( writer );
        writer.writeEndElement();

    writer.writeEndElement();

    return 0;
}

int MRQ_model::serialIn( QXmlStreamReader &reader )
{
    //! start element
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "mrq" )
        {
            while( reader.readNextStartElement() )
            {
                if ( reader.name() == "link" )
                { loadLink( reader ); }
                else if ( reader.name() == "system" )
                { loadSystem(reader); }
                else if ( reader.name() == "rs232" )
                { loadRs232( reader ); }
                else if ( reader.name() == "can" )
                { loadCan( reader); }
                else if ( reader.name() == "clock" )
                { loadClock( reader); }
                else if ( reader.name() == "motion" )
                { loadMotion( reader ); }
                else if ( reader.name() == "identity" )
                { loadIdentity( reader ); }
                else if ( reader.name() == "motor" )
                { loadMotor( reader); }
                else if ( reader.name() == "encoder" )
                { loadEncoder( reader); }
                else if ( reader.name() == "motionplan" )
                { loadMotionPlan( reader); }
                else if ( reader.name() == "report" )
                { loadReport( reader); }
                else if ( reader.name() == "trigger" )
                { loadTrigger( reader); }
                else if ( reader.name() == "driver" )
                { loadDriver( reader); }
                else if ( reader.name() == "do" )
                { loadDo( reader); }
                else if ( reader.name() == "isoo" )
                { loadISOo( reader); }
                else if ( reader.name() == "sensoruart" )
                { loadSensorUART( reader); }
                else if ( reader.name() == "isoi" )
                { loadISOi( reader); }
                else if ( reader.name() == "ai" )
                { loadAi( reader ); }
                else if ( reader.name() == "anglealarm" )
                { loadAngleAlarm( reader); }
                else if ( reader.name() == "distancealarm" )
                { loadDistanceAlarm( reader); }
                else if ( reader.name() == "otp" )
                { loadOtp( reader ); }
                else
                { reader.skipCurrentElement(); }
            }
        }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

void MRQ_model::setSignature( quint32 hash )
{ mSignature = hash; }
quint32 MRQ_model::getSignature()
{ return mSignature; }

QString MRQ_model::getFullDesc( int id )
{
    return QString( "%1(%2)").arg( mDesc ).arg( getName() ) ;
}

QString MRQ_model::getDesc()
{ return mDesc; }
QString MRQ_model::getSN()
{
    return mSn;
}
QString MRQ_model::getSwVer()
{ return mSwVer; }
QString MRQ_model::getHwVer()
{ return mHwVer; }
QString MRQ_model::getFwVer()
{ return mFwVer; }
QString MRQ_model::getBtVer()
{ return mBtVer; }

QString MRQ_model::getSeqVer()
{ return mSeqVer; }

}
