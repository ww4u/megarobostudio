#include "MRQ_model.h"
namespace MegaDevice
{
MRQ_model::MRQ_model()
{
    mSignature = 0;
}

int MRQ_model::serialOut( QXmlStreamWriter &writer )
{
    writer.writeStartElement("mrq");

    //! link
    writer.writeTextElement( "mLINK_INTFC", toString(mLINK_INTFC) );
    writer.writeTextElement( "mSYSTEM_WORKMODE", toString(mSYSTEM_WORKMODE) );

    //! system
    writer.writeTextElement( "mSYSTEM_POWERON", toString(mSYSTEM_POWERON) );

    writer.writeTextElement( "desc", mDesc );
    writer.writeTextElement( "sn", mSn );
    writer.writeTextElement( "sw", mSwVer );
    writer.writeTextElement( "hw", mHwVer );

    writer.writeTextElement( "fw", mFwVer );
    writer.writeTextElement( "bw", mBtVer );

//    writer.writeTextElement( "mSYSTEM_REPORTSWITCH", toString(mSYSTEM_REPORTSWITCH) );

    //! 232
    writer.writeTextElement( "mRS232_BAUD", toString(mRS232_BAUD) );
    writer.writeTextElement( "mRS232_WORDLEN", toString(mRS232_WORDLEN) );
    writer.writeTextElement( "mRS232_FLOWCTL", toString(mRS232_FLOWCTL) );
    writer.writeTextElement( "mRS232_PARITY", toString(mRS232_PARITY) );

    writer.writeTextElement( "mRS232_STOPBIT", toString(mRS232_STOPBIT) );

    //! can
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
//    writer.writeTextElement( "mCAN_NETMANAGESIGNATURESIZE", DeviceModel::toString(mCAN_NETMANAGESIGNATURESIZE) );
    writer.writeTextElement( "mCLOCK_FREQUENCY", DeviceModel::toString(mCLOCK_FREQUENCY) );

    writer.writeTextElement( "mCLOCK_SYNCREGISTER", toString(mCLOCK_SYNCREGISTER) );
    writer.writeTextElement( "mCLOCK_STARTTYPE", toString(mCLOCK_STARTTYPE) );
    writer.writeTextElement( "mCLOCK_SYNCSTATE", toString(mCLOCK_SYNCSTATE) );
    writer.writeTextElement( "mCLOCK_COUNT", DeviceModel::toString(mCLOCK_COUNT) );

    //! motors
    for ( int i = 0; i < axes(); i++ )
    {
        save_motor( i, writer );
    }

    writer.writeEndElement();

    return 0;
}
int MRQ_model::serialIn( QXmlStreamReader &reader )
{
    QStringRef motorIdStr;
    int motorId;

    //! start element
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "mrq" )
        {
            while( reader.readNextStartElement() )
            {
                if ( reader.name() == "mLINK_INTFC" )
                { toValue( reader.readElementText(), &mLINK_INTFC ); }

                if ( reader.name() == "mSYSTEM_WORKMODE" )
                { toValue( reader.readElementText(), &mSYSTEM_WORKMODE ); }

                if ( reader.name() == "mSYSTEM_POWERON" )
                { toValue( reader.readElementText(), &mSYSTEM_POWERON ); }

                if ( reader.name() == "desc" )
                { mDesc =  reader.readElementText(); }

                if ( reader.name() == "sn" )
                { mSn =  reader.readElementText(); }

                if ( reader.name() == "sw" )
                { mSwVer =  reader.readElementText(); }

                if ( reader.name() == "hw" )
                { mHwVer =  reader.readElementText(); }

                if ( reader.name() == "fw" )
                { mFwVer =  reader.readElementText(); }

                if ( reader.name() == "bw" )
                { mBtVer =  reader.readElementText(); }

//                if ( reader.name() == "mSYSTEM_REPORTSWITCH" )
//                { toValue( reader.readElementText(), &mSYSTEM_REPORTSWITCH ); }

                if ( reader.name() == "mRS232_BAUD" )
                { toValue( reader.readElementText(), &mRS232_BAUD ); }
                if ( reader.name() == "mRS232_WORDLEN" )
                { toValue( reader.readElementText(), &mRS232_WORDLEN ); }
                if ( reader.name() == "mRS232_FLOWCTL" )
                { toValue( reader.readElementText(), &mRS232_FLOWCTL ); }
                if ( reader.name() == "mRS232_PARITY" )
                { toValue( reader.readElementText(), &mRS232_PARITY ); }
                if ( reader.name() == "mRS232_STOPBIT" )
                { toValue( reader.readElementText(), &mRS232_STOPBIT ); }

                //! can
                if ( reader.name() == "mCAN_TYPE" )
                { toValue( reader.readElementText(), &mCAN_TYPE ); }
                if ( reader.name() == "mCAN_BAUD" )
                { toValue( reader.readElementText(), &mCAN_BAUD ); }
                if ( reader.name() == "mCAN_GROUP" )
                { DeviceModel::toValue( reader.readElementText(), &mCAN_GROUP ); }
                if ( reader.name() == "mCAN_SENDID" )
                { DeviceModel::toValue( reader.readElementText(), &mCAN_SENDID ); }

                if ( reader.name() == "mCAN_RECEIVEID" )
                { DeviceModel::toValue( reader.readElementText(), &mCAN_RECEIVEID ); }
                if ( reader.name() == "mCAN_GROUPID1" )
                { DeviceModel::toValue( reader.readElementText(), &mCAN_GROUPID1 ); }
                if ( reader.name() == "mCAN_GROUPID2" )
                { DeviceModel::toValue( reader.readElementText(), &mCAN_GROUPID2 ); }
                if ( reader.name() == "mCAN_BROADCASTID" )
                { DeviceModel::toValue( reader.readElementText(), &mCAN_BROADCASTID ); }

                if ( reader.name() == "mCAN_NETMANAGESTATE" )
                { toValue( reader.readElementText(), &mCAN_NETMANAGESTATE ); }

                if ( reader.name() == "mCAN_NETMANAGEID_0" )
                { DeviceModel::toValue( reader.readElementText(), &mCAN_NETMANAGEID[0] ); }
                if ( reader.name() == "mCAN_NETMANAGEID_1" )
                { DeviceModel::toValue( reader.readElementText(), &mCAN_NETMANAGEID[1] ); }
                if ( reader.name() == "mCAN_NETMANAGEID_2" )
                { DeviceModel::toValue( reader.readElementText(), &mCAN_NETMANAGEID[2] ); }

                if ( reader.name() == "mCAN_NETMANAGEHASH" )
                { DeviceModel::toValue( reader.readElementText(), &mCAN_NETMANAGEHASH ); }
//                if ( reader.name() == "mCAN_NETMANAGESIGNATURESIZE" )
//                { DeviceModel::toValue( reader.readElementText(), &mCAN_NETMANAGESIGNATURESIZE ); }
                if ( reader.name() == "mCLOCK_FREQUENCY" )
                { DeviceModel::toValue( reader.readElementText(), &mCLOCK_FREQUENCY ); }

                if ( reader.name() == "mCLOCK_SYNCREGISTER" )
                { toValue( reader.readElementText(), &mCLOCK_SYNCREGISTER ); }
                if ( reader.name() == "mCLOCK_STARTTYPE" )
                { toValue( reader.readElementText(), &mCLOCK_STARTTYPE ); }
                if ( reader.name() == "mCLOCK_SYNCSTATE" )
                { toValue( reader.readElementText(), &mCLOCK_SYNCSTATE ); }
                if ( reader.name() == "mCLOCK_COUNT" )
                { DeviceModel::toValue( reader.readElementText(), &mCLOCK_COUNT ); }

                if ( reader.name() == "motor" )
                {
                    motorIdStr = ( reader.attributes().value( "id" ) );
                    motorId = motorIdStr.toInt();

                    load_motor( motorId, reader );
                }
            }
        }
    }

    return 0;
}

//QString  MRQ_model::name()
//{ return QStringLiteral(""); }

//int     MRQ_model::axes()
//{ return 1; }

int MRQ_model::save_motor( int id, QXmlStreamWriter &writer )
{
    writer.writeStartElement("motor");
    writer.writeAttribute( "id", QString::number(id) );

//    writer.writeTextElement( "mMOTION_STATE", toString( mMOTION_STATE[id]) );
    writer.writeTextElement( "mMOTION_STATEREPORT", toString( mMOTION_STATEREPORT[id]) );
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
//    writer.writeTextElement( "mMOTION_INITPOSITIONUNIT", toString( mMOTION_INITPOSITIONUNIT[id]) );

//    writer.writeTextElement( "mMOTION_INITPOSITION", DeviceModel::toString( mMOTION_INITPOSITION[id]) );
//    writer.writeTextElement( "mMOTION_INITIOSIGNAL", toString( mMOTION_INITIOSIGNAL[id]) );
//    writer.writeTextElement( "mMOTION_INCHINGMODE", toString( mMOTION_INCHINGMODE[id]) );
//    writer.writeTextElement( "mMOTION_INCHINGTIME", DeviceModel::toString( mMOTION_INCHINGTIME[id]) );

    writer.writeTextElement( "mMOTION_OFFSETSTATE", toString( mMOTION_OFFSETSTATE[id]) );
//    writer.writeTextElement( "mMOTION_PVTSTEPS", DeviceModel::toString( mMOTION_PVTSTEPS[id]) );
//    writer.writeTextElement( "mMOTION_COUNTSTEPS", DeviceModel::toString( mMOTION_COUNTSTEPS[id]) );
//    writer.writeTextElement( "mMOTION_PVTCIRCLE", DeviceModel::toString( mMOTION_PVTCIRCLE[id]) );

    writer.writeTextElement( "mMOTION_COUNTCIRCLE", DeviceModel::toString( mMOTION_COUNTCIRCLE[id]) );
    writer.writeTextElement( "mMOTION_ABCOUNT", DeviceModel::toString( mMOTION_ABCOUNT[id]) );
    writer.writeTextElement( "mMOTION_REVMOTION", toString( mMOTION_REVMOTION[id]) );
    writer.writeTextElement( "mIDENTITY_GROUP1", DeviceModel::toString( mIDENTITY_GROUP[id][0]) );
    writer.writeTextElement( "mIDENTITY_GROUP2", DeviceModel::toString( mIDENTITY_GROUP[id][1]) );

//    writer.writeTextElement( "mSTOPDECEL_MODE", toString( mSTOPDECEL_MODE[id]) );
//    writer.writeTextElement( "mSTOPDECEL_DISTANCE", DeviceModel::toString( mSTOPDECEL_DISTANCE[id]) );
//    writer.writeTextElement( "mSTOPDECEL_TIME", DeviceModel::toString( mSTOPDECEL_TIME[id]) );

//    writer.writeTextElement( "mOUTOFSTEP_LINESTATE", toString( mOUTOFSTEP_LINESTATE[id]) );
//    writer.writeTextElement( "mOUTOFSTEP_LINEOUTNUM", DeviceModel::toString( mOUTOFSTEP_LINEOUTNUM[id]) );
//    writer.writeTextElement( "mOUTOFSTEP_LINERESPONSE", toString( mOUTOFSTEP_LINERESPONSE[id]) );
//    writer.writeTextElement( "mOUTOFSTEP_TOTALSTATE", toString( mOUTOFSTEP_TOTALSTATE[id]) );

//    writer.writeTextElement( "mOUTOFSTEP_TOTALOUTNUM", DeviceModel::toString( mOUTOFSTEP_TOTALOUTNUM[id]) );
//    writer.writeTextElement( "mOUTOFSTEP_TOTALRESPONSE", toString( mOUTOFSTEP_TOTALRESPONSE[id]) );
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
    writer.writeTextElement( "mENCODER_LINENUM", DeviceModel::toString( mENCODER_LINENUM[id]) );
    writer.writeTextElement( "mENCODER_CHANNELNUM", toString( mENCODER_CHANNELNUM[id]) );

    writer.writeTextElement( "mENCODER_TYPE", toString( mENCODER_TYPE[id]) );
    writer.writeTextElement( "mENCODER_MULTIPLE", toString( mENCODER_MULTIPLE[id]) );
    writer.writeTextElement( "mENCODER_STATE", toString( mENCODER_STATE[id]) );
//    writer.writeTextElement( "mMOTIONPLAN_PVTCONFIG", toString( mMOTIONPLAN_PVTCONFIG[id]) );

//    writer.writeTextElement( "mMOTIONPLAN_POSITION", toString( mMOTIONPLAN_POSITION[id]) );
//    writer.writeTextElement( "mMOTIONPLAN_VELOCITY", toString( mMOTIONPLAN_VELOCITY[id]) );
//    writer.writeTextElement( "mMOTIONPLAN_TIME", toString( mMOTIONPLAN_TIME[id]) );
//    writer.writeTextElement( "mMOTIONPLAN_EXECUTEMODE", toString( mMOTIONPLAN_EXECUTEMODE[id]) );

//    writer.writeTextElement( "mMOTIONPLAN_PLANMODE", toString( mMOTIONPLAN_PLANMODE[id]) );
//    writer.writeTextElement( "mMOTIONPLAN_MOTIONMODE", toString( mMOTIONPLAN_MOTIONMODE[id]) );
//    writer.writeTextElement( "mMOTIONPLAN_MODIFYDUTY", toString( mMOTIONPLAN_MODIFYDUTY[id]) );
//    writer.writeTextElement( "mMOTIONPLAN_REMAINPOINT", toString( mMOTIONPLAN_REMAINPOINT[id]) );

//    writer.writeTextElement( "mMOTIONPLAN_OUTPUTPOINT", DeviceModel::toString( mMOTIONPLAN_OUTPUTPOINT[id]) );
//    writer.writeTextElement( "mMOTIONPLAN_STARTPOINT", DeviceModel::toString( mMOTIONPLAN_STARTPOINT[id]) );
//    writer.writeTextElement( "mMOTIONPLAN_ENDPOINT", DeviceModel::toString( mMOTIONPLAN_ENDPOINT[id]) );
//    writer.writeTextElement( "mMOTIONPLAN_CYCLENUM", toString( mMOTIONPLAN_CYCLENUM[id]) );

//    writer.writeTextElement( "mMOTIONPLAN_WARNPOINT", toString( mMOTIONPLAN_WARNPOINT[id]) );
//    writer.writeTextElement( "mMOTIONPLAN_ENDSTATE", toString( mMOTIONPLAN_ENDSTATE[id]) );
//    writer.writeTextElement( "mMOTIONPLAN_FEEDBACKRATIO", DeviceModel::toString( mMOTIONPLAN_FEEDBACKRATIO[id]) );
//    writer.writeTextElement( "mREPORT_STATE", toString( mREPORT_STATE[id]) );

//    writer.writeTextElement( "mREPORT_PERIOD", toString( mREPORT_STATE[id]) );
//    writer.writeTextElement( "mREPORT_DATA", toString( mREPORT_STATE[id]) );
    writer.writeTextElement( "mTRIGGER_MODE", toString( mTRIGGER_MODE[id]) );
    writer.writeTextElement( "mTRIGGER_PATTSTATE", toString( mTRIGGER_PATTSTATE[id]) );

    writer.writeTextElement( "mTRIGGER_PATTERN", toString( mTRIGGER_PATTERN[id]) );
    writer.writeTextElement( "mTRIGGER_PATTERN1", toString( mTRIGGER_PATTERN1[id]) );
//    writer.writeTextElement( "mTRIGGER_PATTERN2", toString( mTRIGGER_PATTERN2[id]) );
//    writer.writeTextElement( "mTRIGGER_PATTERN3", toString( mTRIGGER_PATTERN3[id]) );

//    writer.writeTextElement( "mTRIGGER_PATTERN4", toString( mTRIGGER_PATTERN4[id]) );
    writer.writeTextElement( "mTRIGGER_PATTRESP", toString( mTRIGGER_PATTRESP[id]) );
    writer.writeTextElement( "mTRIGGER_PATTSMODE", toString( mTRIGGER_PATTSMODE[id]) );
    writer.writeTextElement( "mTRIGGER_PATTSPERIOD", DeviceModel::toString( mTRIGGER_PATTSPERIOD[id]) );

    //! level

    writer.writeTextElement( "mDRIVER_TYPE", toString( mDRIVER_TYPE[id]) );
    writer.writeTextElement( "mTRIGGER_PATTRESP", toString( mTRIGGER_PATTRESP[id]) );
    writer.writeTextElement( "mTRIGGER_PATTSMODE", toString( mTRIGGER_PATTSMODE[id]) );
    writer.writeTextElement( "mDRIVER_CURRENT", DeviceModel::toString( mDRIVER_CURRENT[id]) );

    writer.writeTextElement( "mDRIVER_MICROSTEPS", toString( mDRIVER_MICROSTEPS[id]) );
//    writer.writeTextElement( "mDRIVER_SWITCH", toString( mDRIVER_SWITCH[id]) );
    writer.writeTextElement( "mTRIGGER_PATTSMODE", toString( mTRIGGER_PATTSMODE[id]) );
    writer.writeTextElement( "mDRIVER_CURRENT", DeviceModel::toString( mDRIVER_CURRENT[id]) );

    writer.writeEndElement();

    return 0;
}

int MRQ_model::load_motor( int id, QXmlStreamReader &reader )
{
    while( reader.readNextStartElement() )
    {
//        if ( reader.name() == "mMOTION_STATE" )
//        { toValue( reader.readElementText(), &mMOTION_STATE[id] ); }
        if ( reader.name() == "mMOTION_STATEREPORT" )
        { toValue( reader.readElementText(), &mMOTION_STATEREPORT[id] ); }
        if ( reader.name() == "mMOTION_STARTTYPE" )
        { toValue( reader.readElementText(), &mMOTION_STARTTYPE[id] ); }

        if ( reader.name() == "mMOTION_MAXSPEED" )
        { DeviceModel::toValue( reader.readElementText(), &mMOTION_MAXSPEED[id] ); }
        if ( reader.name() == "mMOTION_MINSPEED" )
        { DeviceModel::toValue( reader.readElementText(), &mMOTION_MINSPEED[id] ); }

        if ( reader.name() == "mMOTION_MAXPOSITION" )
        { DeviceModel::toValue( reader.readElementText(), &mMOTION_MAXPOSITION[id] ); }
        if ( reader.name() == "mMOTION_MINPOSITION" )
        { DeviceModel::toValue( reader.readElementText(), &mMOTION_MINPOSITION[id] ); }
        if ( reader.name() == "mMOTION_MAXTORQUE" )
        { DeviceModel::toValue( reader.readElementText(), &mMOTION_MAXTORQUE[id] ); }
        if ( reader.name() == "mMOTION_MINTORQUE" )
        { DeviceModel::toValue( reader.readElementText(), &mMOTION_MINTORQUE[id] ); }

        if ( reader.name() == "mMOTION_MAXACCELERATION" )
        { DeviceModel::toValue( reader.readElementText(), &mMOTION_MAXACCELERATION[id] ); }
        if ( reader.name() == "mMOTION_MINACCELERATION" )
        { DeviceModel::toValue( reader.readElementText(), &mMOTION_MINACCELERATION[id] ); }
        if ( reader.name() == "mMOTION_ORIGIN" )
        { DeviceModel::toValue( reader.readElementText(), &mMOTION_ORIGIN[id] ); }
//        if ( reader.name() == "mMOTION_INITPOSITIONUNIT" )
//        { toValue( reader.readElementText(), &mMOTION_INITPOSITIONUNIT[id] ); }

//        if ( reader.name() == "mMOTION_INITPOSITION" )
//        { DeviceModel::toValue( reader.readElementText(), &mMOTION_INITPOSITION[id] ); }
//        if ( reader.name() == "mMOTION_INITIOSIGNAL" )
//        { toValue( reader.readElementText(), &mMOTION_INITIOSIGNAL[id] ); }
//        if ( reader.name() == "mMOTION_INCHINGMODE" )
//        { toValue( reader.readElementText(), &mMOTION_INCHINGMODE[id] ); }
//        if ( reader.name() == "mMOTION_INCHINGTIME" )
//        { DeviceModel::toValue( reader.readElementText(), &mMOTION_INCHINGTIME[id] ); }

        if ( reader.name() == "mMOTION_OFFSETSTATE" )
        { toValue( reader.readElementText(), &mMOTION_OFFSETSTATE[id] ); }
//        if ( reader.name() == "mMOTION_PVTSTEPS" )
//        { DeviceModel::toValue( reader.readElementText(), &mMOTION_PVTSTEPS[id] ); }
//        if ( reader.name() == "mMOTION_COUNTSTEPS" )
//        { DeviceModel::toValue( reader.readElementText(), &mMOTION_COUNTSTEPS[id] ); }
//        if ( reader.name() == "mMOTION_PVTCIRCLE" )
//        { DeviceModel::toValue( reader.readElementText(), &mMOTION_PVTCIRCLE[id] ); }

        if ( reader.name() == "mMOTION_COUNTCIRCLE" )
        { DeviceModel::toValue( reader.readElementText(), &mMOTION_COUNTCIRCLE[id] ); }
        if ( reader.name() == "mMOTION_ABCOUNT" )
        { DeviceModel::toValue( reader.readElementText(), &mMOTION_ABCOUNT[id] ); }
        if ( reader.name() == "mMOTION_REVMOTION" )
        { toValue( reader.readElementText(), &mMOTION_REVMOTION[id] ); }
        if ( reader.name() == "mIDENTITY_GROUP1" )
        { DeviceModel::toValue( reader.readElementText(), &mIDENTITY_GROUP[id][0] ); }
        if ( reader.name() == "mIDENTITY_GROUP2" )
        { DeviceModel::toValue( reader.readElementText(), &mIDENTITY_GROUP[id][1] ); }

//        if ( reader.name() == "mSTOPDECEL_MODE" )
//        { toValue( reader.readElementText(), &mSTOPDECEL_MODE[id] ); }
//        if ( reader.name() == "mSTOPDECEL_DISTANCE" )
//        { DeviceModel::toValue( reader.readElementText(), &mSTOPDECEL_DISTANCE[id] ); }
//        if ( reader.name() == "mSTOPDECEL_TIME" )
//        { DeviceModel::toValue( reader.readElementText(), &mSTOPDECEL_TIME[id] ); }

//        if ( reader.name() == "mOUTOFSTEP_LINESTATE" )
//        { toValue( reader.readElementText(), &mOUTOFSTEP_LINESTATE[id] ); }
//        if ( reader.name() == "mOUTOFSTEP_LINEOUTNUM" )
//        { DeviceModel::toValue( reader.readElementText(), &mOUTOFSTEP_LINEOUTNUM[id] ); }
//        if ( reader.name() == "mOUTOFSTEP_LINERESPONSE" )
//        { toValue( reader.readElementText(), &mOUTOFSTEP_LINERESPONSE[id] ); }
//        if ( reader.name() == "mOUTOFSTEP_TOTALSTATE" )
//        { toValue( reader.readElementText(), &mOUTOFSTEP_TOTALSTATE[id] ); }

//        if ( reader.name() == "mOUTOFSTEP_TOTALOUTNUM" )
//        { DeviceModel::toValue( reader.readElementText(), &mOUTOFSTEP_TOTALOUTNUM[id] ); }
//        if ( reader.name() == "mOUTOFSTEP_TOTALRESPONSE" )
//        { toValue( reader.readElementText(), &mOUTOFSTEP_TOTALRESPONSE[id] ); }
        if ( reader.name() == "mMOTOR_STEPANGLE" )
        { toValue( reader.readElementText(), &mMOTOR_STEPANGLE[id] ); }
        if ( reader.name() == "mMOTOR_TYPE" )
        { toValue( reader.readElementText(), &mMOTOR_TYPE[id] ); }

        if ( reader.name() == "mMOTOR_POSITIONUNIT" )
        { toValue( reader.readElementText(), &mMOTOR_POSITIONUNIT[id] ); }
        if ( reader.name() == "mMOTOR_GEARRATIONUM" )
        { DeviceModel::toValue( reader.readElementText(), &mMOTOR_GEARRATIONUM[id] ); }
        if ( reader.name() == "mMOTOR_GEARRATIODEN" )
        { DeviceModel::toValue( reader.readElementText(), &mMOTOR_GEARRATIODEN[id] ); }
        if ( reader.name() == "mMOTOR_LEAD" )
        { DeviceModel::toValue( reader.readElementText(), &mMOTOR_LEAD[id] ); }

        if ( reader.name() == "mMOTOR_PEAKSPEED" )
        { DeviceModel::toValue( reader.readElementText(), &mMOTOR_PEAKSPEED[id] ); }
        if ( reader.name() == "mMOTOR_PEAKACCELERATION" )
        { DeviceModel::toValue( reader.readElementText(), &mMOTOR_PEAKACCELERATION[id] ); }
        if ( reader.name() == "mMOTOR_SIZE" )
        { toValue( reader.readElementText(), &mMOTOR_SIZE[id] ); }
        if ( reader.name() == "mMOTOR_VOLTAGE" )
        { DeviceModel::toValue( reader.readElementText(), &mMOTOR_VOLTAGE[id] ); }

        if ( reader.name() == "mMOTOR_CURRENT" )
        { DeviceModel::toValue( reader.readElementText(), &mMOTOR_CURRENT[id] ); }
        if ( reader.name() == "mMOTOR_BACKLASH" )
        { DeviceModel::toValue( reader.readElementText(), &mMOTOR_BACKLASH[id] ); }
        if ( reader.name() == "mENCODER_LINENUM" )
        { DeviceModel::toValue( reader.readElementText(), &mENCODER_LINENUM[id] ); }
        if ( reader.name() == "mENCODER_CHANNELNUM" )
        { toValue( reader.readElementText(), &mENCODER_CHANNELNUM[id] ); }

        if ( reader.name() == "mENCODER_TYPE" )
        { toValue( reader.readElementText(), &mENCODER_TYPE[id] ); }
        if ( reader.name() == "mENCODER_MULTIPLE" )
        { toValue( reader.readElementText(), &mENCODER_MULTIPLE[id] ); }
        if ( reader.name() == "mENCODER_STATE" )
        { toValue( reader.readElementText(), &mENCODER_STATE[id] ); }
//        if ( reader.name() == "mMOTIONPLAN_PVTCONFIG" )
//        { toValue( reader.readElementText(), &mMOTIONPLAN_PVTCONFIG[id] ); }

//        if ( reader.name() == "mMOTIONPLAN_EXECUTEMODE" )
//        { toValue( reader.readElementText(), &mMOTIONPLAN_EXECUTEMODE[id] ); }

//        if ( reader.name() == "mMOTIONPLAN_PLANMODE" )
//        { toValue( reader.readElementText(), &mMOTIONPLAN_PLANMODE[id] ); }
//        if ( reader.name() == "mMOTIONPLAN_MOTIONMODE" )
//        { toValue( reader.readElementText(), &mMOTIONPLAN_MOTIONMODE[id] ); }
//        if ( reader.name() == "mMOTIONPLAN_MODIFYDUTY" )
//        { toValue( reader.readElementText(), &mMOTIONPLAN_MODIFYDUTY[id] ); }
//        if ( reader.name() == "mMOTIONPLAN_REMAINPOINT" )
//        { DeviceModel::toValue( reader.readElementText(), &mMOTIONPLAN_REMAINPOINT[id] ); }

//        if ( reader.name() == "mMOTIONPLAN_OUTPUTPOINT" )
//        { DeviceModel::toValue( reader.readElementText(), &mMOTIONPLAN_OUTPUTPOINT[id] ); }
//        if ( reader.name() == "mMOTIONPLAN_STARTPOINT" )
//        { DeviceModel::toValue( reader.readElementText(), &mMOTIONPLAN_STARTPOINT[id] ); }
//        if ( reader.name() == "mMOTIONPLAN_ENDPOINT" )
//        { DeviceModel::toValue( reader.readElementText(), &mMOTIONPLAN_ENDPOINT[id] ); }
//        if ( reader.name() == "mMOTIONPLAN_CYCLENUM" )
//        { DeviceModel::toValue( reader.readElementText(), &mMOTIONPLAN_CYCLENUM[id] ); }

//        if ( reader.name() == "mMOTIONPLAN_WARNPOINT" )
//        { DeviceModel::toValue( reader.readElementText(), &mMOTIONPLAN_WARNPOINT[id] ); }
//        if ( reader.name() == "mMOTIONPLAN_ENDSTATE" )
//        { toValue( reader.readElementText(), &mMOTIONPLAN_ENDSTATE[id] ); }
//        if ( reader.name() == "mMOTIONPLAN_FEEDBACKRATIO" )
//        { DeviceModel::toValue( reader.readElementText(), &mMOTIONPLAN_FEEDBACKRATIO[id] ); }
//        if ( reader.name() == "mMOTIONPLAN_CYCLENUM" )
//        { toValue( reader.readElementText(), &mMOTIONPLAN_CYCLENUM[id] ); }

        if ( reader.name() == "mTRIGGER_MODE" )
        { toValue( reader.readElementText(), &mTRIGGER_MODE[id] ); }
        if ( reader.name() == "mTRIGGER_PATTSTATE" )
        { toValue( reader.readElementText(), &mTRIGGER_PATTSTATE[id] ); }

        if ( reader.name() == "mTRIGGER_PATTERN" )
        { toValue( reader.readElementText(), &mTRIGGER_PATTERN[id] ); }
        if ( reader.name() == "mTRIGGER_PATTERN1" )
        { toValue( reader.readElementText(), &mTRIGGER_PATTERN1[id] ); }
//        if ( reader.name() == "mTRIGGER_PATTERN2" )
//        { toValue( reader.readElementText(), &mTRIGGER_PATTERN2[id] ); }
//        if ( reader.name() == "mTRIGGER_PATTERN3" )
//        { toValue( reader.readElementText(), &mTRIGGER_PATTERN3[id] ); }

//        if ( reader.name() == "mTRIGGER_PATTERN4" )
//        { toValue( reader.readElementText(), &mTRIGGER_PATTERN4[id] ); }
        if ( reader.name() == "mTRIGGER_PATTRESP" )
        { toValue( reader.readElementText(), &mTRIGGER_PATTRESP[id] ); }
        if ( reader.name() == "mTRIGGER_PATTSMODE" )
        { toValue( reader.readElementText(), &mTRIGGER_PATTSMODE[id] ); }
        if ( reader.name() == "mTRIGGER_PATTSPERIOD" )
        { DeviceModel::toValue( reader.readElementText(), &mTRIGGER_PATTSPERIOD[id] ); }

        //! level
        if ( reader.name() == "mTRIGGER_PATTRESP" )
        { toValue( reader.readElementText(), &mTRIGGER_PATTRESP[id] ); }
        if ( reader.name() == "mDRIVER_CURRENT" )
        { DeviceModel::toValue( reader.readElementText(), &mDRIVER_CURRENT[id] ); }

        if ( reader.name() == "mDRIVER_MICROSTEPS" )
        { toValue( reader.readElementText(), &mDRIVER_MICROSTEPS[id] ); }
//        if ( reader.name() == "mDRIVER_SWITCH" )
//        { toValue( reader.readElementText(), &mDRIVER_SWITCH[id] ); }
    }

    return 0;
}

void MRQ_model::setSignature( quint32 hash )
{ mSignature = hash; }
quint32 MRQ_model::getSignature()
{ return mSignature; }

QString MRQ_model::getFullDesc( int id )
{
//    return name();
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

}
