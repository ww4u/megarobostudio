#include "rawrobo.h"

int RawRobo::serialOutRaw( QXmlStreamWriter &writer )
{
    writer.writeTextElement( "step", QString::number(mPlanAttr.mStep) );
    writer.writeTextElement( "mode", QString::number((int)mPlanAttr.mMode) );

    return 0;
}
int RawRobo::serialInRaw( QXmlStreamReader &reader )
{
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "step" )
        {
            mPlanAttr.mStep = reader.readElementText().toFloat();
        }
        else if ( reader.name() == "mode" )
        {
            mPlanAttr.mMode = (eRoboPlanMode)reader.readElementText().toInt();
        }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}
