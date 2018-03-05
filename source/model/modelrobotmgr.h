#ifndef MODELROBOTMGR_H
#define MODELROBOTMGR_H

#include "../../intf/iserial.h"

class modelRobotMgr : public ISerial
{
public:
    modelRobotMgr();

public:
    virtual int save( const QString &str );
    virtual int load( const QString &str );

public:
    QStringList mRobots;


};

#endif // MODELROBOTMGR_H
