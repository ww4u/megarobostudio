#ifndef COMASSIST_H
#define COMASSIST_H

#include <QtCore>

#include "../../include/mcstd.h"

class comAssist
{

public:
    static QString pureFileName( const QString &fullName, bool containPost = true );
    static QString fileSuffix( const QString &name );
    static bool    fileSuffixMatch( const QString &src,
                                    const QString &suffix );
public:
    comAssist();

};

#endif // COMASSIST_H
