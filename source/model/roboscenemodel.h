#ifndef ROBOSCENEMODEL_H
#define ROBOSCENEMODEL_H

#include <QtCore>
#include "../../include/mcstd.h"

#include "mcmodelobj.h"

class sceneModel
{
public:
    sceneModel();

public:
    QString mClass;
    QString mName;
    int mX, mY, mW, mH;
};

class roboSceneModel : public mcModelObj
{
public:
    roboSceneModel();
    ~roboSceneModel();

public:
    int load( const QString &name );
    int save( const QString &name );

public:
    QList< sceneModel *> mSceneItems;
};

#endif // ROBOSCENEMODEL_H
