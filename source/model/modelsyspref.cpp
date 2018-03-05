#include "modelsyspref.h"

#include "../../include/mcstd.h"

modelSysPref::modelSysPref()
{
    rst();
}

void modelSysPref::rst()
{
    mPort = 0;
    mSpeed = 1000000;

    mTimeout = time_ms(100);
    mInterval = time_us(100);       //! by port
    mFailTryCnt = 2;

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
    mDumpPath = QCoreApplication::applicationDirPath() + "/temp";

    //! database
    mDbMeta.mbUpload = false;
    mDbMeta.mHostName = "127.0.0.1";
    mDbMeta.mDbName = "test";
    mDbMeta.mTableName = "test";
    mDbMeta.mUserName = "root";
    mDbMeta.mPassword = "a";
}

//! save to xml
int modelSysPref::save( const QString &str )
{
    QFile fileOut( str );

    if ( !fileOut.open( QIODevice::WriteOnly) )
    { return ERR_FILE_OPEN_FAIL; }

    QXmlStreamWriter writer( &fileOut );

    //! write
    writer.writeStartDocument();

    writer.writeStartElement("pref");

    writer.writeTextElement( "version", QString::number(mVersion) );

    //! port
    writer.writeTextElement( "port", QString::number(mPort) );
    writer.writeTextElement( "speed", QString::number(mSpeed) );

    //! time
    writer.writeTextElement( "tmo", QString::number(mTimeout) );
    writer.writeTextElement( "interval", QString::number(mInterval) );
    writer.writeTextElement( "fail_try", QString::number(mFailTryCnt) );
    writer.writeTextElement( "enum_tmo", QString::number(mEnumerateTimeout) );

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
    writer.writeTextElement( "sample_tick", QString::number( mSampleTick) );
    writer.writeTextElement( "auto_load", QString::number( mbAutoLoadSetup) );

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

                if ( reader.name() == "port" )
                { mPort = reader.readElementText().toInt(); }

                if ( reader.name() == "speed" )
                { mSpeed = reader.readElementText().toInt(); }

                if ( reader.name() == "tmo" )
                { mTimeout = reader.readElementText().toInt(); }

                if ( reader.name() == "interval" )
                { mInterval = reader.readElementText().toInt(); }

                if ( reader.name() == "fail_try" )
                { mFailTryCnt = reader.readElementText().toInt(); }

                if ( reader.name() == "enum_tmo" )
                { mEnumerateTimeout = reader.readElementText().toInt(); }

                if ( reader.name() == "id" )
                {
                    while( reader.readNextStartElement() )
                    {
                        if ( reader.name() == "recv" )
                        {
                            loadFromTo( reader, mRecvIdFrom, mRecvIdTo );
                        }

                        if ( reader.name() == "send" )
                        {
                            loadFromTo( reader, mSendIdFrom, mSendIdTo );
                        }

                        if ( reader.name() == "group" )
                        {
                            loadFromTo( reader, mGroupIdFrom, mGroupIdTo );
                        }
                    }
                }

                if ( reader.name() == "unit" )
                {
                    while( reader.readNextStartElement() )
                    {
                        if ( reader.name() == "time" )
                        { mTimeUnit = reader.readElementText().toDouble(); }

                        if ( reader.name() == "position" )
                        { mPosUnit = reader.readElementText().toDouble(); }

                        if ( reader.name() == "velocity" )
                        { mVelUnit = reader.readElementText().toDouble(); }
                    }
                }

                if ( reader.name() == "user" )
                {
                    while( reader.readNextStartElement() )
                    {
                        if ( reader.name() == "auto_expand" )
                        { mAutoExpand = reader.readElementText().toInt() > 0; }
                        if ( reader.name() == "search_onopen" )
                        { mbSearchOnOpen = reader.readElementText().toInt() > 0; }
                        if ( reader.name() == "sample_tick" )
                        { mSampleTick = reader.readElementText().toInt(); }
                        if ( reader.name() == "auto_load" )
                        { mbAutoLoadSetup = reader.readElementText().toInt() > 0 ; }
                    }
                }

                if ( reader.name() == "database" )
                {
                    loadDatabase( reader );
                }
            }
        }
    }

    return 0;
}

int modelSysPref::loadFromTo( QXmlStreamReader &reader, int &from, int &to )
{
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "from" )
        { from = reader.readElementText().toInt(); }

        if ( reader.name() == "to" )
        { to = reader.readElementText().toInt(); }
    }

    return 0;
}

int modelSysPref::loadDatabase( QXmlStreamReader &reader )
{
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "upload" )
        { mDbMeta.mbUpload = reader.readElementText().toInt() > 0; }
        if ( reader.name() == "hostname" )
        { mDbMeta.mHostName = reader.readElementText(); }
        if ( reader.name() == "dbname" )
        { mDbMeta.mDbName = reader.readElementText(); }
        if ( reader.name() == "tablename" )
        { mDbMeta.mTableName = reader.readElementText(); }
        if ( reader.name() == "username" )
        { mDbMeta.mUserName = reader.readElementText(); }
        if ( reader.name() == "password" )
        { mDbMeta.mPassword = reader.readElementText(); }
    }

    return 0;
}
