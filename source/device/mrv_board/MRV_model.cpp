#include "MRV_model.h"
namespace MegaDevice
{
MRV_model::MRV_model()
{
    mSignature = 0;

}

int MRV_model::serialOut( QXmlStreamWriter &writer )
{
    writer.writeStartElement("mrv");

        writer.writeStartElement("link");logDbg();
        saveLink( writer );
        writer.writeEndElement();

        writer.writeStartElement("system");logDbg();
        saveSystem( writer );
        writer.writeEndElement();

        writer.writeStartElement("rs232");logDbg();
        saveRs232( writer );
        writer.writeEndElement();

        writer.writeStartElement("can");logDbg();
        saveCan( writer );
        writer.writeEndElement();

        writer.writeStartElement("motion");logDbg();
        saveMotion( writer );
        writer.writeEndElement();

        writer.writeStartElement("global");logDbg();
        saveGlobal( writer );
        writer.writeEndElement();

        writer.writeStartElement("valve");logDbg();
        saveValve( writer );
        writer.writeEndElement();

        writer.writeStartElement("threshold");logDbg();
        saveThreshold( writer );
        writer.writeEndElement();

        writer.writeStartElement("calibration");logDbg();
        saveCalibration( writer );
        writer.writeEndElement();

        writer.writeStartElement("ioconfig");logDbg();
        saveIOConfig( writer );
        writer.writeEndElement();

    writer.writeEndElement();

    return 0;
}

int MRV_model::serialIn( QXmlStreamReader &reader )
{
    //! start element
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "mrv" )
        {
            while( reader.readNextStartElement() )
            {
                if ( reader.name() == "link" )
                { loadLink( reader );logDbg(); }
                else if ( reader.name() == "system" )
                { loadSystem(reader);logDbg(); }
                else if ( reader.name() == "rs232" )
                { loadRs232( reader );logDbg(); }
                else if ( reader.name() == "can" )
                { loadCan( reader);logDbg(); }

                else if ( reader.name() == "motion" )
                { loadMotion( reader );logDbg(); }
                else if ( reader.name() == "global" )
                { loadGlobal( reader );logDbg(); }
                else if ( reader.name() == "valve" )
                { loadValve( reader );logDbg(); }
                else if ( reader.name() == "threshold" )
                { loadThreshold( reader);logDbg(); }
                else if ( reader.name() == "calibration" )
                { loadCalibration( reader);logDbg(); }
                else if ( reader.name() == "ioconfig" )
                { loadIOConfig( reader);logDbg(); }

                else
                { reader.skipCurrentElement(); }
            }
        }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

void MRV_model::setSignature( quint32 hash )
{ mSignature = hash; }
quint32 MRV_model::getSignature()
{ return mSignature; }

QString MRV_model::getFullDesc( int id )
{
    return QString( "%1(%2)").arg( mDesc ).arg( getName() ) ;
}

QString MRV_model::getDesc()
{ return mDesc; }

QString MRV_model::getSwVer()
{ return mSwVer; }
QString MRV_model::getHwVer()
{ return mHwVer; }
QString MRV_model::getFwVer()
{ return mFwVer; }
QString MRV_model::getBtVer()
{ return mBtVer; }

QString MRV_model::getSeqVer()
{ return mSeqVer; }

}
