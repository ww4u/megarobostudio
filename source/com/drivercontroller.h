#ifndef DRIVERCONTROLLER_H
#define DRIVERCONTROLLER_H

#include <QtCore>
#include "../../include/mcstd.h"

class driverController
{
public:
    driverController();

public:
    QString getName();
    QString getClass();
    int getCHs();
    int getMotor();
    QString getVersion();
    int getCurrent();

    int read( QXmlStreamReader &reader );

protected:
    QString mName;
    QString mClass;
    int mChs;
    int mMotor;
    QString mVersion;
    float mCurrent;
};

class deviceDb
{
public:
    deviceDb();
    ~deviceDb();

public:
    int load( const QString &fileName );
    driverController * find( const QString &name );
    QString findClass( const QString &desc );

public:
    QList<driverController *> mDevices;
};

#endif // DRIVERCONTROLLER_H
