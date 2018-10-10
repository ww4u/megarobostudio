#include "MRQ_model.h"
namespace MegaDevice
{

int MRQ_model::saveLink( QXmlStreamWriter &writer )
{
    writer.writeTextElement( "mLINK_INTFC", toString(mLINK_INTFC) );

    return 0;
}
int MRQ_model::loadLink( QXmlStreamReader &reader )
{
    while( reader.readNextStartElement() )
    {
        if(reader.name() == "mLINK_INTFC")
        { toValue( reader.readElementText(), &mLINK_INTFC ); }
        else
        { reader.skipCurrentElement(); }
    }
    return 0;
}

int MRQ_model::saveSystem( QXmlStreamWriter &writer )
{
    writer.writeTextElement( "mSYSTEM_WORKMODE", toString(mSYSTEM_WORKMODE) );

    writer.writeTextElement( "mSYSTEM_POWERON", toString(mSYSTEM_POWERON) );

    writer.writeTextElement( "desc", mDesc );
    writer.writeTextElement( "sn", mSN );
    writer.writeTextElement( "sw", mSwVer );
    writer.writeTextElement( "hw", mHwVer );

    writer.writeTextElement( "fw", mFwVer );
    writer.writeTextElement( "bw", mBtVer );

    return 0;
}
int MRQ_model::loadSystem( QXmlStreamReader &reader )
{
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "mSYSTEM_WORKMODE" )
        { toValue( reader.readElementText(), &mSYSTEM_WORKMODE ); }

        else if ( reader.name() == "mSYSTEM_POWERON" )
        { toValue( reader.readElementText(), &mSYSTEM_POWERON ); }

        else if ( reader.name() == "desc" )
        { mDesc =  reader.readElementText(); }

        else if ( reader.name() == "sn" )
        { mSN =  reader.readElementText(); }

        else if ( reader.name() == "sw" )
        { mSwVer =  reader.readElementText(); }

        else if ( reader.name() == "hw" )
        { mHwVer =  reader.readElementText(); }

        else if ( reader.name() == "fw" )
        { mFwVer =  reader.readElementText(); }

        else if ( reader.name() == "bw" )
        { mBtVer =  reader.readElementText(); }

        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int MRQ_model::saveRs232( QXmlStreamWriter &writer )
{
    writer.writeTextElement( "mRS232_BAUD", toString(mRS232_BAUD) );
    writer.writeTextElement( "mRS232_WORDLEN", toString(mRS232_WORDLEN) );
    writer.writeTextElement( "mRS232_FLOWCTL", toString(mRS232_FLOWCTL) );
    writer.writeTextElement( "mRS232_PARITY", toString(mRS232_PARITY) );

    writer.writeTextElement( "mRS232_STOPBIT", toString(mRS232_STOPBIT) );

    return 0;
}
int MRQ_model::loadRs232( QXmlStreamReader &reader )
{
    while (reader.readNextStartElement() )
    {
        if ( reader.name() == "mRS232_BAUD" )
        { toValue( reader.readElementText(), &mRS232_BAUD ); }
        else if ( reader.name() == "mRS232_WORDLEN" )
        { toValue( reader.readElementText(), &mRS232_WORDLEN ); }
        else if ( reader.name() == "mRS232_FLOWCTL" )
        { toValue( reader.readElementText(), &mRS232_FLOWCTL ); }
        else if ( reader.name() == "mRS232_PARITY" )
        { toValue( reader.readElementText(), &mRS232_PARITY ); }
        else if ( reader.name() == "mRS232_STOPBIT" )
        { toValue( reader.readElementText(), &mRS232_STOPBIT ); }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int MRQ_model::saveCan( QXmlStreamWriter &writer )
{
    writer.writeTextElement( "mCAN_TYPE", toString(mCAN_TYPE) );
    writer.writeTextElement( "mCAN_BAUD", toString(mCAN_BAUD) );
    writer.writeTextElement( "mCAN_GROUP", DeviceModel::toString(mCAN_GROUP) );
    writer.writeTextElement( "mCAN_SENDID", DeviceModel::toString(mCAN_SENDID) );

    writer.writeTextElement( "mCAN_RECEIVEID", DeviceModel::toString(mCAN_RECEIVEID) );
    writer.writeTextElement( "mCAN_GROUPID1", DeviceModel::toString(mCAN_GROUPID1) );
    writer.writeTextElement( "mCAN_GROUPID2", DeviceModel::toString(mCAN_GROUPID2) );
    writer.writeTextElement( "mCAN_BROADCASTID", DeviceModel::toString(mCAN_BROADCASTID) );

    writer.writeTextElement( "mCAN_NETMANAGESTATE", toString(mCAN_NETMANAGESTATE) );

    writer.writeTextElement( "mCAN_NETMANAGEID_0", DeviceModel::toString(mCAN_NETMANAGEID[0]) );
    writer.writeTextElement( "mCAN_NETMANAGEID_1", DeviceModel::toString(mCAN_NETMANAGEID[1]) );
    writer.writeTextElement( "mCAN_NETMANAGEID_2", DeviceModel::toString(mCAN_NETMANAGEID[2]) );

    writer.writeTextElement( "mCAN_NETMANAGEHASH", DeviceModel::toString(mCAN_NETMANAGEHASH) );

    return 0;
}
int MRQ_model::loadCan( QXmlStreamReader &reader )
{
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "mCAN_TYPE" )
        { toValue( reader.readElementText(), &mCAN_TYPE ); }
        else if ( reader.name() == "mCAN_BAUD" )
        { toValue( reader.readElementText(), &mCAN_BAUD ); }

        else if ( reader.name() == "mCAN_NETMANAGESTATE" )
        { toValue( reader.readElementText(), &mCAN_NETMANAGESTATE ); }

        else if ( reader.name() == "mCAN_NETMANAGEID_0" )
        { DeviceModel::toValue( reader.readElementText(), &mCAN_NETMANAGEID[0] ); }
        else if ( reader.name() == "mCAN_NETMANAGEID_1" )
        { DeviceModel::toValue( reader.readElementText(), &mCAN_NETMANAGEID[1] ); }
        else if ( reader.name() == "mCAN_NETMANAGEID_2" )
        { DeviceModel::toValue( reader.readElementText(), &mCAN_NETMANAGEID[2] ); }

        else if ( reader.name() == "mCAN_NETMANAGEHASH" )
        { DeviceModel::toValue( reader.readElementText(), &mCAN_NETMANAGEHASH ); }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int MRQ_model::saveClock( QXmlStreamWriter &writer )
{
    writer.writeTextElement( "mCLOCK_FREQUENCY", DeviceModel::toString(mCLOCK_FREQUENCY) );

    writer.writeTextElement( "mCLOCK_SYNCREGISTER", toString(mCLOCK_SYNCREGISTER) );
    writer.writeTextElement( "mCLOCK_STARTTYPE", toString(mCLOCK_STARTTYPE) );
    writer.writeTextElement( "mCLOCK_SYNCSTATE", toString(mCLOCK_SYNCSTATE) );
    writer.writeTextElement( "mCLOCK_COUNT", DeviceModel::toString(mCLOCK_COUNT) );

    return 0;
}
int MRQ_model::loadClock( QXmlStreamReader &reader )
{
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "mCLOCK_FREQUENCY" )
        { DeviceModel::toValue( reader.readElementText(), &mCLOCK_FREQUENCY ); }

        else if ( reader.name() == "mCLOCK_SYNCREGISTER" )
        { toValue( reader.readElementText(), &mCLOCK_SYNCREGISTER ); }
        else if ( reader.name() == "mCLOCK_STARTTYPE" )
        { toValue( reader.readElementText(), &mCLOCK_STARTTYPE ); }
        else if ( reader.name() == "mCLOCK_SYNCSTATE" )
        { toValue( reader.readElementText(), &mCLOCK_SYNCSTATE ); }
        else if ( reader.name() == "mCLOCK_COUNT" )
        { DeviceModel::toValue( reader.readElementText(), &mCLOCK_COUNT ); }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int MRQ_model::saveMotion( QXmlStreamWriter &writer )
{
    for ( int i = 0; i < axes(); i++ )
    {
        writer.writeStartElement("axes");
        saveSubMotion( i, writer );
        writer.writeEndElement();
    }

    return 0;
}
int MRQ_model::loadMotion( QXmlStreamReader &reader )
{
    int ax = 0;
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "axes" )
        {
            loadSubMotion( ax++, reader );
        }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int MRQ_model::saveSubMotion( int id, QXmlStreamWriter &writer )
{
    writer.writeTextElement( "mMOTION_STATEREPORT", toString( mMOTION_STATEREPORT[id]) );
    writer.writeTextElement( "mMOTION_STARTSOURCE", toString( mMOTION_STARTSOURCE[id]) );
    writer.writeTextElement( "mMOTION_STARTTYPE", toString( mMOTION_STARTTYPE[id]) );

    writer.writeTextElement( "mMOTION_MAXSPEED", DeviceModel::toString( mMOTION_MAXSPEED[id]) );
    writer.writeTextElement( "mMOTION_MINSPEED", DeviceModel::toString( mMOTION_MINSPEED[id]) );
    writer.writeTextElement( "mMOTION_MAXPOSITION", DeviceModel::toString( mMOTION_MAXPOSITION[id]) );
    writer.writeTextElement( "mMOTION_MINPOSITION", DeviceModel::toString( mMOTION_MINPOSITION[id]) );

    writer.writeTextElement( "mMOTION_MAXTORQUE", DeviceModel::toString( mMOTION_MAXTORQUE[id]) );
    writer.writeTextElement( "mMOTION_MINTORQUE", DeviceModel::toString( mMOTION_MINTORQUE[id]) );
    writer.writeTextElement( "mMOTION_MAXACCELERATION", DeviceModel::toString( mMOTION_MAXACCELERATION[id]) );
    writer.writeTextElement( "mMOTION_MINACCELERATION", DeviceModel::toString( mMOTION_MINACCELERATION[id]) );

    writer.writeTextElement( "mMOTION_ORIGIN", DeviceModel::toString( mMOTION_ORIGIN[id]) );
    writer.writeTextElement( "mMOTION_OFFSETSTATE", DeviceModel::toString( mMOTION_OFFSETSTATE[id]) );

    return 0;
}

int MRQ_model::loadSubMotion( int id, QXmlStreamReader &reader )
{
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "mMOTION_STATEREPORT" )
        { toValue( reader.readElementText(), &mMOTION_STATEREPORT[id] ); }
        else if ( reader.name() == "mMOTION_STARTSOURCE" )
        { toValue( reader.readElementText(), &mMOTION_STARTSOURCE[id] ); }
        else if ( reader.name() == "mMOTION_STARTTYPE" )
        { toValue( reader.readElementText(), &mMOTION_STARTTYPE[id] ); }

        else if ( reader.name() == "mMOTION_MAXSPEED" )
        { DeviceModel::toValue( reader.readElementText(), &mMOTION_MAXSPEED[id] ); }
        else if ( reader.name() == "mMOTION_MINSPEED" )
        { DeviceModel::toValue( reader.readElementText(), &mMOTION_MINSPEED[id] ); }
        else if ( reader.name() == "mMOTION_MAXPOSITION" )
        { DeviceModel::toValue( reader.readElementText(), &mMOTION_MAXPOSITION[id] ); }
        else if ( reader.name() == "mMOTION_MINPOSITION" )
        { DeviceModel::toValue( reader.readElementText(), &mMOTION_MINPOSITION[id] ); }

        else if ( reader.name() == "mMOTION_MAXTORQUE" )
        { DeviceModel::toValue( reader.readElementText(), &mMOTION_MAXTORQUE[id] ); }
        else if ( reader.name() == "mMOTION_MINTORQUE" )
        { DeviceModel::toValue( reader.readElementText(), &mMOTION_MINTORQUE[id] ); }
        else if ( reader.name() == "mMOTION_MAXACCELERATION" )
        { DeviceModel::toValue( reader.readElementText(), &mMOTION_MAXACCELERATION[id] ); }
        else if ( reader.name() == "mMOTION_MINACCELERATION" )
        { DeviceModel::toValue( reader.readElementText(), &mMOTION_MINACCELERATION[id] ); }

        else if ( reader.name() == "mMOTION_ORIGIN" )
        { DeviceModel::toValue( reader.readElementText(), &mMOTION_ORIGIN[id] ); }
        else if ( reader.name() == "mMOTION_OFFSETSTATE" )
        { toValue( reader.readElementText(), &mMOTION_OFFSETSTATE[id] ); }

        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int MRQ_model::saveIdentity( QXmlStreamWriter &writer )
{
    for ( int i = 0; i < axes(); i++ )
    {
        writer.writeStartElement("axes");
        saveSubIdentity( i, writer );
        writer.writeEndElement();
    }

    writer.writeTextElement( "mIDENTITY_DISTDEVICE", toString( mIDENTITY_DISTDEVICE ) );

    return 0;
}
int MRQ_model::loadIdentity( QXmlStreamReader &reader )
{
    int ax = 0;
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "axes" )
        {
            loadSubIdentity( ax++, reader );
        }
        else if ( reader.name() == "mIDENTITY_DISTDEVICE" )
        { toValue( reader.readElementText(), &mIDENTITY_DISTDEVICE ); }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int MRQ_model::saveSubIdentity( int id, QXmlStreamWriter &writer )
{
    writer.writeTextElement( "mIDENTITY_GROUP1", DeviceModel::toString( mIDENTITY_GROUP[id][0]) );
    writer.writeTextElement( "mIDENTITY_GROUP2", DeviceModel::toString( mIDENTITY_GROUP[id][1]) );

    return 0;
}
int MRQ_model::loadSubIdentity( int sub, QXmlStreamReader &reader )
{
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "mIDENTITY_GROUP1" )
        { DeviceModel::toValue( reader.readElementText(), &mIDENTITY_GROUP[sub][0] ); }
        else if ( reader.name() == "mIDENTITY_GROUP2" )
        { DeviceModel::toValue( reader.readElementText(), &mIDENTITY_GROUP[sub][1] ); }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int MRQ_model::saveMotor( QXmlStreamWriter &writer )
{
    for ( int i = 0; i < axes(); i++ )
    {
        writer.writeStartElement("axes");

        saveSubMotor( i, writer );

        writer.writeEndElement();
    }

    return 0;
}
int MRQ_model::loadMotor( QXmlStreamReader &reader )
{
    int ax = 0;
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "axes" )
        {
            loadSubMotor( ax++, reader );
        }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int MRQ_model::saveSubMotor( int id, QXmlStreamWriter &writer )
{
    writer.writeTextElement( "mMOTOR_STEPANGLE", toString( mMOTOR_STEPANGLE[id]) );
    writer.writeTextElement( "mMOTOR_TYPE", toString( mMOTOR_TYPE[id]) );
    writer.writeTextElement( "mMOTOR_POSITIONUNIT", toString( mMOTOR_POSITIONUNIT[id]) );

    writer.writeTextElement( "mMOTOR_GEARRATIONUM", DeviceModel::toString( mMOTOR_GEARRATIONUM[id]) );
    writer.writeTextElement( "mMOTOR_GEARRATIODEN", DeviceModel::toString( mMOTOR_GEARRATIODEN[id]) );
    writer.writeTextElement( "mMOTOR_LEAD", DeviceModel::toString( mMOTOR_LEAD[id]) );
    writer.writeTextElement( "mMOTOR_PEAKSPEED", DeviceModel::toString( mMOTOR_PEAKSPEED[id]) );

    writer.writeTextElement( "mMOTOR_PEAKACCELERATION", DeviceModel::toString( mMOTOR_PEAKACCELERATION[id]) );
    writer.writeTextElement( "mMOTOR_SIZE", toString( mMOTOR_SIZE[id]) );
    writer.writeTextElement( "mMOTOR_VOLTAGE", DeviceModel::toString( mMOTOR_VOLTAGE[id]) );
    writer.writeTextElement( "mMOTOR_CURRENT", DeviceModel::toString( mMOTOR_CURRENT[id]) );

    writer.writeTextElement( "mMOTOR_BACKLASH", DeviceModel::toString( mMOTOR_BACKLASH[id]) );

    writer.writeTextElement( "mAcc", QString::number( mAccList.at(id) ) );
    writer.writeTextElement( "mDec", QString::number( mDecList.at(id) ) );

    return 0;
}
int MRQ_model::loadSubMotor( int id, QXmlStreamReader &reader )
{
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "mMOTOR_STEPANGLE" )
        { toValue( reader.readElementText(), &mMOTOR_STEPANGLE[id] ); }
        else if ( reader.name() == "mMOTOR_TYPE" )
        { toValue( reader.readElementText(), &mMOTOR_TYPE[id] ); }
        else if ( reader.name() == "mMOTOR_POSITIONUNIT" )
        { toValue( reader.readElementText(), &mMOTOR_POSITIONUNIT[id] ); }

        else if ( reader.name() == "mMOTOR_GEARRATIONUM" )
        { DeviceModel::toValue( reader.readElementText(), &mMOTOR_GEARRATIONUM[id] ); }
        else if ( reader.name() == "mMOTOR_GEARRATIODEN" )
        { DeviceModel::toValue( reader.readElementText(), &mMOTOR_GEARRATIODEN[id] ); }
        else if ( reader.name() == "mMOTOR_LEAD" )
        { DeviceModel::toValue( reader.readElementText(), &mMOTOR_LEAD[id] ); }
        else if ( reader.name() == "mMOTOR_PEAKSPEED" )
        { DeviceModel::toValue( reader.readElementText(), &mMOTOR_PEAKSPEED[id] ); }

        else if ( reader.name() == "mMOTOR_PEAKACCELERATION" )
        { DeviceModel::toValue( reader.readElementText(), &mMOTOR_PEAKACCELERATION[id] ); }
        else if ( reader.name() == "mMOTOR_SIZE" )
        { toValue( reader.readElementText(), &mMOTOR_SIZE[id] ); }
        else if ( reader.name() == "mMOTOR_VOLTAGE" )
        { DeviceModel::toValue( reader.readElementText(), &mMOTOR_VOLTAGE[id] ); }
        else if ( reader.name() == "mMOTOR_CURRENT" )
        { DeviceModel::toValue( reader.readElementText(), &mMOTOR_CURRENT[id] ); }

        else if ( reader.name() == "mMOTOR_BACKLASH" )
        { DeviceModel::toValue( reader.readElementText(), &mMOTOR_BACKLASH[id] ); }

        else if ( reader.name() == "mAcc" )
        { mAccList[id] = reader.readElementText().toInt(); }

        else if ( reader.name() == "mDec" )
        { mDecList[id] = reader.readElementText().toInt(); }

        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int MRQ_model::saveEncoder( QXmlStreamWriter &writer )
{
    for ( int i = 0; i < axes(); i++ )
    {
        writer.writeStartElement("axes");
        saveSubEncoder( i, writer );
        writer.writeEndElement();
    }

    return 0;
}
int MRQ_model::loadEncoder( QXmlStreamReader &reader )
{
    int ax = 0;
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "axes" )
        {
            loadSubEncoder( ax++, reader );
        }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int MRQ_model::saveSubEncoder( int id, QXmlStreamWriter &writer )
{
    writer.writeTextElement( "mENCODER_LINENUM", DeviceModel::toString( mENCODER_LINENUM[id]) );
    writer.writeTextElement( "mENCODER_CHANNELNUM", toString( mENCODER_CHANNELNUM[id]) );
    writer.writeTextElement( "mENCODER_TYPE", toString( mENCODER_TYPE[id]) );

    writer.writeTextElement( "mENCODER_MULTIPLE", toString( mENCODER_MULTIPLE[id]) );
    writer.writeTextElement( "mENCODER_STATE", toString( mENCODER_STATE[id]) );
    writer.writeTextElement( "mENCODER_FEEDBACKRATIO", QString::number( mENCODER_FEEDBACKRATIO[id]) );

    return 0;
}
int MRQ_model::loadSubEncoder( int id, QXmlStreamReader &reader )
{
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "mENCODER_LINENUM" )
        { DeviceModel::toValue( reader.readElementText(), &mENCODER_LINENUM[id] ); }
        else if ( reader.name() == "mENCODER_CHANNELNUM" )
        { toValue( reader.readElementText(), &mENCODER_CHANNELNUM[id] ); }
        else if ( reader.name() == "mENCODER_TYPE" )
        { toValue( reader.readElementText(), &mENCODER_TYPE[id] ); }

        else if ( reader.name() == "mENCODER_MULTIPLE" )
        { toValue( reader.readElementText(), &mENCODER_MULTIPLE[id] ); }
        else if ( reader.name() == "mENCODER_STATE" )
        { toValue( reader.readElementText(), &mENCODER_STATE[id] ); }
        else if ( reader.name() == "mENCODER_FEEDBACKRATIO" )
        { DeviceModel::toValue( reader.readElementText(), &mENCODER_FEEDBACKRATIO[id] ); }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int MRQ_model::saveMotionPlan( QXmlStreamWriter &writer )
{
    for ( int i = 0; i < axes(); i++ )
    {
        writer.writeStartElement("axes");

        saveSubMotionPlan( i, writer );

        writer.writeEndElement();
    }

    return 0;
}
int MRQ_model::loadMotionPlan( QXmlStreamReader &reader )
{
    int ax = 0;
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "axes" )
        { loadSubMotionPlan( ax++, reader); }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int MRQ_model::saveSubMotionPlan( int ax, QXmlStreamWriter &writer )
{
    for ( int i = 0; i < regions(); i++ )
    {
        writer.writeStartElement("page");

        saveSubMotionPlanPage( ax, i, writer );

        writer.writeEndElement();
    }

    return 0;
}
int MRQ_model::loadSubMotionPlan( int ax, QXmlStreamReader &reader )
{
    int page = 0 ;
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "page" )
        { loadSubMotionPlanPage( ax, page++, reader ); }
        else
        { reader.skipCurrentElement(); }
    }
    return 0;
}

int MRQ_model::saveSubMotionPlanPage( int id, int page,
                                      QXmlStreamWriter &writer )
{
    writer.writeTextElement( "mMOTIONPLAN_PRESETSTATE", toString( mMOTIONPLAN_PRESETSTATE[id][page] ) );
    writer.writeTextElement( "mMOTIONPLAN_EXECUTEMODE", toString( mMOTIONPLAN_EXECUTEMODE[id][page] ) );
    writer.writeTextElement( "mMOTIONPLAN_PLANMODE", toString( mMOTIONPLAN_PLANMODE[id][page] ) );
    writer.writeTextElement( "mMOTIONPLAN_MOTIONMODE", toString( mMOTIONPLAN_MOTIONMODE[id][page] ) );

    writer.writeTextElement( "mMOTIONPLAN_MODIFYDUTY", toString( mMOTIONPLAN_MODIFYDUTY[id][page] ) );
    writer.writeTextElement( "mMOTIONPLAN_CYCLENUM", DeviceModel::toString( mMOTIONPLAN_CYCLENUM[id][page] ) );
    writer.writeTextElement( "mMOTIONPLAN_WARNPOINT", DeviceModel::toString( mMOTIONPLAN_WARNPOINT[id][page] ) );

    writer.writeTextElement( "mMOTIONPLAN_ENDSTATE", toString( mMOTIONPLAN_ENDSTATE[id][page] ) );

    writer.writeTextElement( "mMOTIONPLAN_STOPMODE", toString( mMOTIONPLAN_STOPMODE[id][page] ) );
    writer.writeTextElement( "mMOTIONPLAN_STOPDISTANCE", DeviceModel::toString( mMOTIONPLAN_STOPDISTANCE[id][page] ) );
    writer.writeTextElement( "mMOTIONPLAN_STOPTIME", DeviceModel::toString( mMOTIONPLAN_STOPTIME[id][page] ) );
    writer.writeTextElement( "mMOTIONPLAN_OOSLINESTATE", toString( mMOTIONPLAN_OOSLINESTATE[id][page] ) );

    writer.writeTextElement( "mMOTIONPLAN_OOSLINEOUTNUM", DeviceModel::toString( mMOTIONPLAN_OOSLINEOUTNUM[id][page] ) );
    writer.writeTextElement( "mMOTIONPLAN_OOSLINERESPONSE", toString( mMOTIONPLAN_OOSLINERESPONSE[id][page] ) );
    writer.writeTextElement( "mMOTIONPLAN_OOSTOTALSTATE", toString( mMOTIONPLAN_OOSTOTALSTATE[id][page] ) );
    writer.writeTextElement( "mMOTIONPLAN_OOSTOTALOUTNUM", DeviceModel::toString( mMOTIONPLAN_OOSTOTALOUTNUM[id][page] ) );

    writer.writeTextElement( "mMOTIONPLAN_OOSTOTALRESPONSE", toString( mMOTIONPLAN_OOSTOTALRESPONSE[id][page] ) );

    return 0;
}
int MRQ_model::loadSubMotionPlanPage( int id, int page,
                                      QXmlStreamReader &reader )
{
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "mMOTIONPLAN_PRESETSTATE" )
        { toValue( reader.readElementText(), &mMOTIONPLAN_PRESETSTATE[id][page] ); }
        else if ( reader.name() == "mMOTIONPLAN_EXECUTEMODE" )
        { toValue( reader.readElementText(), &mMOTIONPLAN_EXECUTEMODE[id][page] ); }
        else if ( reader.name() == "mMOTIONPLAN_PLANMODE" )
        { toValue( reader.readElementText(), &mMOTIONPLAN_PLANMODE[id][page] ); }
        else if ( reader.name() == "mMOTIONPLAN_MOTIONMODE" )
        { toValue( reader.readElementText(), &mMOTIONPLAN_MOTIONMODE[id][page] ); }

        else if ( reader.name() == "mMOTIONPLAN_MODIFYDUTY" )
        { toValue( reader.readElementText(), &mMOTIONPLAN_MODIFYDUTY[id][page] ); }
        else if ( reader.name() == "mMOTIONPLAN_CYCLENUM" )
        { DeviceModel::toValue( reader.readElementText(), &mMOTIONPLAN_CYCLENUM[id][page] ); }
        else if ( reader.name() == "mMOTIONPLAN_WARNPOINT" )
        { DeviceModel::toValue( reader.readElementText(), &mMOTIONPLAN_WARNPOINT[id][page] ); }

        else if ( reader.name() == "mMOTIONPLAN_ENDSTATE" )
        { toValue( reader.readElementText(), &mMOTIONPLAN_ENDSTATE[id][page] ); }

        else if ( reader.name() == "mMOTIONPLAN_STOPMODE" )
        { toValue( reader.readElementText(), &mMOTIONPLAN_STOPMODE[id][page] ); }
        else if ( reader.name() == "mMOTIONPLAN_STOPDISTANCE" )
        { DeviceModel::toValue( reader.readElementText(), &mMOTIONPLAN_STOPDISTANCE[id][page] ); }
        else if ( reader.name() == "mMOTIONPLAN_STOPTIME" )
        { DeviceModel::toValue( reader.readElementText(), &mMOTIONPLAN_STOPTIME[id][page] ); }
        else if ( reader.name() == "mMOTIONPLAN_OOSLINESTATE" )
        { toValue( reader.readElementText(), &mMOTIONPLAN_OOSLINESTATE[id][page] ); }

        else if ( reader.name() == "mMOTIONPLAN_OOSLINEOUTNUM" )
        { DeviceModel::toValue( reader.readElementText(), &mMOTIONPLAN_OOSLINEOUTNUM[id][page] ); }
        else if ( reader.name() == "mMOTIONPLAN_OOSLINERESPONSE" )
        { toValue( reader.readElementText(), &mMOTIONPLAN_OOSLINERESPONSE[id][page] ); }
        else if ( reader.name() == "mMOTIONPLAN_OOSTOTALSTATE" )
        { toValue( reader.readElementText(), &mMOTIONPLAN_OOSTOTALSTATE[id][page] ); }
        else if ( reader.name() == "mMOTIONPLAN_OOSTOTALOUTNUM" )
        { DeviceModel::toValue( reader.readElementText(), &mMOTIONPLAN_OOSTOTALOUTNUM[id][page] ); }

        else if ( reader.name() == "mMOTIONPLAN_OOSTOTALRESPONSE" )
        { toValue( reader.readElementText(), &mMOTIONPLAN_OOSTOTALRESPONSE[id][page] ); }

        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int MRQ_model::saveReport( QXmlStreamWriter &writer )
{
    for ( int i = 0; i < axes(); i++ )
    {
        writer.writeStartElement("axes");
        saveSubReport( i, writer );
        writer.writeEndElement();
    }
    return 0;
}
int MRQ_model::loadReport( QXmlStreamReader &reader )
{
    int ax = 0;
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "axes" )
        { loadSubReport( ax++, reader); }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int MRQ_model::saveSubReport( int ax, QXmlStreamWriter &writer )
{
    for ( int j = 0; j < 15; j++ )
    {
        writer.writeStartElement("item");
        writer.writeTextElement( "mREPORT_STATE", toString( mREPORT_STATE[ax][j] ) );
        writer.writeTextElement( "mREPORT_PERIOD", DeviceModel::toString( mREPORT_PERIOD[ax][j] ) );
        writer.writeEndElement();
    }

    return 0;
}
int MRQ_model::loadSubReport( int ax, QXmlStreamReader &reader )
{
    int id = 0;
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "item")
        {
            while( reader.readNextStartElement() )
            {
                if (reader.name() == "mREPORT_STATE" )
                { toValue( reader.readElementText(), &mREPORT_STATE[ax][id]); }
                else if ( reader.name() == "mREPORT_PERIOD" )
                { DeviceModel::toValue( reader.readElementText(), &mREPORT_PERIOD[ax][id]); }
                else
                { reader.skipCurrentElement(); }
            }

            id++;
        }
        else
        { reader.skipCurrentElement(); }

    }

    return 0;
}

int MRQ_model::saveTrigger( QXmlStreamWriter &writer )
{
    for ( int i = 0; i < axes(); i++ )
    {
        writer.writeStartElement("axes");
        saveSubTrigger( i, writer );
        writer.writeEndElement();
    }

    return 0;
}
int MRQ_model::loadTrigger( QXmlStreamReader &reader )
{
    int ax = 0;
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "axes" )
        { loadSubTrigger( ax++, reader ); }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int MRQ_model::saveSubTrigger( int sub, QXmlStreamWriter &writer )
{
    writer.writeTextElement( "mTRIGGER_MODE", toString(mTRIGGER_MODE[sub]) );

    writer.writeTextElement( "mTRIGGER_PATTSTATE", toString(mTRIGGER_PATTSTATE[sub]) );
    writer.writeTextElement( "mTRIGGER_PATTERN", toString(mTRIGGER_PATTERN[sub]) );
    writer.writeTextElement( "mTRIGGER_PATTERN1", toString(mTRIGGER_PATTERN1[sub]) );
    writer.writeTextElement( "mTRIGGER_PATTRESP", toString(mTRIGGER_PATTRESP[sub]) );

    writer.writeTextElement( "mTRIGGER_PATTSMODE", toString(mTRIGGER_PATTSMODE[sub]) );
    writer.writeTextElement( "mTRIGGER_PATTSPERIOD", DeviceModel::toString(mTRIGGER_PATTSPERIOD[sub]) );

    int trigId;
    for ( int j = 0; j < mTrigSrcs; j++ )
    {
        writer.writeStartElement("channel");

        trigId = mTrigIdsList.at( j );

        saveSubTriggerChannel( sub, trigId, writer );

        writer.writeEndElement();
    }

    return 0;
}
int MRQ_model::loadSubTrigger( int ax, QXmlStreamReader &reader )
{
    int ch=0;
    while( reader.readNextStartElement() )
    {
        if (reader.name() == "mTRIGGER_MODE" )
        { toValue( reader.readElementText(), &mTRIGGER_MODE[ax]); }

        else if ( reader.name() == "mTRIGGER_PATTSTATE" )
        { toValue( reader.readElementText(), &mTRIGGER_PATTSTATE[ax]); }
        else if ( reader.name() == "mTRIGGER_PATTERN" )
        { toValue( reader.readElementText(), &mTRIGGER_PATTERN[ax]); }
        else if ( reader.name() == "mTRIGGER_PATTERN1" )
        { toValue( reader.readElementText(), &mTRIGGER_PATTERN1[ax]); }
        else if ( reader.name() == "mTRIGGER_PATTRESP" )
        { toValue( reader.readElementText(), &mTRIGGER_PATTRESP[ax]); }

        else if ( reader.name() == "mTRIGGER_PATTSMODE" )
        { toValue( reader.readElementText(), &mTRIGGER_PATTSMODE[ax]); }
        else if ( reader.name() == "mTRIGGER_PATTSPERIOD" )
        { DeviceModel::toValue( reader.readElementText(), &mTRIGGER_PATTSPERIOD[ax]); }
        else if ( reader.name() == "channel" )
        { loadSubTriggerChannel( ax, mTrigIdsList.at( ch ), reader ); ch++; }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int MRQ_model::saveSubTriggerChannel( int sub, int ch, QXmlStreamWriter &writer )
{
    writer.writeTextElement( "mTRIGGER_LEVELSTATE", toString(mTRIGGER_LEVELSTATE[sub][ch]) );
    writer.writeTextElement( "mTRIGGER_LEVELTYPE", toString(mTRIGGER_LEVELTYPE[sub][ch]) );

    writer.writeTextElement( "mTRIGGER_LEVELRESP", toString(mTRIGGER_LEVELRESP[sub][ch]) );
    writer.writeTextElement( "mTRIGGER_LEVELSMODE", DeviceModel::toString(mTRIGGER_LEVELSMODE[sub][ch]) );
    writer.writeTextElement( "mTRIGGER_LEVELSPERIOD", DeviceModel::toString(mTRIGGER_LEVELSPERIOD[sub][ch]) );

    writer.writeTextElement( "mTRIGGER_LEVELRUNWAVE", toString(mTRIGGER_LEVELRUNWAVE[sub][ch]) );

    return 0;
}
int MRQ_model::loadSubTriggerChannel( int ax, int ch, QXmlStreamReader &reader )
{
    while( reader.readNextStartElement() )
    {
        if (reader.name() == "mTRIGGER_LEVELSTATE" )
        { toValue( reader.readElementText(), &mTRIGGER_LEVELSTATE[ax][ch]); }
        else if ( reader.name() == "mTRIGGER_LEVELTYPE" )
        { toValue( reader.readElementText(), &mTRIGGER_LEVELTYPE[ax][ch]); }

        else if ( reader.name() == "mTRIGGER_LEVELRESP" )
        { toValue( reader.readElementText(), &mTRIGGER_LEVELRESP[ax][ch]); }
        else if ( reader.name() == "mTRIGGER_LEVELSMODE" )
        { toValue( reader.readElementText(), &mTRIGGER_LEVELSMODE[ax][ch]); }
        else if ( reader.name() == "mTRIGGER_LEVELSPERIOD" )
        { DeviceModel::toValue( reader.readElementText(), &mTRIGGER_LEVELSPERIOD[ax][ch]); }
        else if ( reader.name() == "mTRIGGER_LEVELRUNWAVE" )
        { toValue( reader.readElementText(), &mTRIGGER_LEVELRUNWAVE[ax][ch]); }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int MRQ_model::saveDriver( QXmlStreamWriter &writer )
{
    for ( int i = 0; i < axes(); i++ )
    {
        writer.writeStartElement("axes");

        if ( mDriverId == VRobot::motor_driver_262 )
        { saveSubDriver( i, writer ); }
        else if ( mDriverId == VRobot::motor_driver_820 )
        { saveSubDriver820( i, writer); }
        else
        { Q_ASSERT(false); }

        writer.writeEndElement();
    }

    return 0;
}
int MRQ_model::loadDriver( QXmlStreamReader &reader )
{
    int ax = 0;
    while( reader.readNextStartElement() )
    {
        if (reader.name() == "axes" )
        {
            if ( mDriverId == VRobot::motor_driver_262 )
            { loadSubDriver( ax++, reader ); }
            else if ( mDriverId == VRobot::motor_driver_820 )
            { loadSubDriver820( ax++, reader ); }
            else
            { Q_ASSERT(false); }
        }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int MRQ_model::saveSubDriver( int sub, QXmlStreamWriter &writer )
{
    //! driver
    writer.writeTextElement( "mDRIVER_TYPE", toString(mDRIVER_TYPE[sub]) );

    writer.writeTextElement( "mDRIVER_CURRENT", DeviceModel::toString(mDRIVER_CURRENT[sub]) );
    writer.writeTextElement( "mDRIVER_MICROSTEPS", toString(mDRIVER_MICROSTEPS[sub]) );
    writer.writeTextElement( "mDRIVER_STATE", toString(mDRIVER_STATE[sub]) );

    writer.writeTextElement( "mDRIVER_IDLECURRENT", DeviceModel::toString(mDRIVER_IDLECURRENT[sub]) );

    writer.writeTextElement( "mDRIVER_SWITCHTIME", DeviceModel::toString(mDRIVER_SWITCHTIME[sub]) );
    writer.writeTextElement( "mDRIVER_MINICURRRATIO", toString(mDRIVER_MINICURRRATIO[sub]) );

    return 0;
}
int MRQ_model::loadSubDriver( int sub, QXmlStreamReader &reader )
{
    while( reader.readNextStartElement() )
    {
        if (reader.name() == "mDRIVER_TYPE" )
        { toValue( reader.readElementText(), &mDRIVER_TYPE[sub]); }

        else if ( reader.name() == "mDRIVER_CURRENT" )
        { DeviceModel::toValue( reader.readElementText(), &mDRIVER_CURRENT[sub]); }
        else if ( reader.name() == "mDRIVER_MICROSTEPS" )
        { toValue( reader.readElementText(), &mDRIVER_MICROSTEPS[sub]); }
        else if ( reader.name() == "mDRIVER_STATE" )
        { toValue( reader.readElementText(), &mDRIVER_STATE[sub]); }

        else if ( reader.name() == "mDRIVER_IDLECURRENT" )
        { DeviceModel::toValue( reader.readElementText(), &mDRIVER_IDLECURRENT[sub]); }

        else if ( reader.name() == "mDRIVER_SWITCHTIME" )
        { DeviceModel::toValue( reader.readElementText(), &mDRIVER_SWITCHTIME[sub]); }
        else if ( reader.name() == "mDRIVER_MINICURRRATIO" )
        { toValue( reader.readElementText(), &mDRIVER_MINICURRRATIO[sub]); }

        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int MRQ_model::saveSubDriver820( int sub, QXmlStreamWriter &writer )
{
    //! driver
    writer.writeTextElement( "mNEWDRIVER_TYPE", toString(mNEWDRIVER_TYPE[sub]) );

    writer.writeTextElement( "mNEWDRIVER_CURRENT", DeviceModel::toString(mNEWDRIVER_CURRENT) );
    writer.writeTextElement( "mNEWDRIVER_MICROSTEPS", toString(mNEWDRIVER_MICROSTEPS) );
    writer.writeTextElement( "mNEWDRIVER_STATE", toString(mNEWDRIVER_STATE[sub]) );

    return 0;
}
int MRQ_model::loadSubDriver820( int sub, QXmlStreamReader &reader )
{
    while( reader.readNextStartElement() )
    {
        if (reader.name() == "mNEWDRIVER_TYPE" )
        { toValue( reader.readElementText(), &mNEWDRIVER_TYPE[sub]); }

        else if ( reader.name() == "mNEWDRIVER_CURRENT" )
        { DeviceModel::toValue( reader.readElementText(), &mNEWDRIVER_CURRENT); }
        else if ( reader.name() == "mNEWDRIVER_MICROSTEPS" )
        { toValue( reader.readElementText(), &mNEWDRIVER_MICROSTEPS); }
        else if ( reader.name() == "mNEWDRIVER_STATE" )
        { toValue( reader.readElementText(), &mNEWDRIVER_STATE[sub]); }

        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int MRQ_model::saveDo( QXmlStreamWriter &writer )
{
    for ( int i = 0; i < dos(); i++ )
    {
        writer.writeStartElement("do");
        saveSubDo( i, writer );
        writer.writeEndElement();
    }

    return 0;
}
int MRQ_model::loadDo( QXmlStreamReader &reader )
{
    int dos = 0;
    while( reader.readNextStartElement() )
    {
        if (reader.name() == "do" )
        {
            loadSubDo( dos++, reader );
        }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int MRQ_model::saveSubDo( int sub, QXmlStreamWriter &writer )
{
    writer.writeTextElement( "mDIGITALOUTPUT_STATE", toString(mDIGITALOUTPUT_STATE[sub]) );

    writer.writeTextElement( "mDIGITALOUTPUT_SIGNAL", toString(mDIGITALOUTPUT_SIGNAL[sub]) );
    writer.writeTextElement( "mDIGITALOUTPUT_POLARITY", toString(mDIGITALOUTPUT_POLARITY[sub]) );
    writer.writeTextElement( "mDIGITALOUTPUT_SOURCE", DeviceModel::toString(mDIGITALOUTPUT_SOURCE[sub]) );
    writer.writeTextElement( "mDIGITALOUTPUT_CONDITION", toString(mDIGITALOUTPUT_CONDITION[sub]) );

    writer.writeTextElement( "mDIGITALOUTPUT_PERIOD", DeviceModel::toString(mDIGITALOUTPUT_PERIOD[sub]) );
    writer.writeTextElement( "mDIGITALOUTPUT_DUTY", DeviceModel::toString(mDIGITALOUTPUT_DUTY[sub]) );

    return 0;
}
int MRQ_model::loadSubDo( int sub, QXmlStreamReader &reader )
{
    while( reader.readNextStartElement() )
    {
        if (reader.name() == "mDIGITALOUTPUT_STATE" )
        { toValue( reader.readElementText(), &mDIGITALOUTPUT_STATE[sub]); }

        else if ( reader.name() == "mDIGITALOUTPUT_SIGNAL" )
        { toValue( reader.readElementText(), &mDIGITALOUTPUT_SIGNAL[sub]); }
        else if ( reader.name() == "mDIGITALOUTPUT_POLARITY" )
        { toValue( reader.readElementText(), &mDIGITALOUTPUT_POLARITY[sub]); }
        else if ( reader.name() == "mDIGITALOUTPUT_SOURCE" )
        { DeviceModel::toValue( reader.readElementText(), &mDIGITALOUTPUT_SOURCE[sub]); }
        else if ( reader.name() == "mDIGITALOUTPUT_CONDITION" )
        { toValue( reader.readElementText(), &mDIGITALOUTPUT_CONDITION[sub]); }

        else if ( reader.name() == "mDIGITALOUTPUT_PERIOD" )
        { DeviceModel::toValue( reader.readElementText(), &mDIGITALOUTPUT_PERIOD[sub]); }
        else if ( reader.name() == "mDIGITALOUTPUT_DUTY" )
        { DeviceModel::toValue( reader.readElementText(), &mDIGITALOUTPUT_DUTY[sub]); }

        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int MRQ_model::saveISOo( QXmlStreamWriter &writer )
{
    for ( int i = 0; i < isos();i++ )
    {
        writer.writeStartElement("iso");
        saveSubISOo( i, writer );
        writer.writeEndElement();
    }

    return 0;
}
int MRQ_model::loadISOo( QXmlStreamReader &reader )
{
    int id=0;
    while( reader.readNextStartElement() )
    {
        if (reader.name() == "iso" )
        {
            loadSubISOo( id++, reader );
        }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int MRQ_model::saveSubISOo( int sub, QXmlStreamWriter &writer )
{
    writer.writeTextElement( "mISOLATOROUTPUT_STATE", toString( mISOLATOROUTPUT_STATE[sub]) );
    writer.writeTextElement( "mISOLATOROUTPUT_SOURCE", DeviceModel::toString( mISOLATOROUTPUT_SOURCE[sub]) );
    writer.writeTextElement( "mISOLATOROUTPUT_CONDITION", toString( mISOLATOROUTPUT_CONDITION[sub]) );
    writer.writeTextElement( "mISOLATOROUTPUT_RESPONSE", toString( mISOLATOROUTPUT_RESPONSE[sub]) );

    return 0;
}
int MRQ_model::loadSubISOo( int sub, QXmlStreamReader &reader )
{
    while( reader.readNextStartElement() )
    {
        if (reader.name() == "mISOLATOROUTPUT_STATE" )
        { toValue( reader.readElementText(), &mISOLATOROUTPUT_STATE[sub]); }
        else if ( reader.name() == "mISOLATOROUTPUT_SOURCE" )
        { DeviceModel::toValue( reader.readElementText(), &mISOLATOROUTPUT_SOURCE[sub]); }
        else if ( reader.name() == "mISOLATOROUTPUT_CONDITION" )
        { toValue( reader.readElementText(), &mISOLATOROUTPUT_CONDITION[sub]); }
        else if ( reader.name() == "mISOLATOROUTPUT_RESPONSE" )
        { toValue( reader.readElementText(), &mISOLATOROUTPUT_RESPONSE[sub]); }

        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int MRQ_model::saveSensorUART( QXmlStreamWriter &writer )
{
    for ( int i = 0; i < uarts(); i++ )
    {
        writer.writeStartElement("uart");

        saveSubUART( i, writer );

        writer.writeEndElement();
    }
    return 0;
}
int MRQ_model::loadSensorUART( QXmlStreamReader &reader )
{
    int uart;
    uart = 0;
    while( reader.readNextStartElement() )
    {
        if (reader.name() == "uart" )
        { loadSubUART( uart++, reader ); }

        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int MRQ_model::saveSubUART( int sub, QXmlStreamWriter &writer )
{
    writer.writeTextElement( "mSENSORUART_BAUD", toString( mSENSORUART_BAUD[sub]) );
    writer.writeTextElement( "mSENSORUART_WORDLEN", toString( mSENSORUART_WORDLEN[sub]) );
    writer.writeTextElement( "mSENSORUART_FLOWCTL", toString( mSENSORUART_FLOWCTL[sub]) );
    writer.writeTextElement( "mSENSORUART_PARITY", toString( mSENSORUART_PARITY[sub]) );

    writer.writeTextElement( "mSENSORUART_STOPBIT", toString( mSENSORUART_STOPBIT[sub]) );

    for ( int j = 0;j < 4; j++ )
    {
        writer.writeStartElement("sensor");
        saveSubUARTSensor( sub, j, writer );
        writer.writeEndElement();
    }

    return 0;
}
int MRQ_model::loadSubUART( int sub, QXmlStreamReader &reader )
{
    int sens = 0;
    while( reader.readNextStartElement() )
    {
        if (reader.name() == "mSENSORUART_BAUD" )
        { toValue( reader.readElementText(), &mSENSORUART_BAUD[sub]); }
        else if ( reader.name() == "mSENSORUART_WORDLEN" )
        { toValue( reader.readElementText(), &mSENSORUART_WORDLEN[sub]); }
        else if ( reader.name() == "mSENSORUART_FLOWCTL" )
        { toValue( reader.readElementText(), &mSENSORUART_FLOWCTL[sub]); }
        else if ( reader.name() == "mSENSORUART_PARITY" )
        { toValue( reader.readElementText(), &mSENSORUART_PARITY[sub]); }

        else if ( reader.name() == "mSENSORUART_STOPBIT" )
        { toValue( reader.readElementText(), &mSENSORUART_STOPBIT[sub]); }

        else if ( reader.name() == "sensor" )
        {
            loadSubUARTSensor( sub, sens++, reader );
        }

        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int MRQ_model::saveSubUARTSensor( int sub, int subsens, QXmlStreamWriter &writer )
{
    writer.writeTextElement( "mSENSORUART_STATE", toString( mSENSORUART_STATE[sub][subsens]) );

    writer.writeTextElement( "mSENSORUART_SOF", DeviceModel::toString( mSENSORUART_SOF[sub][subsens]) );
    writer.writeTextElement( "mSENSORUART_FRAMELEN", DeviceModel::toString( mSENSORUART_FRAMELEN[sub][subsens]) );
    writer.writeTextElement( "mSENSORUART_RECEIVENUM", DeviceModel::toString( mSENSORUART_RECEIVENUM[sub][subsens]) );
    writer.writeTextElement( "mSENSORUART_SWITCHTIME", DeviceModel::toString( mSENSORUART_SWITCHTIME[sub][subsens]) );

    return 0;
}
int MRQ_model::loadSubUARTSensor( int sub, int subSens, QXmlStreamReader &reader )
{
    while( reader.readNextStartElement() )
    {
        if (reader.name() == "mSENSORUART_STATE" )
        { toValue( reader.readElementText(), &mSENSORUART_STATE[sub][subSens]); }

        else if ( reader.name() == "mSENSORUART_SOF" )
        { DeviceModel::toValue( reader.readElementText(), &mSENSORUART_SOF[sub][subSens]); }
        else if ( reader.name() == "mSENSORUART_FRAMELEN" )
        { DeviceModel::toValue( reader.readElementText(), &mSENSORUART_FRAMELEN[sub][subSens]); }
        else if ( reader.name() == "mSENSORUART_RECEIVENUM" )
        { DeviceModel::toValue( reader.readElementText(), &mSENSORUART_RECEIVENUM[sub][subSens]); }
        else if ( reader.name() == "mSENSORUART_SWITCHTIME" )
        { DeviceModel::toValue( reader.readElementText(), &mSENSORUART_SWITCHTIME[sub][subSens]); }

        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int MRQ_model::saveTunning( QXmlStreamWriter &writer )
{
    for ( int i = 0; i < axes(); i++ )
    {
        writer.writeStartElement("axes");

        saveSubTunning( i, writer );

        writer.writeEndElement();
    }

    return 0;
}
int MRQ_model::loadTunning( QXmlStreamReader &reader )
{
    int axes =0;
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "axes" )
        {
            loadSubTunning( axes, reader );
            axes++;
        }
        else
        {
            reader.skipCurrentElement();
        }
    }

    return 0;
}

int MRQ_model::saveSubTunning( int ax, QXmlStreamWriter &writer )
{
    writer.writeTextElement( "state", toString( mTUNING_STATE[ax] ) );
    writer.writeTextElement( "mini_ratio", toString( mTUNING_MINICURRRATIO[ax] ) );

    writer.writeTextElement( "energy", DeviceModel::toString( mTUNING_ENERGYEFFIC[ax] ) );
    writer.writeTextElement( "energy1", DeviceModel::toString( mTUNING_ENERGYEFFIC1[ax] ) );

    writer.writeTextElement( "regulate", toString( mTUNING_CURRREGULATE[ax] ) );
    writer.writeTextElement( "regulate1", toString( mTUNING_CURRREGULATE1[ax] ) );

    return 0;
}
int MRQ_model::loadSubTunning( int ax, QXmlStreamReader &reader )
{
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "state" )
        { toValue( reader.readElementText(), mTUNING_STATE + ax ); }
        else if ( reader.name() == "mini_ratio" )
        { toValue( reader.readElementText(), mTUNING_MINICURRRATIO + ax ); }
        else if ( reader.name() == "energy" )
        { DeviceModel::toValue( reader.readElementText(), mTUNING_ENERGYEFFIC + ax ); }
        else if ( reader.name() == "energy1" )
        { DeviceModel::toValue( reader.readElementText(), mTUNING_ENERGYEFFIC1 + ax ); }
        else if ( reader.name() == "regulate")
        { toValue( reader.readElementText(), mTUNING_CURRREGULATE + ax ); }
        else if ( reader.name() == "regulate1" )
        { toValue( reader.readElementText(), mTUNING_CURRREGULATE1 + ax ); }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int MRQ_model::saveISOi( QXmlStreamWriter &writer )
{
    writer.writeTextElement("mISOLATORIN_STATE", toString(mISOLATORIN_STATE));
    writer.writeTextElement("mISOLATORIN_TYPE", toString(mISOLATORIN_TYPE));
    writer.writeTextElement("mISOLATORIN_RESPONSE", toString(mISOLATORIN_RESPONSE));

    //! \todo respchan

    writer.writeTextElement("mISOLATORIN_SMODE", toString(mISOLATORIN_SMODE));
    writer.writeTextElement("mISOLATORIN_SPERIOD", DeviceModel::toString(mISOLATORIN_SPERIOD));

    return 0;
}

int MRQ_model::loadISOi( QXmlStreamReader &reader )
{
    while( reader.readNextStartElement() )
    {
        if (reader.name() == "mISOLATORIN_STATE" )
        { toValue( reader.readElementText(), &mISOLATORIN_STATE); }
        else if ( reader.name() == "mISOLATORIN_TYPE" )
        { toValue( reader.readElementText(), &mISOLATORIN_TYPE); }
        else if ( reader.name() == "mISOLATORIN_RESPONSE" )
        { toValue( reader.readElementText(), &mISOLATORIN_RESPONSE); }

        else if ( reader.name() == "mISOLATORIN_SMODE" )
        { toValue( reader.readElementText(), &mISOLATORIN_SMODE); }
        else if ( reader.name() == "mISOLATORIN_SPERIOD" )
        { DeviceModel::toValue( reader.readElementText(), &mISOLATORIN_SPERIOD); }

        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int MRQ_model::saveAi( QXmlStreamWriter &writer )
{
    writer.writeTextElement("mANALOGIN_STATE", toString(mANALOGIN_STATE));

    writer.writeTextElement("mANALOGIN_RESPONSEH", toString(mANALOGIN_RESPONSEH));
    writer.writeTextElement("mANALOGIN_RESPONSEL", toString(mANALOGIN_RESPONSEL));

    writer.writeTextElement("mANALOGIN_THRESHOLDH", DeviceModel::toString(mANALOGIN_THRESHOLDH));
    writer.writeTextElement("mANALOGIN_THRESHOLDL", DeviceModel::toString(mANALOGIN_THRESHOLDL));

    return 0;

}
int MRQ_model::loadAi( QXmlStreamReader &reader )
{
    while( reader.readNextStartElement() )
    {
        if (reader.name() == "mANALOGIN_STATE" )
        { toValue( reader.readElementText(), &mISOLATORIN_STATE); }

        else if ( reader.name() == "mANALOGIN_RESPONSEH" )
        { toValue( reader.readElementText(), &mANALOGIN_RESPONSEH); }
        else if ( reader.name() == "mANALOGIN_RESPONSEL" )
        { toValue( reader.readElementText(), &mANALOGIN_RESPONSEL); }

        else if ( reader.name() == "mANALOGIN_THRESHOLDH" )
        { DeviceModel::toValue( reader.readElementText(), &mANALOGIN_THRESHOLDH); }
        else if ( reader.name() == "mANALOGIN_THRESHOLDL" )
        { DeviceModel::toValue( reader.readElementText(), &mANALOGIN_THRESHOLDL); }

        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int MRQ_model::saveAngleAlarm( QXmlStreamWriter &writer )
{
    for ( int i = 0; i < 4; i++ )
    {
        writer.writeStartElement("sensor");
        saveSubAngleAlarm( i, writer );
        writer.writeEndElement();
    }
    return 0;
}
int MRQ_model::loadAngleAlarm( QXmlStreamReader &reader )
{
    int id = 0;

    while( reader.readNextStartElement() )
    {
        if (reader.name() == "sensor" )
        { loadSubAngleAlarm(id++,reader); }

        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int MRQ_model::saveSubAngleAlarm( int sub, QXmlStreamWriter &writer )
{
    writer.writeTextElement("mABSENCALARM_STATE", toString(mABSENCALARM_STATE[sub]));
    writer.writeTextElement("mABSENCALARM_UPLIMIT", DeviceModel::toString(mABSENCALARM_UPLIMIT[sub]));
    writer.writeTextElement("mABSENCALARM_DOWNLIMIT", DeviceModel::toString(mABSENCALARM_DOWNLIMIT[sub]));
    writer.writeTextElement("mABSENCALARM_ZEROVALUE", DeviceModel::toString(mABSENCALARM_ZEROVALUE[sub]));

    writer.writeTextElement("mABSENCALARM_ZEROPOSITION", toString(mABSENCALARM_ZEROPOSITION[sub]));
    writer.writeTextElement("mABSENCALARM_RESPONSE", toString(mABSENCALARM_RESPONSE));

    return 0;

}
int MRQ_model::loadSubAngleAlarm( int sub, QXmlStreamReader &reader )
{
    while( reader.readNextStartElement() )
    {
        if (reader.name() == "mABSENCALARM_STATE" )
        { toValue( reader.readElementText(), &mABSENCALARM_STATE[sub]); }
        else if (reader.name() == "mABSENCALARM_UPLIMIT" )
        { DeviceModel::toValue( reader.readElementText(), &mABSENCALARM_UPLIMIT[sub]); }
        else if (reader.name() == "mABSENCALARM_DOWNLIMIT" )
        { DeviceModel::toValue( reader.readElementText(), &mABSENCALARM_DOWNLIMIT[sub]); }

        else if (reader.name() == "mABSENCALARM_ZEROPOSITION" )
        { toValue( reader.readElementText(), &mABSENCALARM_ZEROPOSITION[sub]); }
        else if (reader.name() == "mABSENCALARM_RESPONSE" )
        { toValue( reader.readElementText(), &mABSENCALARM_RESPONSE); }

        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int MRQ_model::saveDistanceAlarm( QXmlStreamWriter &writer )
{
    for ( int i = 0; i < 4; i++ )
    {
        writer.writeStartElement("sensor");
        saveSubDistanceAlarm( i, writer );
        writer.writeEndElement();
    }
    return 0;
}
int MRQ_model::loadDistanceAlarm( QXmlStreamReader &reader )
{
    int id = 0;

    while( reader.readNextStartElement() )
    {
        if (reader.name() == "sensor" )
        {  loadSubDistanceAlarm( id++, reader); }

        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int MRQ_model::saveSubDistanceAlarm( int sub, QXmlStreamWriter &writer )
{
    writer.writeTextElement("mDISTANCEALARM_STATE", toString(mDISTANCEALARM_STATE[sub]));
    writer.writeTextElement("mDISTANCEALARM_ALARM1DIST", DeviceModel::toString(mDISTANCEALARM_ALARM1DIST[sub]));
    writer.writeTextElement("mDISTANCEALARM_ALARM2DIST", DeviceModel::toString(mDISTANCEALARM_ALARM2DIST[sub]));
    writer.writeTextElement("mDISTANCEALARM_ALARM3DIST", DeviceModel::toString(mDISTANCEALARM_ALARM3DIST[sub]));
    return 0;
}
int MRQ_model::loadSubDistanceAlarm( int sub, QXmlStreamReader &reader )
{
    while( reader.readNextStartElement() )
    {
        if (reader.name() == "mDISTANCEALARM_STATE" )
        { toValue( reader.readElementText(), &mDISTANCEALARM_STATE[sub]); }
        else if (reader.name() == "mDISTANCEALARM_ALARM1DIST" )
        { DeviceModel::toValue( reader.readElementText(), &mDISTANCEALARM_ALARM1DIST[sub]); }
        else if (reader.name() == "mDISTANCEALARM_ALARM2DIST" )
        { DeviceModel::toValue( reader.readElementText(), &mDISTANCEALARM_ALARM2DIST[sub]); }
        else if (reader.name() == "mDISTANCEALARM_ALARM3DIST" )
        { DeviceModel::toValue( reader.readElementText(), &mDISTANCEALARM_ALARM3DIST[sub]); }

        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int MRQ_model::saveOtp( QXmlStreamWriter &writer )
{
    writer.writeTextElement("mOTP_STATE", toString(mOTP_STATE));
    writer.writeTextElement("mOTP_THRESHOLD", DeviceModel::toString(mOTP_THRESHOLD));
    writer.writeTextElement("mOTP_RESPONSE", toString(mOTP_RESPONSE));
    writer.writeTextElement("mOTP_PERIOD", DeviceModel::toString(mOTP_PERIOD));
    return 0;
}

int MRQ_model::loadOtp( QXmlStreamReader &reader )
{
    while( reader.readNextStartElement() )
    {
        if (reader.name() == "mOTP_STATE" )
        { toValue( reader.readElementText(), &mOTP_STATE); }
        else if (reader.name() == "mOTP_THRESHOLD" )
        { DeviceModel::toValue( reader.readElementText(), &mOTP_THRESHOLD); }
        else if (reader.name() == "mOTP_RESPONSE" )
        { toValue( reader.readElementText(), &mOTP_RESPONSE); }
        else if (reader.name() == "mOTP_PERIOD" )
        { DeviceModel::toValue( reader.readElementText(), &mOTP_PERIOD); }

        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}


}
