#include "comassist.h"

QStringList comAssist::_mRemotePath;

void comAssist::setRemotePath( const QStringList &path )
{ _mRemotePath = path; }
QStringList &comAssist::remotePath()
{ return _mRemotePath; }

#define norm_rem( n, vin, base, remain )   n = (vin) / (base);\
                                           remain = (vin) - (n) * (base)
QString comAssist::msToHmsz( qint64 ms )
{
    qint64 h, m, s;
    qint64 r;

    norm_rem( h, ms, 3600*1000, r );
    norm_rem( m, r, 60*1000, r );
    norm_rem( s, r, 1000, r );

    return QString( "%1:%2:%3.%4").arg(h).arg(m).arg(s).arg(r);
}

QString comAssist::pureFileName( const QString &fileName,
                                 bool bContainPost )
{
    QString fullName;
    fullName = fileName;
    int sep = fullName.lastIndexOf( QDir::separator() );

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

int comAssist::align( double val, double unit )
{
    int aligned;

    if ( val > 0 )
    {
        aligned = ( (val + unit/10) / unit );
    }
    else
    {
        aligned = ( ( val - unit/10) / unit );
    }

    return aligned;
}

float comAssist::normalizeDegree360( float degree )
{
    while( degree >= 360 )
    { degree -= 360; }
    while( degree < 0 )
    { degree += 360; }

    return degree;
}

float comAssist::normalizeDegreeN360( float degree )
{
    while( degree > 0 )
    { degree -= 360; }
    while( degree <= -360 )
    { degree += 360; }

    return degree;
}

float comAssist::eulcidenDistance( float x1, float y1, float z1,
                                 float x2, float y2, float z2 )
{
    return qSqrt( pow(x1-x2,2 ) + pow( y1-y2, 2 ) + pow(z1-z2, 2 ) );
}

bool comAssist::convertDataset( const QStringList &line,
                               float *pData,
                               int cols )
{
    Q_ASSERT( NULL != pData );

    if ( line.size() < cols )
    { return false; }

    bool bOk;
    for ( int i = 0; i < cols; i++ )
    {
        pData[i] = line.at(i).toFloat( &bOk );
        if ( !bOk )
        { logDbg()<<line;return false; }
    }

    return true;
}

bool    comAssist::ammendFileName( QString &fileName )
{
    //! file file in each dir
    do
    {
        if (  QFile::exists( fileName ) )
        { return true; }

        //! for each path
        QString fullName;
        foreach( QString str, comAssist::_mRemotePath )
        {
            fullName = str + QDir::separator() + fileName;
            if ( QFile::exists(fullName) )
            {
                fileName = fullName;
                return true;
            }
        }

    }while(0);

    return false;
}

int comAssist::loadDataset( const char *pFileName,
                            int nameLen,
                            int col,
                            QList<float> &dataset,
                            const char &colSep,
                            const char &lineSep )
{
    Q_ASSERT( pFileName != NULL  );
    QByteArray fileName( pFileName, nameLen );

    return loadDataset( fileName, col, dataset, colSep, lineSep );
}

int comAssist::loadDataset( const QString &fileName,
                            int col,
                            QList<float> &dataset,
                            const char &colSep,
                            const char &lineSep )
{
    bool bExist;

    QString realFileName = fileName;
    bExist = ammendFileName( realFileName );
    if ( bExist )
    {}
    else
    { return ERR_FILE_OPEN_FAIL; }

    QFile file(realFileName);
    if ( !file.open(QIODevice::ReadOnly) )
    { return ERR_FILE_OPEN_FAIL; }
logDbg()<<realFileName;
    //! read all
    QByteArray ary = file.readAll();
    QList<QByteArray> lines = ary.split( lineSep );

    //! convert for each line
    QString lineStr;
    QStringList lineArgs;

    float lineDatas[ col ];
    for ( int i = 0; i < lines.size(); i++ )
    {
        lineStr.clear();
        lineStr.append( lines[i] );
        lineArgs = lineStr.split( colSep, QString::SkipEmptyParts );
        if ( lineArgs.size() >= col )
        {
            if ( convertDataset( lineArgs, lineDatas, col ) )
            {
                for ( int j = 0; j < col; j++ )
                {
                    dataset.append( lineDatas[j] );
                }
            }
        }
        else
        {}
    }

    return 0;
}


comAssist::comAssist()
{

}



