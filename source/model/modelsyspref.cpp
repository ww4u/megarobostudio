#include "modelsyspref.h"

#include "../../include/mcstd.h"

modelSysPref::modelSysPref()
{
    rst();
}

void modelSysPref::rst()
{
    mPort = 0;
    mSpeed = 1000000;               //! 1M index

    mTimeout = time_ms(100);
    mRecvTmo = 1;                   //! ms
    mInterval = time_us(100);       //! by port
    mTpvInterval = time_ms( 1 );
    mFailTryCnt = 1;
    mbAutoAssignId = false;
    mDeviceId = 0;
    mDeviceCount = 1;

    mVisaTmo = 2000;                //! 2s

    mVisaLanAddr = "TCPIP::172.16.0.1::INSTR";
    mVisaLanList.clear();

    mVisaUsbAddr.clear();
    mVisaUsbList.clear();

    mVisa232Addr.clear();
    mVisa232List.clear();

    mVisaEAddr.clear();
    mVisaEList.clear();
                                    //! rs232
    mBaudIndex = 7;
    mDataWidthIndex = 0;
    mParityIndex = 0;
    mStopIndex = 0;
    mFlowControlIndex = 0;

    mRecvIdFrom = receive_id_from;  //! id range
    mRecvIdTo = receive_id_to;

    mSendIdFrom = send_id_from;
    mSendIdTo = send_id_to;

    mGroupIdFrom = group_id_from;
    mGroupIdTo = group_id_to;

    mTimeUnit = 1.0;
    mPosUnit = 1.0;
    mVelUnit = 1.0;

    mAutoExpand = true;
    mbSearchOnOpen = true;
    mbStopOnSearch = false;
    mbMaximizeStartup = true;
    mbAutoLoadPrj = true;
    mbAffirmZero = true;
    mbAutoStatusView = true;
    mbShowNotice = true;
    mbShowHelp = true;
    mbStateInterrupt = false;
    mbNewAutoAdd = true;

    mDumpPath = QCoreApplication::applicationDirPath() + "/temp";
    mEventLogFile = "eventlog.dat";

    //! database
    mDbMeta.mbUpload = false;
    mDbMeta.mHostName = "127.0.0.1";
    mDbMeta.mDbName = "test";
    mDbMeta.mTableName = "test";
    mDbMeta.mUserName = "root";
    mDbMeta.mPassword = "a";

    //! misa
    mMisaEn = true;
    mMisaSocket = 1234;
    mMisaPortCnt = 16;

    mComEn = true;
    mComName = "COM6";

    mRemoteDirPath.clear();

    //! motion
    mGeometryResolution = 5;    //! mm
    mAngleResolution = 0.01;    //! deg
    mOmitEn = true;
    mOmitThreshold = 0.5;       //! deg

    mMaxSpeed = 100;
    mMaxTerminalSpeed = 100;

    //! font
    mFontFamily = "arial";
    mPointSize = 12;

    mLangIndex = 0;
    mStyleIndex = 0;

    mAliasEn = true;

    mAppStartDelay = 30;

    mSysMode = sys_normal;
}

#include "../../com/comassist.h"
//! save to xml
int modelSysPref::save( const QString &str )
{
    //! check path
    QString path = QDir::homePath() + "/megarobostudio";

    if ( QDir( path ).exists() )
    { }
    else
    {
        if ( QDir().mkdir( path) )
        {  }
        else
        { logDbg()<<path; return ERR_FILE_OPEN_FAIL; }
    }

    //! save file
    QFile fileOut( str );

    if ( !fileOut.open( QIODevice::WriteOnly) )
    { logDbg()<<str; return ERR_FILE_OPEN_FAIL; }

    QXmlStreamWriter writer( &fileOut );

    //! write
    writer.writeStartDocument();

    writer.writeStartElement("pref");

    writer.writeTextElement( "version", QString::number(mVersion) );

    //! port
    writer.writeTextElement( "port", QString::number(mPort) );
    writer.writeTextElement( "speed", QString::number(mSpeed) );

    //! lan
    writer.writeTextElement( "visa_lan_addr",( mVisaLanAddr ) );
    writer.writeTextElement( "visa_lan_list", mVisaLanList.join(',') );
    //! rs232
    writer.writeTextElement( "visa_232_addr",( mVisa232Addr ) );
    writer.writeTextElement( "visa_232_list", mVisa232List.join(',') );
    //! usb
    writer.writeTextElement( "visa_usb_addr",( mVisaUsbAddr ) );
    writer.writeTextElement( "visa_usb_list", mVisaUsbList.join(',') );
    //! usb
    writer.writeTextElement( "visa_e_addr",( mVisaEAddr ) );
    writer.writeTextElement( "visa_e_list", mVisaEList.join(',') );

    writer.writeTextElement( "visa_tmo", QString::number( mVisaTmo ) );

    //! rs232
    writer.writeStartElement("rs232");
    {
        writer.writeTextElement( "baud", QString::number(mBaudIndex) );
        writer.writeTextElement( "datawidth", QString::number(mDataWidthIndex) );
        writer.writeTextElement( "parity", QString::number(mParityIndex) );
        writer.writeTextElement( "stop", QString::number(mStopIndex) );

        writer.writeTextElement( "flow", QString::number(mFlowControlIndex) );
    }
    writer.writeEndElement();

    //! time
    writer.writeTextElement( "tmo", QString::number(mTimeout) );
    writer.writeTextElement( "recv_tmo", QString::number(mRecvTmo) );
    writer.writeTextElement( "interval", QString::number(mInterval) );
    writer.writeTextElement( "fail_try", QString::number(mFailTryCnt) );
    writer.writeTextElement( "enum_tmo", QString::number(mEnumerateTimeout) );
    writer.writeTextElement( "tpv_interval", QString::number(mTpvInterval) );
    writer.writeTextElement( "auto_assign_id", QString::number( mbAutoAssignId ) );
    writer.writeTextElement( "device_count", QString::number( mDeviceCount ) );
    writer.writeTextElement( "device_id", QString::number( mDeviceId ) );

    //! ids
    writer.writeStartElement("id");

    writer.writeStartElement("recv");
    writer.writeTextElement( "from", QString::number(mRecvIdFrom) );
    writer.writeTextElement( "to", QString::number(mRecvIdTo) );
    writer.writeEndElement();

    writer.writeStartElement("send");
    writer.writeTextElement( "from", QString::number(mSendIdFrom) );
    writer.writeTextElement( "to", QString::number(mSendIdTo) );
    writer.writeEndElement();

    writer.writeStartElement("group");
    writer.writeTextElement( "from", QString::number(mGroupIdFrom) );
    writer.writeTextElement( "to", QString::number(mGroupIdTo) );
    writer.writeEndElement();

    writer.writeEndElement();

    //! unit
    writer.writeStartElement("unit");

    writer.writeTextElement( "time", QString::number( mTimeUnit) );
    writer.writeTextElement( "position", QString::number( mPosUnit) );
    writer.writeTextElement( "velocity", QString::number( mVelUnit) );

    writer.writeEndElement();

    //! user
    writer.writeStartElement("user");

    writer.writeTextElement( "auto_expand", QString::number( mAutoExpand) );
    writer.writeTextElement( "search_onopen", QString::number( mbSearchOnOpen) );
    writer.writeTextElement( "stop_on_search", QString::number( mbStopOnSearch) );
    writer.writeTextElement( "sample_tick", QString::number( mSampleTick) );
    writer.writeTextElement( "auto_load", QString::number( mbAutoLoadSetup) );
    writer.writeTextElement( "max_startup", QString::number( mbMaximizeStartup) );
    writer.writeTextElement( "auto_load_prj", QString::number( mbAutoLoadPrj) );
    writer.writeTextElement( "affirm_zero", QString::number( mbAffirmZero) );
    writer.writeTextElement( "auto_status", QString::number( mbAutoStatusView) );
    writer.writeTextElement( "show_notice", QString::number(mbShowNotice) );
    writer.writeTextElement( "show_help", QString::number(mbShowHelp) );
    writer.writeTextElement( "state_interrupt", QString::number(mbStateInterrupt) );
    writer.writeTextElement( "new_auto_add", QString::number(mbNewAutoAdd) );


    writer.writeTextElement( "language_id", QString::number(mLangIndex) );
    writer.writeTextElement( "style_id", QString::number(mStyleIndex) );
    writer.writeTextElement( "latest_prj_path", mLatestPrjPath );
    writer.writeTextElement( "latest_prj_name", mLatestPrjName );

    writer.writeTextElement( "dump", mDumpPath );
    writer.writeTextElement( "event_log", mEventLogFile );

    writer.writeEndElement();

    //! data base
    writer.writeStartElement("database");

    writer.writeTextElement( "upload", QString::number( mDbMeta.mbUpload ) );
    writer.writeTextElement( "hostname", mDbMeta.mHostName );
    writer.writeTextElement( "dbname", mDbMeta.mDbName );
    writer.writeTextElement( "tablename", mDbMeta.mTableName );

    writer.writeTextElement( "username", mDbMeta.mUserName );
    writer.writeTextElement( "password", mDbMeta.mPassword );

    writer.writeEndElement();

    //! misa
    writer.writeStartElement("misa");

    writer.writeTextElement( "enable", QString::number( mMisaEn ) );
    writer.writeTextElement( "socket", QString::number( mMisaSocket ) );
    writer.writeTextElement( "port_cnt", QString::number( mMisaPortCnt ) );

    writer.writeTextElement( "com_enable", QString::number( mComEn ) );
    writer.writeTextElement( "com_port", ( mComName ) );

    writer.writeTextElement( "path", mRemoteDirPath );

    writer.writeEndElement();

    //! motion
    writer.writeStartElement("motion");

    writer.writeTextElement( "geomerty_resolution", QString::number( mGeometryResolution ) );
    writer.writeTextElement( "angle_resolution", QString::number( mAngleResolution ) );

    writer.writeTextElement( "omit_enable", QString::number(mOmitEn) );
    writer.writeTextElement( "omit_threshold", QString::number(mOmitThreshold) );

    writer.writeTextElement( "max_speed", QString::number(mMaxSpeed) );
    writer.writeTextElement( "max_terminal_speed", QString::number(mMaxTerminalSpeed) );

    writer.writeEndElement();

    //! font
    writer.writeStartElement("font");

    writer.writeTextElement( "family", mFontFamily );
    writer.writeTextElement( "size", QString::number( mPointSize ) );

    writer.writeEndElement();

    //! alias
    writer.writeStartElement("alias");
    writer.writeTextElement("enable", QString::number( mAliasEn ) );
    for( int i = 0; i < mAlias.mItems.size(); i++ )
    {
        writer.writeStartElement("item");

            writer.writeTextElement("name", mAlias.mItems.at(i)->name() );
            writer.writeTextElement("value", mAlias.mItems.at(i)->value() );

        writer.writeEndElement();
    }
    writer.writeEndElement();

    //! app model
    writer.writeStartElement("apps");
    writer.writeTextElement( "delay", QString::number( mAppStartDelay ) );
    for ( int i = 0; i < mAppModel.mItems.size(); i++ )
    {
        writer.writeStartElement("app");

            writer.writeTextElement("autostart", QString::number( mAppModel.mItems.at(i)->autoStart() ) );
            writer.writeTextElement("program", mAppModel.mItems.at(i)->program() );
            writer.writeTextElement("argument", mAppModel.mItems.at(i)->argument() );
            writer.writeTextElement("comment", mAppModel.mItems.at(i)->comment() );

        writer.writeEndElement();
    }
    writer.writeEndElement();

    //! event action model
    writer.writeStartElement("event_action");
        mEventActionModel.serialOut( writer );
    writer.writeEndElement();

    writer.writeEndElement();

    writer.writeEndDocument();

    return 0;
}

int modelSysPref::load( const QString &str )
{
    //! check ver
    QFile fileIn(str);
    if ( !fileIn.open( QIODevice::ReadOnly) )
    { return ERR_FILE_OPEN_FAIL; }

    QXmlStreamReader reader( &fileIn );

    //! start element
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "pref" )
        {
            while( reader.readNextStartElement() )
            {
                if ( reader.name() == "version" )
                { mVersion = reader.readElementText().toInt(); }

                else if ( reader.name() == "port" )
                { mPort = reader.readElementText().toInt(); }

                else if ( reader.name() == "speed" )
                { mSpeed = reader.readElementText().toInt(); }

                //! lan
                else if ( reader.name() == "visa_lan_addr" )
                { mVisaLanAddr = reader.readElementText(); }

                else if ( reader.name() == "visa_lan_list" )
                { mVisaLanList = reader.readElementText().split(',',QString::SkipEmptyParts); }

                //! rs232
                else if ( reader.name() == "visa_232_addr" )
                { mVisa232Addr = reader.readElementText(); }

                else if ( reader.name() == "visa_232_list" )
                { mVisa232List = reader.readElementText().split(',',QString::SkipEmptyParts); }

                //! usb
                else if ( reader.name() == "visa_usb_addr" )
                { mVisaUsbAddr = reader.readElementText(); }

                else if ( reader.name() == "visa_usb_list" )
                { mVisaUsbList = reader.readElementText().split(',',QString::SkipEmptyParts); }

                //! -e
                else if ( reader.name() == "visa_e_addr" )
                { mVisaEAddr = reader.readElementText(); }

                else if ( reader.name() == "visa_e_list" )
                { mVisaEList = reader.readElementText().split(',',QString::SkipEmptyParts); }

                else if ( reader.name() == "rs232" )
                {
                    while( reader.readNextStartElement() )
                    {
                        if ( reader.name() == "baud" )
                        { mBaudIndex = reader.readElementText().toInt(); }
                        else if ( reader.name() == "datawidth" )
                        { mDataWidthIndex = reader.readElementText().toInt(); }
                        else if ( reader.name() == "parity" )
                        { mParityIndex = reader.readElementText().toInt(); }
                        else if ( reader.name() == "stop" )
                        { mStopIndex = reader.readElementText().toInt(); }
                        else if ( reader.name() == "flow" )
                        { mFlowControlIndex = reader.readElementText().toInt(); }
                        else
                        { reader.skipCurrentElement(); }
                    }
                }

                //! tmo
                else if ( reader.name() == "visa_tmo" )
                { mVisaTmo = reader.readElementText().toInt(); }

                else if ( reader.name() == "tmo" )
                { mTimeout = reader.readElementText().toInt(); }

                else if ( reader.name() == "recv_tmo" )
                { mRecvTmo = reader.readElementText().toInt(); }

                else if ( reader.name() == "interval" )
                { mInterval = reader.readElementText().toInt(); }

                else if ( reader.name() == "fail_try" )
                { mFailTryCnt = reader.readElementText().toInt(); }

                else if ( reader.name() == "enum_tmo" )
                { mEnumerateTimeout = reader.readElementText().toInt(); }

                else if ( reader.name() == "tpv_interval" )
                { mTpvInterval = reader.readElementText().toInt(); }

                else if ( reader.name() == "auto_assign_id" )
                { mbAutoAssignId = reader.readElementText().toInt() > 0; }

                else if ( reader.name() == "device_count" )
                { mDeviceCount = reader.readElementText().toInt(); }

                else if ( reader.name() == "device_id" )
                { mDeviceId = reader.readElementText().toInt(); }

                else if ( reader.name() == "id" )
                {
                    while( reader.readNextStartElement() )
                    {
                        if ( reader.name() == "recv" )
                        {
                            loadFromTo( reader, mRecvIdFrom, mRecvIdTo );
                        }
                        else if ( reader.name() == "send" )
                        {
                            loadFromTo( reader, mSendIdFrom, mSendIdTo );
                        }
                        else if ( reader.name() == "group" )
                        {
                            loadFromTo( reader, mGroupIdFrom, mGroupIdTo );
                        }
                        else
                        { reader.skipCurrentElement(); }
                    }
                }

                else if ( reader.name() == "unit" )
                {
                    while( reader.readNextStartElement() )
                    {
                        if ( reader.name() == "time" )
                        { mTimeUnit = reader.readElementText().toDouble(); }
                        else if ( reader.name() == "position" )
                        { reader.skipCurrentElement(); mPosUnit = 1.0; /*mPosUnit = reader.readElementText().toDouble();*/ }
                        else if ( reader.name() == "velocity" )
                        { reader.skipCurrentElement(); mVelUnit = 1.0; /*mVelUnit = reader.readElementText().toDouble();*/ }
                        else
                        { reader.skipCurrentElement(); }
                    }
                }

                else if ( reader.name() == "user" )
                {
                    while( reader.readNextStartElement() )
                    {
                        if ( reader.name() == "auto_expand" )
                        { mAutoExpand = reader.readElementText().toInt() > 0; }
                        else if ( reader.name() == "search_onopen" )
                        { mbSearchOnOpen = reader.readElementText().toInt() > 0; }
                        else if ( reader.name() == "stop_on_search" )
                        { mbStopOnSearch = reader.readElementText().toInt() > 0; }
                        else if ( reader.name() == "sample_tick" )
                        { mSampleTick = reader.readElementText().toInt(); }
                        else if ( reader.name() == "auto_load" )
                        { mbAutoLoadSetup = reader.readElementText().toInt() > 0 ; }
                        else if ( reader.name() == "max_startup" )
                        { mbMaximizeStartup = reader.readElementText().toInt() > 0 ; }
                        else if ( reader.name() == "auto_load_prj" )
                        { mbAutoLoadPrj = reader.readElementText().toInt() > 0; }
                        else if ( reader.name() == "affirm_zero" )
                        { mbAffirmZero = reader.readElementText().toInt() > 0; }
                        else if ( reader.name() == "auto_status" )
                        { mbAutoStatusView = reader.readElementText().toInt() > 0; }

                        else if ( reader.name() == "show_notice" )
                        { mbShowNotice = reader.readElementText().toInt() > 0; }

                        else if ( reader.name() == "show_help" )
                        { mbShowHelp = toBool( reader ); }

                        else if ( reader.name() == "state_interrupt" )
                        { mbStateInterrupt = toBool( reader ); }

                        else if ( reader.name() == "new_auto_add" )
                        { mbNewAutoAdd = toBool( reader ); }

                        else if ( reader.name() == "language_id" )
                        { mLangIndex = reader.readElementText().toInt(); }
                        else if ( reader.name() == "style_id" )
                        { mStyleIndex = reader.readElementText().toInt() > 0 ; }
                        else if ( reader.name() == "latest_prj_path" )
                        { mLatestPrjPath = reader.readElementText(); }
                        else if ( reader.name() == "latest_prj_name" )
                        { mLatestPrjName = reader.readElementText(); }
                        else if ( reader.name() == "dump" )
                        { mDumpPath = reader.readElementText(); }
                        else if ( reader.name() == "event_log" )
                        { mEventLogFile = reader.readElementText(); }
                        else
                        { reader.skipCurrentElement(); }
                    }
                }

                else if ( reader.name() == "database" )
                {
                    loadDatabase( reader );
                }

                else if ( reader.name() == "misa" )
                {
                    while( reader.readNextStartElement() )
                    {
                        if ( reader.name() == "enable" )
                        { mMisaEn = reader.readElementText().toInt() > 0; }
                        else if ( reader.name() == "socket" )
                        { mMisaSocket = reader.readElementText().toInt(); }
                        else if ( reader.name() == "port_cnt" )
                        { mMisaPortCnt = reader.readElementText().toInt(); }

                        else if ( reader.name() == "com_enable" )
                        { mComEn = reader.readElementText().toInt() > 0; }
                        else if ( reader.name() == "com_port" )
                        { mComName = reader.readElementText(); }

                        else if ( reader.name() == "path" )
                        { mRemoteDirPath = reader.readElementText(); }
                        else
                        { reader.skipCurrentElement(); }
                    }
                }

                else if ( reader.name() == "motion" )
                {
                    while( reader.readNextStartElement() )
                    {
                        if ( reader.name() == "geometry_resolution" )
                        { mGeometryResolution = reader.readElementText().toDouble(); }
                        else if ( reader.name() == "angle_resolution" )
                        { mAngleResolution = reader.readElementText().toDouble(); }
                        else if ( reader.name() == "omit_enable" )
                        { mOmitEn = reader.readElementText().toInt() > 0; }
                        else if ( reader.name() == "omit_threshold" )
                        { mOmitThreshold = reader.readElementText().toDouble(); }
                        else if ( reader.name() == "max_speed" )
                        { mMaxSpeed = reader.readElementText().toDouble(); }
                        else if ( reader.name() == "max_terminal_speed" )
                        { mMaxTerminalSpeed = reader.readElementText().toDouble(); }
                        else
                        { reader.skipCurrentElement(); }
                    }
                }
                else if ( reader.name() == "font" )
                {
                    while( reader.readNextStartElement() )
                    {
                        if ( reader.name() == "family" )
                        { mFontFamily = reader.readElementText(); }
                        else if ( reader.name() == "size" )
                        { mPointSize = reader.readElementText().toInt(); }
                        else
                        { reader.skipCurrentElement(); }
                    }
                }
                else if ( reader.name() == "alias" )
                {
                    Relation *pItem;
                    while( reader.readNextStartElement() )
                    {
                        if ( reader.name() == "enable" )
                        { mAliasEn = reader.readElementText().toInt()> 0; }
                        else if ( reader.name() == "item" )
                        {
                            pItem = new Relation();
                            Q_ASSERT( NULL != pItem );
                            while( reader.readNextStartElement() )
                            {
                                if ( reader.name() == "name" )
                                { pItem->setName( reader.readElementText());}
                                else if ( reader.name() == "value" )
                                { pItem->setValue( reader.readElementText()); }
                                else
                                { reader.skipCurrentElement(); }
                            }
                            mAlias.mItems.append( pItem );
                        }
                        else
                        { reader.skipCurrentElement(); }
                    }
                }

                else if ( reader.name() == "apps" )
                {
                    AppItem *pItem;
                    while( reader.readNextStartElement() )
                    {
                        if ( reader.name() == "delay" )
                        {
                            mAppStartDelay = reader.readElementText().toInt();
                        }
                        else if ( reader.name() == "app" )
                        {
                            pItem = new AppItem();
                            Q_ASSERT( NULL != pItem );
                            while( reader.readNextStartElement() )
                            {
                                if ( reader.name() == "autostart" )
                                { pItem->setAutoStart( toBool(reader) ); }
                                else if ( reader.name() == "program" )
                                { pItem->setProgram( reader.readElementText()); }
                                else if ( reader.name() == "argument" )
                                { pItem->setArgument( reader.readElementText()); }
                                else if ( reader.name() == "comment" )
                                { pItem->setComment( reader.readElementText()); }
                                else
                                { reader.skipCurrentElement(); }
                            }
                            mAppModel.mItems.append( pItem );
                        }
                        else
                        { reader.skipCurrentElement(); }
                    }
                }

                else if ( reader.name() =="event_action" )
                {
                    mEventActionModel.serialIn( reader );
                }

                else
                { reader.skipCurrentElement(); }
            }
        }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int modelSysPref::load( const QString &a, const QString &b )
{
    //! find a
    if ( QFile::exists( a ) )
    {
        if ( 0 == load( a ) )
        { return 0; }
    }

    if ( QFile::exists( b ) )
    {
        if ( 0 == load( b ) )
        { return 0; }
    }

    //! load fail
    rst();

    return -1;
}

int modelSysPref::loadFromTo( QXmlStreamReader &reader, int &from, int &to )
{
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "from" )
        { from = reader.readElementText().toInt(); }

        else if ( reader.name() == "to" )
        { to = reader.readElementText().toInt(); }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int modelSysPref::loadDatabase( QXmlStreamReader &reader )
{
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "upload" )
        { mDbMeta.mbUpload = reader.readElementText().toInt() > 0; }
        else if ( reader.name() == "hostname" )
        { mDbMeta.mHostName = reader.readElementText(); }
        else if ( reader.name() == "dbname" )
        { mDbMeta.mDbName = reader.readElementText(); }
        else if ( reader.name() == "tablename" )
        { mDbMeta.mTableName = reader.readElementText(); }
        else if ( reader.name() == "username" )
        { mDbMeta.mUserName = reader.readElementText(); }
        else if ( reader.name() == "password" )
        { mDbMeta.mPassword = reader.readElementText(); }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

void modelSysPref::setLatestPrj( const QString &path,
                                 const QString &name )
{
    mLatestPrjPath = path;
    mLatestPrjName = name;
}
QString& modelSysPref::latestPrjPath()
{ return mLatestPrjPath; }
QString& modelSysPref::latestPrjName()
{ return mLatestPrjName; }

int modelSysPref::findAlias( const QString &sn, QString &alias )
{
    QString strLower = sn.toLower();

    for ( int i = 0; i < mAlias.mItems.size(); i++ )
    {
        if ( str_is( strLower, mAlias.mItems.at(i)->name()))
        {
            alias = mAlias.mItems.at(i)->value();
            if ( alias.size() > 1 )
            { return 0; }
            else
            { return -1; }
        }
    }

    return -1;
}

