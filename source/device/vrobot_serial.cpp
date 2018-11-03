#include "vrobot.h"

int VRobot::tryLoad()
{
    QString rawPath;
    if ( getPath().length() > 1 )
    { rawPath = getPath(); }
    else
    { rawPath = QDir::currentPath(); }

    QString fullName;
    fullName = rawPath + QDir::separator() + getName() +  setup_d_ext;
    fullName = QDir::toNativeSeparators( fullName );
    QFile file( fullName );
    if ( file.exists() )
    {
        if ( load( fullName ) == 0 )
        {
            setShadow( true );
            setPath( rawPath );
            //! \note name have been set
//            setName( getName() );
            return 0;
        }
        else
        {
            return -1;
        }
    }
    else
    { return -1; }
}

//! serial
int VRobot::load( const QString &name )
{
    QFile file(name);
    if ( !file.open( QIODevice::ReadOnly) )
    { return ERR_FILE_OPEN_FAIL; }

    QXmlStreamReader reader( &file );

    int ret = 0;
    QString strClass;
    while( reader.readNextStartElement() )
    {
        if ( reader.name()=="robot" )
        {
            while(reader.readNextStartElement())
            {
                if ( reader.name()=="meta" )
                {
                    while(reader.readNextStartElement())
                    {
                        if ( reader.name() == "class" )
                        {
                            //! check class
                            strClass = reader.readElementText();
                            if ( mClass.compare( strClass,Qt::CaseInsensitive) == 0 )
                            {}
                            else
                            {
                                sysError( strClass, mClass, QObject::tr("do not match") );
                                return -1;
                            }
                        }
                        else if ( reader.name() == "power" )
                        {
                            mbPwr = reader.readElementText().toInt() > 0;
                        }
                        else if ( reader.name() == "axes" )
                        {
                            mAxesConnectionName.clear();
                            while(reader.readNextStartElement())
                            {
                                if ( reader.name() == "count" )
                                {
                                    mAxes  = reader.readElementText().toInt();
                                }
                                else if ( reader.name() == "name" )
                                {
                                    mAxesConnectionName.append( reader.readElementText() );
                                }
                                else
                                { reader.skipCurrentElement(); }
                            }
                        }
                        else if ( reader.name() == "zero_about" )
                        {
                            while(reader.readNextStartElement())
                            {
                                if ( reader.name() == "speed" )
                                { mZeroSpeed = reader.readElementText().toDouble(); }
                                else if ( reader.name() == "tmo" )
                                { mZeroTmo = reader.readElementText().toInt(); }
                                else  if ( reader.name() == "tick" )
                                { mZeroTick = reader.readElementText().toInt(); }
                                else
                                { reader.skipCurrentElement(); }
                            }

                        }

                        else if ( reader.name() == "group" )
                        {
                            while(reader.readNextStartElement())
                            {
                                if ( reader.name() == "can_id" )
                                {
                                    mCanGroupId  = reader.readElementText().toInt();
                                }
                                else if ( reader.name() == "group_sel" )
                                {
                                    mCanGroupSel =  reader.readElementText().toInt();
                                }
                                else if ( reader.name() == "sub_group" )
                                {
                                    mSubGroup  = reader.readElementText().toInt();
                                }
                                else if ( reader.name() == "sub_groupid" )
                                {
                                    mSubGroupId  = reader.readElementText().toInt();
                                }
                                else
                                { reader.skipCurrentElement(); }
                            }
                        }

                        else if ( reader.name() == "joint_zero" )
                        {
                            int subId = 0;
                            while(reader.readNextStartElement())
                            {
                                if ( reader.name() == "ccw" )
                                {
                                    mJointZeroCcw[ subId++ ]  = reader.readElementText().toInt() > 0;
                                }
                                else
                                { reader.skipCurrentElement(); }
                            }
                        }

                        else if ( reader.name() == "angle_dir" )
                        {
                            int subId = 0;
                            while(reader.readNextStartElement())
                            {
                                if ( reader.name() == "dir" )
                                {
                                    mAngleDir[ subId++ ]  = reader.readElementText().toInt() > 0;
                                }
                                else
                                { reader.skipCurrentElement(); }
                            }
                        }

                        else if ( reader.name() == "geomerty" )
                        {
                            while( reader.readNextStartElement() )
                            {
                                if ( reader.name() == "coord" )
                                { mCoord = (eRoboCoord)reader.readElementText().toInt(); }
                                else
                                { reader.skipCurrentElement(); }
                            }
                        }

                        else
                        { reader.skipCurrentElement(); }
                    }
                }
                else if ( reader.name() == "setup" )
                {
                    ret = serialIn( reader );
                }
                else
                {
                    reader.skipCurrentElement();
                }
            }
        }
    }

    postload();

    return ret;
}

int VRobot::save( const QString &name )
{
    QFile file(name);

    if ( !file.open( QIODevice::WriteOnly) )
    { return ERR_FILE_OPEN_FAIL; }

    QXmlStreamWriter writer( &file );

    int ret;
    writer.writeStartDocument();

    writer.writeStartElement("robot");

        //! base
        writer.writeStartElement("meta");

            writer.writeTextElement("class", mClass );
            writer.writeTextElement("power", QString::number( mbPwr) );

            writer.writeStartElement("axes");
                writer.writeTextElement("count", QString::number(mAxes) );

                for ( int i = 0; i < axes(); i++ )
                {
                    writer.writeTextElement("name", mAxesConnectionName.at(i) );
                }

            writer.writeEndElement();

            writer.writeStartElement("zero_about");
                writer.writeTextElement("speed", QString::number(mZeroSpeed) );
                writer.writeTextElement("tmo", QString::number(mZeroTmo) );
                writer.writeTextElement("tick", QString::number(mZeroTick) );
            writer.writeEndElement();

            writer.writeStartElement("group");

                writer.writeTextElement("group_sel", QString::number(mCanGroupSel) );
                writer.writeTextElement("can_id", QString::number(mCanGroupId) );
                writer.writeTextElement("sub_group", QString::number(mSubGroup) );
                writer.writeTextElement("sub_groupid", QString::number(mSubGroupId) );

            writer.writeEndElement();

            //! zero ccw
            writer.writeStartElement("joint_zero");

            for ( int i = 0; i < mJointZeroCcw.size(); i++ )
            {
                writer.writeTextElement("ccw", QString::number(mJointZeroCcw.at(i)) );
            }

            writer.writeEndElement();

            //! angle dir
            writer.writeStartElement("angle_dir");

            for ( int i = 0; i < mAngleDir.size(); i++ )
            {
                writer.writeTextElement("dir", QString::number(mAngleDir.at(i)) );
            }

            writer.writeEndElement();

            //! coord
            writer.writeStartElement( "geometry" );
                writer.writeTextElement( "coord", QString::number( (int)mCoord ) );
            writer.writeEndElement();

        writer.writeEndElement();

        //! extend
        writer.writeStartElement("setup");
            ret = serialOut( writer );
        writer.writeEndElement();

    writer.writeEndElement();

    writer.writeEndDocument();

    file.close();

    return ret;
}

int VRobot::serialIn( QXmlStreamReader &/*reader*/ )
{
    return 0;
}

int VRobot::serialOut( QXmlStreamWriter &/*writer*/ )
{
    return 0;
}

void VRobot::postload()
{}

int VRobot::uploadSetting()
{ return 0; }
int VRobot::applySetting()
{ return 0; }

quint32 VRobot::getSignature()
{ return 0; }

QString VRobot::getSN()
{ return mSN; }
