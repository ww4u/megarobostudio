#include "MRV_model.h"

namespace MegaDevice
{

int MRV_model::saveLink( QXmlStreamWriter &writer )
{
    writer.writeTextElement( "mLINK_INTFC", toString(mLINK_INTFC) );

    return 0;
}
int MRV_model::loadLink( QXmlStreamReader &reader )
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

int MRV_model::saveSystem( QXmlStreamWriter &writer )
{
    writer.writeTextElement( "mSYSTEM_WORKMODE", toString(mSYSTEM_WORKMODE) );

    writer.writeTextElement( "mSYSTEM_POWERON", toString(mSYSTEM_POWERON) );

    writer.writeTextElement( "desc", mDesc );
    writer.writeTextElement( "sn", mSn );
    writer.writeTextElement( "sw", mSwVer );
    writer.writeTextElement( "hw", mHwVer );

    writer.writeTextElement( "fw", mFwVer );
    writer.writeTextElement( "bw", mBtVer );

    return 0;
}
int MRV_model::loadSystem( QXmlStreamReader &reader )
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
        { mSn =  reader.readElementText(); }

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

int MRV_model::saveRs232( QXmlStreamWriter &writer )
{
    writer.writeTextElement( "mRS232_BAUD", toString(mRS232_BAUD) );
    writer.writeTextElement( "mRS232_WORDLEN", toString(mRS232_WORDLEN) );
    writer.writeTextElement( "mRS232_FLOWCTL", toString(mRS232_FLOWCTL) );
    writer.writeTextElement( "mRS232_PARITY", toString(mRS232_PARITY) );

    writer.writeTextElement( "mRS232_STOPBIT", toString(mRS232_STOPBIT) );

    return 0;
}
int MRV_model::loadRs232( QXmlStreamReader &reader )
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

int MRV_model::saveCan( QXmlStreamWriter &writer )
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

    writer.writeTextElement( "mCAN_NETMANAGEID", DeviceModel::toString(mCAN_NETMANAGEID) );

    writer.writeTextElement( "mCAN_NETMANAGEHASH", DeviceModel::toString(mCAN_NETMANAGEHASH) );

    return 0;
}
int MRV_model::loadCan( QXmlStreamReader &reader )
{
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "mCAN_TYPE" )
        { toValue( reader.readElementText(), &mCAN_TYPE ); }
        else if ( reader.name() == "mCAN_BAUD" )
        { toValue( reader.readElementText(), &mCAN_BAUD ); }
//        else if ( reader.name() == "mCAN_GROUP" )
//        { DeviceModel::toValue( reader.readElementText(), &mCAN_GROUP ); }
//        else if ( reader.name() == "mCAN_SENDID" )
//        { DeviceModel::toValue( reader.readElementText(), &mCAN_SENDID ); }

//        else if ( reader.name() == "mCAN_RECEIVEID" )
//        { DeviceModel::toValue( reader.readElementText(), &mCAN_RECEIVEID ); }
//        else if ( reader.name() == "mCAN_GROUPID1" )
//        { DeviceModel::toValue( reader.readElementText(), &mCAN_GROUPID1 ); }
//        else if ( reader.name() == "mCAN_GROUPID2" )
//        { DeviceModel::toValue( reader.readElementText(), &mCAN_GROUPID2 ); }
//        else if ( reader.name() == "mCAN_BROADCASTID" )
//        { DeviceModel::toValue( reader.readElementText(), &mCAN_BROADCASTID ); }

        else if ( reader.name() == "mCAN_NETMANAGESTATE" )
        { toValue( reader.readElementText(), &mCAN_NETMANAGESTATE ); }

        else if ( reader.name() == "mCAN_NETMANAGEID" )
        { DeviceModel::toValue( reader.readElementText(), &mCAN_NETMANAGEID ); }

        else if ( reader.name() == "mCAN_NETMANAGEHASH" )
        { DeviceModel::toValue( reader.readElementText(), &mCAN_NETMANAGEHASH ); }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int MRV_model::saveMotion( QXmlStreamWriter &writer )
{
    for ( int i = 0; i < axes(); i++ )
    {
        writer.writeStartElement("axes");

        saveSubMotion( i, writer );

        writer.writeEndElement();
    }

    return 0;
}
int MRV_model::loadMotion( QXmlStreamReader &reader )
{
    int iSub = 0;
    while( reader.readNextStartElement() )
    {
        if (reader.name() == "axes" )
        { loadSubMotion( iSub++, reader ); }

        else
        { reader.skipCurrentElement(); }
    }
    return 0;
}

int MRV_model::saveSubMotion( int iSub, QXmlStreamWriter &writer )
{
    writer.writeTextElement( "mMOTION_STATEREPORT", toString(mMOTION_STATEREPORT[iSub]) );

    return 0;
}
int MRV_model::loadSubMotion( int iSub, QXmlStreamReader &reader )
{
    while( reader.readNextStartElement() )
    {
        if (reader.name() == "mMOTION_STATEREPORT" )
        { toValue( reader.readElementText(), &mMOTION_STATEREPORT[iSub]); }

        else
        { reader.skipCurrentElement(); }
    }
    return 0;
}

int MRV_model::saveGlobal( QXmlStreamWriter &writer )
{
    for ( int i = 0; i < axes(); i++ )
    {
        writer.writeStartElement("axes");

        saveSubGlobal( i, writer );

        writer.writeEndElement();
    }

    return 0;
}
int MRV_model::loadGlobal( QXmlStreamReader &reader )
{
    int iSub = 0;
    while( reader.readNextStartElement() )
    {
        if (reader.name() == "axes" )
        { loadSubGlobal( iSub++, reader ); }

        else
        { reader.skipCurrentElement(); }
    }
    return 0;
}

int MRV_model::saveSubGlobal( int iSub, QXmlStreamWriter &writer )
{
//    writer.writeTextElement( "mGLOBAL_GROUP", toString( mGLOBAL_GROUP[iSub]) ) ;
    writer.writeTextElement( "mGLOBAL_DISTINGUISH", toString( mGLOBAL_DISTINGUISH[iSub]) ) ;

    return 0;
}
int MRV_model::loadSubGlobal( int iSub, QXmlStreamReader &reader )
{
    while( reader.readNextStartElement() )
    {
/*        if (reader.name() == "mGLOBAL_GROUP" )
        { toValue( reader.readElementText(), &mGLOBAL_GROUP[iSub]); }
        else */if (reader.name() == "mGLOBAL_DISTINGUISH" )
        { toValue( reader.readElementText(), &mGLOBAL_DISTINGUISH[iSub]); }

        else
        { reader.skipCurrentElement(); }
    }
    return 0;
}

int MRV_model::saveValve( QXmlStreamWriter &writer )
{
    for ( int i = 0; i < axes(); i++ )
    {
        writer.writeStartElement("axes");

        saveSubValve( i, writer );

        writer.writeEndElement();
    }

    return 0;
}
int MRV_model::loadValve( QXmlStreamReader &reader )
{
    int iSub = 0;
    while( reader.readNextStartElement() )
    {
        if (reader.name() == "axes" )
        { loadSubValve( iSub++, reader ); }

        else
        { reader.skipCurrentElement(); }
    }
    return 0;
}

int MRV_model::saveSubValve( int iSub, QXmlStreamWriter &writer )
{
    writer.writeTextElement( "mVALVECTRL_DEVICE", toString( mVALVECTRL_DEVICE[iSub]) ) ;
    writer.writeTextElement( "mVALVECTRL_PWMFREQ", DeviceModel::toString( mVALVECTRL_PWMFREQ ) ) ;
    writer.writeTextElement( "mVALVECTRL_RTDUTY", DeviceModel::toString( mVALVECTRL_RTDUTY[iSub] ) ) ;
    writer.writeTextElement( "mVALVECTRL_ACTION", toString( mVALVECTRL_ACTION[iSub] ) ) ;

    writer.writeTextElement( "mVALVECTRL_IDLEDUTY", DeviceModel::toString( mVALVECTRL_IDLEDUTY[iSub] ) ) ;
    writer.writeTextElement( "mVALVECTRL_HOLDDUTY", DeviceModel::toString( mVALVECTRL_HOLDDUTY[iSub] ) ) ;
    writer.writeTextElement( "mVALVECTRL_OPENDUTY", DeviceModel::toString( mVALVECTRL_OPENDUTY[iSub] ) ) ;
    writer.writeTextElement( "mVALVECTRL_OPENDLYTIME", DeviceModel::toString( mVALVECTRL_OPENDLYTIME[iSub] ) ) ;

    writer.writeTextElement( "mVALVECTRL_OPENTIME", DeviceModel::toString( mVALVECTRL_OPENTIME[iSub] ) ) ;
    writer.writeTextElement( "mVALVECTRL_RECORDSTATE", toString( mVALVECTRL_RECORDSTATE[iSub] ) ) ;
    writer.writeTextElement( "mVALVECTRL_ONNUMS", DeviceModel::toString( mVALVECTRL_ONNUMS[iSub] ) ) ;
    writer.writeTextElement( "mVALVECTRL_OFFNUMS", DeviceModel::toString( mVALVECTRL_OFFNUMS[iSub] ) ) ;

    writer.writeTextElement( "mVALVECTRL_TIME", DeviceModel::toString( mVALVECTRL_TIME[iSub] ) ) ;

    return 0;
}
int MRV_model::loadSubValve( int iSub, QXmlStreamReader &reader )
{
    while( reader.readNextStartElement() )
    {
        if (reader.name() == "mVALVECTRL_DEVICE" )
        { toValue( reader.readElementText(), &mVALVECTRL_DEVICE[iSub]); }
        else if (reader.name() == "mVALVECTRL_PWMFREQ" )
        { DeviceModel::toValue( reader.readElementText(), &mVALVECTRL_PWMFREQ); }
        else if (reader.name() == "mVALVECTRL_RTDUTY" )
        { DeviceModel::toValue( reader.readElementText(), &mVALVECTRL_RTDUTY[iSub]); }
        else if (reader.name() == "mVALVECTRL_ACTION" )
        { toValue( reader.readElementText(), &mVALVECTRL_ACTION[iSub]); }

        else if (reader.name() == "mVALVECTRL_IDLEDUTY" )
        { DeviceModel::toValue( reader.readElementText(), &mVALVECTRL_IDLEDUTY[iSub]); }
        else if (reader.name() == "mVALVECTRL_HOLDDUTY" )
        { DeviceModel::toValue( reader.readElementText(), &mVALVECTRL_HOLDDUTY[iSub]); }
        else if (reader.name() == "mVALVECTRL_OPENDUTY" )
        { DeviceModel::toValue( reader.readElementText(), &mVALVECTRL_OPENDUTY[iSub]); }
        else if (reader.name() == "mVALVECTRL_OPENDLYTIME" )
        { DeviceModel::toValue( reader.readElementText(), &mVALVECTRL_OPENDLYTIME[iSub]); }

        else if (reader.name() == "mVALVECTRL_OPENTIME" )
        { DeviceModel::toValue( reader.readElementText(), &mVALVECTRL_OPENTIME[iSub]); }
        else if (reader.name() == "mVALVECTRL_RECORDSTATE" )
        { toValue( reader.readElementText(), &mVALVECTRL_RECORDSTATE[iSub]); }
        else if (reader.name() == "mVALVECTRL_ONNUMS" )
        { DeviceModel::toValue( reader.readElementText(), &mVALVECTRL_ONNUMS[iSub]); }
        else if (reader.name() == "mVALVECTRL_OFFNUMS" )
        { DeviceModel::toValue( reader.readElementText(), &mVALVECTRL_OFFNUMS[iSub]); }

        else if (reader.name() == "mVALVECTRL_TIME" )
        { DeviceModel::toValue( reader.readElementText(), &mVALVECTRL_TIME[iSub]); }

        else
        { reader.skipCurrentElement(); }
    }
    return 0;
}

int MRV_model::saveThreshold( QXmlStreamWriter &writer )
{
    for ( int i = 0; i < axes(); i++ )
    {
        writer.writeStartElement("axes");

        saveSubThreshold( i, writer );

        writer.writeEndElement();
    }

    return 0;
}
int MRV_model::loadThreshold( QXmlStreamReader &reader )
{
    int iSub = 0;
    while( reader.readNextStartElement() )
    {
        if (reader.name() == "axes" )
        { loadSubThreshold( iSub++, reader ); }

        else
        { reader.skipCurrentElement(); }
    }
    return 0;
}

int MRV_model::saveSubThreshold( int iSub, QXmlStreamWriter &writer )
{
    writer.writeTextElement( "mTHRESHOLD_HIGHPRESSURE", DeviceModel::toString( mTHRESHOLD_HIGHPRESSURE[iSub]) ) ;
    writer.writeTextElement( "mTHRESHOLD_HIPRESSUREACTION", toString( mTHRESHOLD_HIPRESSUREACTION[iSub]) ) ;
    writer.writeTextElement( "mTHRESHOLD_LOWPRESSURE", DeviceModel::toString( mTHRESHOLD_LOWPRESSURE[iSub]) ) ;

    writer.writeTextElement( "mTHRESHOLD_LOWPRESSUREACTION", toString( mTHRESHOLD_LOWPRESSUREACTION[iSub]) ) ;
    writer.writeTextElement( "mTHRESHOLD_HIGHCURRENT", DeviceModel::toString( mTHRESHOLD_HIGHCURRENT[iSub]) ) ;
    writer.writeTextElement( "mTHRESHOLD_HICURRENTACTION", toString( mTHRESHOLD_HICURRENTACTION[iSub]) ) ;
    writer.writeTextElement( "mTHRESHOLD_LOWCURRENT", DeviceModel::toString( mTHRESHOLD_LOWCURRENT[iSub]) ) ;

    writer.writeTextElement( "mTHRESHOLD_LOWCURRENTACTION", toString( mTHRESHOLD_LOWCURRENTACTION[iSub]) ) ;
    writer.writeTextElement( "mTHRESHOLD_ONNUMS", DeviceModel::toString( mTHRESHOLD_ONNUMS[iSub]) ) ;
    writer.writeTextElement( "mTHRESHOLD_NUMSACTION", toString( mTHRESHOLD_NUMSACTION[iSub]) ) ;
    writer.writeTextElement( "mTHRESHOLD_TIME", DeviceModel::toString( mTHRESHOLD_TIME[iSub]) ) ;

    writer.writeTextElement( "mTHRESHOLD_TIMEACTION", toString( mTHRESHOLD_TIMEACTION[iSub]) ) ;

    return 0;
}
int MRV_model::loadSubThreshold( int iSub, QXmlStreamReader &reader )
{
    while( reader.readNextStartElement() )
    {
        if (reader.name() == "mTHRESHOLD_HIGHPRESSURE" )
        { DeviceModel::toValue( reader.readElementText(), &mTHRESHOLD_HIGHPRESSURE[iSub]); }
        else if (reader.name() == "mTHRESHOLD_HIPRESSUREACTION" )
        { toValue( reader.readElementText(), &mTHRESHOLD_HIPRESSUREACTION[iSub]); }
        else if (reader.name() == "mTHRESHOLD_LOWPRESSURE" )
        { DeviceModel::toValue( reader.readElementText(), &mTHRESHOLD_LOWPRESSURE[iSub]); }


        else if (reader.name() == "mTHRESHOLD_LOWPRESSUREACTION" )
        { toValue( reader.readElementText(), &mTHRESHOLD_LOWPRESSUREACTION[iSub]); }
        else if (reader.name() == "mTHRESHOLD_HIGHCURRENT" )
        { DeviceModel::toValue( reader.readElementText(), &mTHRESHOLD_HIGHCURRENT[iSub]); }
        else if (reader.name() == "mTHRESHOLD_HICURRENTACTION" )
        { toValue( reader.readElementText(), &mTHRESHOLD_HICURRENTACTION[iSub]); }
        else if (reader.name() == "mTHRESHOLD_LOWCURRENT" )
        { DeviceModel::toValue( reader.readElementText(), &mTHRESHOLD_LOWCURRENT[iSub]); }

        else if (reader.name() == "mTHRESHOLD_LOWCURRENTACTION" )
        { toValue( reader.readElementText(), &mTHRESHOLD_LOWCURRENTACTION[iSub]); }
        else if (reader.name() == "mTHRESHOLD_ONNUMS" )
        { DeviceModel::toValue( reader.readElementText(), &mTHRESHOLD_ONNUMS[iSub]); }
        else if (reader.name() == "mTHRESHOLD_NUMSACTION" )
        { toValue( reader.readElementText(), &mTHRESHOLD_NUMSACTION[iSub]); }
        else if (reader.name() == "mTHRESHOLD_TIME" )
        { DeviceModel::toValue( reader.readElementText(), &mTHRESHOLD_TIME[iSub]); }

        else if (reader.name() == "mTHRESHOLD_TIMEACTION" )
        { toValue( reader.readElementText(), &mTHRESHOLD_TIMEACTION[iSub]); }

        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int MRV_model::saveCalibration( QXmlStreamWriter &writer )
{
    for ( int i = 0; i < axes(); i++ )
    {
        writer.writeStartElement("axes");

        saveSubCalibration( i, writer );

        writer.writeEndElement();
    }

    return 0;
}
int MRV_model::loadCalibration( QXmlStreamReader &reader )
{
    int iSub = 0;
    while( reader.readNextStartElement() )
    {
        if (reader.name() == "axes" )
        { loadSubCalibration( iSub++, reader ); }

        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int MRV_model::saveSubCalibration( int iSub, QXmlStreamWriter &writer )
{
    writer.writeTextElement( "mCALIBRATION_TYPE", toString( mCALIBRATION_TYPE[iSub]) ) ;

    writer.writeTextElement( "mCALIBRATION_SAMPLERATE", toString( mCALIBRATION_SAMPLERATE[iSub]) ) ;
    writer.writeTextElement( "mCALIBRATION_OPENTIME", DeviceModel::toString( mCALIBRATION_OPENTIME[iSub]) ) ;
    writer.writeTextElement( "mCALIBRATION_HOLDTIME", DeviceModel::toString( mCALIBRATION_HOLDTIME[iSub]) ) ;
    writer.writeTextElement( "mCALIBRATION_CLOSETIME", DeviceModel::toString( mCALIBRATION_CLOSETIME[iSub]) ) ;

    writer.writeTextElement( "mCALIBRATION_OPENDUTY", DeviceModel::toString( mCALIBRATION_OPENDUTY[iSub]) ) ;
    writer.writeTextElement( "mCALIBRATION_HOLDDUTY", DeviceModel::toString( mCALIBRATION_HOLDDUTY[iSub]) ) ;
    writer.writeTextElement( "mCALIBRATION_DATALENGTH", DeviceModel::toString( mCALIBRATION_DATALENGTH[iSub]) ) ;

    return 0;
}
int MRV_model::loadSubCalibration( int iSub, QXmlStreamReader &reader )
{
    while( reader.readNextStartElement() )
    {
        if (reader.name() == "mCALIBRATION_TYPE" )
        { toValue( reader.readElementText(), &mCALIBRATION_TYPE[iSub]); }

        else if (reader.name() == "mCALIBRATION_SAMPLERATE" )
        { toValue( reader.readElementText(), &mCALIBRATION_SAMPLERATE[iSub]); }
        else if (reader.name() == "mCALIBRATION_OPENTIME" )
        { DeviceModel::toValue( reader.readElementText(), &mCALIBRATION_OPENTIME[iSub]); }
        else if (reader.name() == "mCALIBRATION_HOLDTIME" )
        { DeviceModel::toValue( reader.readElementText(), &mCALIBRATION_HOLDTIME[iSub]); }
        else if (reader.name() == "mCALIBRATION_CLOSETIME" )
        { DeviceModel::toValue( reader.readElementText(), &mCALIBRATION_CLOSETIME[iSub]); }

        else if (reader.name() == "mCALIBRATION_OPENDUTY" )
        { DeviceModel::toValue( reader.readElementText(), &mCALIBRATION_OPENDUTY[iSub]); }
        else if (reader.name() == "mCALIBRATION_HOLDDUTY" )
        { DeviceModel::toValue( reader.readElementText(), &mCALIBRATION_HOLDDUTY[iSub]); }
        else if (reader.name() == "mCALIBRATION_DATALENGTH" )
        { DeviceModel::toValue( reader.readElementText(), &mCALIBRATION_DATALENGTH[iSub]); }

        else
        { reader.skipCurrentElement(); }
    }
    return 0;
}

int MRV_model::saveIOConfig( QXmlStreamWriter &writer )
{
    for ( int i = 0; i < axes(); i++ )
    {
        writer.writeStartElement("axes");

        saveSubIOConfig( i, writer );

        writer.writeEndElement();
    }

    return 0;
}
int MRV_model::loadIOConfig( QXmlStreamReader &reader )
{
    int iSub = 0;
    while( reader.readNextStartElement() )
    {
        if (reader.name() == "axes" )
        { loadSubIOConfig( iSub++, reader ); }

        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int MRV_model::saveSubIOConfig( int iSub, QXmlStreamWriter &writer )
{
    writer.writeTextElement( "mIOCONFIG_IOFUNCSEL", toString( mIOCONFIG_IOFUNCSEL[iSub]) ) ;
    writer.writeTextElement( "mIOCONFIG_SWFILTERDELAY", DeviceModel::toString( mIOCONFIG_SWFILTERDELAY[iSub]) ) ;

    writer.writeTextElement( "mIOCONFIG_SWFILTERCOUNTER", DeviceModel::toString( mIOCONFIG_SWFILTERCOUNTER[iSub]) ) ;
    writer.writeTextElement( "mIOCONFIG_TRIGEDGE", toString( mIOCONFIG_TRIGEDGE[iSub]) ) ;
    writer.writeTextElement( "mIOCONFIG_BAUD", toString( mIOCONFIG_BAUD[iSub]) ) ;
    writer.writeTextElement( "mIOCONFIG_WORDLEN", toString( mIOCONFIG_WORDLEN[iSub]) ) ;

    writer.writeTextElement( "mIOCONFIG_FLOWCTL", toString( mIOCONFIG_FLOWCTL[iSub]) ) ;
    writer.writeTextElement( "mIOCONFIG_PARITY", toString( mIOCONFIG_PARITY[iSub]) ) ;
    writer.writeTextElement( "mIOCONFIG_STOPBIT", toString( mIOCONFIG_STOPBIT[iSub]) ) ;

    return 0;
}
int MRV_model::loadSubIOConfig( int iSub, QXmlStreamReader &reader )
{
    while( reader.readNextStartElement() )
    {
        if (reader.name() == "mIOCONFIG_IOFUNCSEL" )
        { toValue( reader.readElementText(), &mIOCONFIG_IOFUNCSEL[iSub]); }
        else if (reader.name() == "mIOCONFIG_SWFILTERDELAY" )
        { DeviceModel::toValue( reader.readElementText(), &mIOCONFIG_SWFILTERDELAY[iSub]); }

        else if (reader.name() == "mIOCONFIG_SWFILTERCOUNTER" )
        { DeviceModel::toValue( reader.readElementText(), &mIOCONFIG_SWFILTERCOUNTER[iSub]); }
        else if (reader.name() == "mIOCONFIG_TRIGEDGE" )
        { toValue( reader.readElementText(), &mIOCONFIG_TRIGEDGE[iSub]); }
        else if (reader.name() == "mIOCONFIG_BAUD" )
        { toValue( reader.readElementText(), &mIOCONFIG_BAUD[iSub]); }
        else if (reader.name() == "mIOCONFIG_WORDLEN" )
        { toValue( reader.readElementText(), &mIOCONFIG_WORDLEN[iSub]); }

        else if (reader.name() == "mIOCONFIG_FLOWCTL" )
        { toValue( reader.readElementText(), &mIOCONFIG_FLOWCTL[iSub]); }
        else if (reader.name() == "mIOCONFIG_PARITY" )
        { toValue( reader.readElementText(), &mIOCONFIG_PARITY[iSub]); }
        else if (reader.name() == "mIOCONFIG_STOPBIT" )
        { toValue( reader.readElementText(), &mIOCONFIG_STOPBIT[iSub]); }

        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

}

