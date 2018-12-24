#ifndef MRGGRAPHMODEL_H
#define MRGGRAPHMODEL_H

#include "mcmodelobj.h"
//#include "../../source/mrg/entity.h"

#include "../../source/mrg/robographmodel.h"

class MrgGraphModel : public mcModelObj
{
public:
    MrgGraphModel();

public:
    int load( const QString &name );
    int save( const QString &name );

protected:
    RoboGraphModel mModel;
};

#endif // MRGGRAPHMODEL_H
