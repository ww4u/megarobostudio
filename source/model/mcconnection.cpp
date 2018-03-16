#include "mcconnection.h"

mcConnection::mcConnection()
{
    mAxesId = 0;
}
mcConnection::~mcConnection()
{
}

void mcConnection::setRoboName( const QString &str )
{ mRoboName = str; }
QString mcConnection::getRoboName()
{ return mRoboName; }

void mcConnection::setDevice( const QString &str, int ch )
{
    mDeviceName = str;
    mAxesId = ch;
}
void mcConnection::setDeviceName( const QString &str )
{
    mDeviceName = str;
}
void mcConnection::setDeviceCH( int ch )
{
    mAxesId = ch;
}

QString mcConnection::getDeviceName()
{ return mDeviceName; }
int mcConnection::getDeviceCH()
{ return mAxesId; }

