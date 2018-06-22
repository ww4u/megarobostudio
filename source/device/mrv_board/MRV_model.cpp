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

        writer.writeStartElement("motion");
        saveMotion( writer );
        writer.writeEndElement();

        writer.writeStartElement("global");
        saveGlobal( writer );
        writer.writeEndElement();

        writer.writeStartElement("valve");
        saveValve( writer );
        writer.writeEndElement();

        writer.writeStartElement("threshold");
        saveThreshold( writer );
        writer.writeEndElement();

        writer.writeStartElement("calibration");
        saveCalibration( writer );
        writer.writeEndElement();

        writer.writeStartElement("ioconfig");
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
                { loadLink( reader ); }
                else if ( reader.name() == "system" )
                { loadSystem(reader); }
                else if ( reader.name() == "rs232" )
                { loadRs232( reader ); }
                else if ( reader.name() == "can" )
                { loadCan( reader); }

                else if ( reader.name() == "motion" )
                { loadMotion( reader ); }
                else if ( reader.name() == "global" )
                { loadGlobal( reader ); }
                else if ( reader.name() == "valve" )
                { loadValve( reader ); }
                else if ( reader.name() == "threshold" )
                { loadThreshold( reader); }
                else if ( reader.name() == "calibration" )
                { loadCalibration( reader); }
                else if ( reader.name() == "ioconfig" )
                { loadIOConfig( reader); }

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
QString MRV_model::getSN()
{
    return mSn;
}
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
