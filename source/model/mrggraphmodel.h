#ifndef MRGGRAPHMODEL_H
#define MRGGRAPHMODEL_H

#include "mcmodelobj.h"
//#include "../../source/mrg/entity.h"

//#include "../../source/mrg/robographmodel.h"

#include "../widget/entitywidget.h"
class RoboGraph;
class MrgGraphModel : public mcModelObj
{
public:
    MrgGraphModel();

public:
    int load( const QString &name, RoboGraph *parent );
    int save( const QString &name );

    void clean();

protected:
    int doLoad( QXmlStreamReader &reader,
                RoboGraph *parent );
    int crossLink( );

public:
    QList<EntityWidget*> mChildWidgets;

};

#endif // MRGGRAPHMODEL_H
