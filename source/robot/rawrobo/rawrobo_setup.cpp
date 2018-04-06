#include "rawrobo.h"

int RawRobo::serialOutRaw( QXmlStreamWriter &writer )
{
    writer.writeTextElement( "step", QString::number(mPlanStep) );
    writer.writeTextElement( "mode", QString::number((int)mPlanMode) );

    return 0;
}
int RawRobo::serialInRaw( QXmlStreamReader &reader )
{
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "step" )
        {
            mPlanStep = reader.readElementText().toFloat();
        }
        else if ( reader.name() == "mode" )
        {
            mPlanMode = (eRoboPlanMode)reader.readElementText().toInt();
        }
        else
        {}
    }

    return 0;
}
