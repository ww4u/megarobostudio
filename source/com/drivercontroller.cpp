#include "drivercontroller.h"

driverController::driverController()
{
}

QString driverController::getName()
{ return mName; }
QString driverController::getClass()
{ return mClass; }
int driverController::getCHs()
{ return mChs; }
int driverController::getMotor()
{ return mMotor; }
QString driverController::getVersion()
{ return mVersion; }
int driverController::getCurrent()
{ return mCurrent; }

int driverController::read( QXmlStreamReader &reader )
{
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "name" )
        { mName = reader.readElementText(); }

        else if ( reader.name() == "class" )
        { mClass = reader.readElementText(); }

        else if ( reader.name() == "ch" )
        { mChs = reader.readElementText().toInt(); }

        else if ( reader.name() == "version" )
        { mVersion = reader.readElementText(); }

        else if ( reader.name() == "current" )
        { mCurrent = reader.readElementText().toFloat(); }

        else if ( reader.name() == "motor" )
        { mMotor = reader.readElementText().toInt(); }

        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

deviceDb::deviceDb()
{
}

deviceDb::~deviceDb()
{
    delete_all( mDevices );
}

int deviceDb::load( const QString &fileName )
{
    QFile file(fileName);

    if ( !file.open( QIODevice::ReadOnly) )
    { return ERR_FILE_OPEN_FAIL; }

    delete_all( mDevices );

    QXmlStreamReader reader( &file );
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "root" )
        {
            while( reader.readNextStartElement() )
            {
                if ( reader.name() == "device" )
                {
                    driverController *pController = new driverController();
                    if ( pController->read(reader) == 0 )
                    {
                        mDevices.append( pController );
                    }
                    else
                    { delete pController; }
                }
                else
                { reader.skipCurrentElement(); }
            }
        }
        else
        { reader.skipCurrentElement(); }
    }

    file.close();

    return 0;
}
driverController * deviceDb::find( const QString &name )
{
    foreach (driverController* pDevice, mDevices)
    {
        Q_ASSERT( NULL != pDevice );

        if ( pDevice->getName() == name )
        { return pDevice; }
    }

    return NULL;
}

QString deviceDb::findClass( const QString &desc )
{
    driverController *pDriver;

    //! default class
    pDriver = find( desc );
    if ( NULL == pDriver )
    { return QString("quebeley"); }
    else
    { return pDriver->getClass(); }
}
