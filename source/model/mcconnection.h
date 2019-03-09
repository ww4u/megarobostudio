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
//    void setRoboName( const QString &str );
//    QString getRoboName();

//    void setRoboPage( int page );
//    int roboPage();

//    void setDevice( const QString &str, int ch );

//    void setDeviceName( const QString &str );
//    void setDeviceCH( int ch );

//    QString getDeviceName();
//    int getDeviceCH();

//    void setDevicePage( int page );
//    int devicePage();

    void setDeviceMap( const QMap<QString, int> &map );
    QMap<QString, int> &deviceMap();

    void setRobots( const QStringList &robos );
    QStringList &robots();

public:
//    QString mRoboName;
//    int     mRoboPage;

//    QString mDeviceName;
//    int     mAxesId;
//    int     mDevicePage;

    QMap<QString, int> mDeviceMap;
    QStringList mRobos;

};

#endif // MCCONNECTION_H
