#include "modelrobotmgr.h"

#include "../../include/mcstd.h"


modelRobotMgr::modelRobotMgr()
{
    mVersion = 0;
}

int modelRobotMgr::save( const QString &str )
{
    return 0;
}
int modelRobotMgr::load( const QString &str )
{
    //! check ver
    QFile fileIn(str);
    if ( !fileIn.open( QIODevice::ReadOnly) )
    { return ERR_FILE_OPEN_FAIL; }

    QXmlStreamReader reader( &fileIn );

    //! start element
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "robots" )
        {
            while( reader.readNextStartElement() )
            {
                if ( reader.name() == "robot" )
                {
                    mRobots.append( reader.readElementText() );
                }
            }
        }
    }

    return 0;
}
