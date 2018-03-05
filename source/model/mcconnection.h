#ifndef MCCONNECTION_H
#define MCCONNECTION_H

#include <QtCore>

#include "../../device/vrobot.h"

class mcConnection
{
public:
    mcConnection();
    ~mcConnection();

public:
    void setRoboName( const QString &str );
    QString getRoboName();

    void setDevice( const QString &str, int ch );

    void setDeviceName( const QString &str );
    void setDeviceCH( int ch );

    QString getDeviceName();
    int getDeviceCH();

//    void setRobot( VRobot *pRobo );
//    VRobot *getRobot();
//    VRobot *robot();

public:
    QString mRoboName;
    QString mDeviceName;
    int     mAxesId;

//    VRobot  *m_pRobot;
};

#endif // MCCONNECTION_H
