#include "link.h"

Link::Link( const QString &objName ): Entity(objName)
{
    m_pFrom = NULL;
    m_pTo = NULL;

    mClass = "link";

    postCtor();
}
