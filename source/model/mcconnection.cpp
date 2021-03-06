#include "mcconnection.h"

mcConnection::mcConnection()
{
    mRoboPage = 0;      //! \note default to MAIN

    mAxesId = 0;
    mDevicePage = 0;
}
mcConnection::~mcConnection()
{
}

void mcConnection::setRoboName( const QString &str )
{ mRoboName = str; }
QString mcConnection::getRoboName()
{ return mRoboName; }

void mcConnection::setRoboPage( int page )
{ mRoboPage = page; }
int mcConnection::roboPage()
{ return mRoboPage; }

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

void mcConnection::setDevicePage( int page )
{ mDevicePage = page; }
int mcConnection::devicePage()
{ return mDevicePage; }

