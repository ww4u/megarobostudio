#include "vrobot.h"

//! serial
int VRobot::load( const QString &name )
{
    QFile file(name);
    if ( !file.open( QIODevice::ReadOnly) )
    { return ERR_FILE_OPEN_FAIL; }

    QXmlStreamReader reader( &file );

    int ret = 0;
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
                            mClass = reader.readElementText();
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
                                {}
                            }
                        }
                        else if ( reader.name() == "group" )
                        {
                            while(reader.readNextStartElement())
                            {
                                if ( reader.name() == "can_id" )
                                {
                                    mCanGroupId  = reader.readElementText().toInt();
                                    logDbg()<<mCanGroupId;
                                }
                                else if ( reader.name() == "sub_group" )
                                {
                                    mSubGroup  = reader.readElementText().toInt();
                                }
                                else if ( reader.name() == "sub_groupid" )
                                {
                                    mSubGroupId  = reader.readElementText().toInt();
                                    logDbg()<<mSubGroupId;
                                }
                                else
                                {}
                            }
                        }
                        else
                        {}
                    }
                }
                else if ( reader.name() == "setup" )
                {
                    ret = serialIn( reader );
                }
                else
                {

                }
            }
        }
    }

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
        writer.writeTextElement("class",mClass );

            writer.writeStartElement("axes");
            writer.writeTextElement("count", QString::number(mAxes) );

            for ( int i = 0; i < mAxes; i++ )
            {
                writer.writeTextElement("name", mAxesConnectionName.at(i) );
            }

            writer.writeEndElement();

            writer.writeStartElement("group");

                writer.writeTextElement("can_id", QString::number(mCanGroupId) );
                writer.writeTextElement("sub_group", QString::number(mSubGroup) );
                writer.writeTextElement("sub_groupid", QString::number(mSubGroupId) );

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
