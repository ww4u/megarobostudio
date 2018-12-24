#include "mrxt4.h"

MrxT4::MrxT4( const QString &objName ):Robot(objName)
{
    mClass = __FILE__;

    postCtor();
}
