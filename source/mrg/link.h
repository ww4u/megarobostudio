#ifndef LINK_H
#define LINK_H

#include "entity.h"

class Link : public Entity
{
public:
    Link( const QString &objName="" );

protected:
    Entity *m_pFrom, *m_pTo;
};

#endif // LINK_H
