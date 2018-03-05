#include "mcconnection.h"

mcConnection::mcConnection()
{
    mAxesId = 0;

//    m_pRobot = NULL;
}
mcConnection::~mcConnection()
{
//    if ( NULL != m_pRobot )
//    {
//        delete m_pRobot;
//        m_pRobot = NULL;
//    }
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

//void mcConnection::setRobot( VRobot *pRobo )
//{
//    //! \note can be null

//    //! gc
//    if ( m_pRobot != pRobo )
//    {
//        if ( NULL != m_pRobot )
//        {
//            delete m_pRobot;
//            m_pRobot = NULL;
//        }
//    }

//    m_pRobot = pRobo;
//}
//VRobot *mcConnection::getRobot()
//{
//    return m_pRobot;
//}
//VRobot *mcConnection::robot()
//{ return m_pRobot; }
