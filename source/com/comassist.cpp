#include "comassist.h"

QString comAssist::pureFileName( const QString &fileName,
                                 bool bContainPost )
{
    QString fullName;
    fullName = fileName;
    int sep = fullName.lastIndexOf('/');

    QString pureName;
    if ( sep < 0 )
    { pureName = fileName; }
    else
    { pureName = fullName.right( fileName.length() - sep - 1 ); }

    //! remove the dot
    if ( !bContainPost )
    {
        int sep = pureName.lastIndexOf('.');
        if ( sep < 0 )
        {}
        else
        { pureName = pureName.left(sep); }
    }

    return pureName;
}

QString comAssist::fileSuffix( const QString &fileName )
{
    QString fullName;
    fullName = fileName;
    int sep = fullName.lastIndexOf('.');

    //! none
    if ( sep < 0 )
    { return QString(""); }
    else
    { return fileName.right( fileName.length() - sep - 1 ); }
}

bool comAssist::fileSuffixMatch( const QString &src,
                                 const QString &suffix )
{
    return src.compare( suffix, Qt::CaseInsensitive ) == 0;
}

comAssist::comAssist()
{

}
