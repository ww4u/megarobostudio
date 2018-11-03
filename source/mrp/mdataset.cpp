#include "mdataset.h"
#include "../../include/mcstd.h"

MDataSet::MDataSet()
{
    m_pNowSection = NULL;
}

MDataSet::~MDataSet()
{
    delete_all( mSections );
}

bool MDataSet::isEmpty()
{
    if ( mSections.size() < 1 )
    { return true; }

    Q_ASSERT( NULL != mSections.at(0) );
    if ( mSections.at(0)->rows() < 1 )
    { return true; }

    return false;
}

void MDataSet::setModel( const QString &m )
{ mModel = m; }

QString MDataSet::model()
{ return mModel; }

void MDataSet::setHeaders( const QStringList &strs )
{ mHeaders = strs; }
QStringList MDataSet::headers()
{ return mHeaders; }

QString MDataSet::header( int c )
{
    if ( c < 0 || c > mHeaders.size() )
    { return ""; }
    else
    { return mHeaders.at(c);}
}

int MDataSet::sections()
{
    return mSections.size();
}

int MDataSet::columns()
{
    return mHeaders.size();
}

bool MDataSet::verifyHeader( const QStringList &header )
{
    QString colName;
    for( int i = 0; i < header.size(); i++ )
    {
        colName = header.at( i );

        colName = colName.toLower();
        colName = colName.simplified();

        if ( mHeaders.contains(colName) )
        { }
        else
        { return false; }
    }

    return true;
}

bool MDataSet::verifyHeader( const QString &str1 )
{
    QStringList strList;
    strList<<str1;

    return verifyHeader( strList );
}
bool MDataSet::verifyHeader( const QString &str1,
                   const QString &str2 )
{
    QStringList strList;
    strList<<str1<<str2;

    return verifyHeader( strList );
}
bool MDataSet::verifyHeader( const QString &str1,
                   const QString &str2,
                   const QString &str3
                   )
{
    QStringList strList;
    strList<<str1<<str2<<str3;

    return verifyHeader( strList );
}
bool MDataSet::verifyHeader( const QString &str1,
                   const QString &str2,
                   const QString &str3,
                   const QString &str4
                   )
{
    QStringList strList;
    strList<<str1<<str2<<str3<<str4;

    return verifyHeader( strList );
}

int MDataSet::columnIndex( const QString &name, int from )
{
    //! lower case
    QString normName;

    normName = name.toLower();

    return mHeaders.indexOf( normName, from );
}

QString MDataSet::columnName( int index )
{
    if ( index < 0 || index > mHeaders.size() )
    { return ""; }
    else
    { return mHeaders.at(index); }
}

MDataSection * MDataSet::addSection( )
{
    MDataSection *pSec = new MDataSection( mSections.size() );
    if ( NULL == pSec )
    { return NULL; }

    pSec->setModel( mModel );
    pSec->setHeaders( mHeaders );
    mSections.append( pSec );

    return pSec;
}

MDataSection * MDataSet::section( int id )
{
    if ( id < 0 || id >= mSections.size() )
    { return NULL; }

    return mSections.at( id );
}

int MDataSet::load( const QString &fullname )
{
    //! 1. open
    QFile file( fullname );
    if ( file.open(QIODevice::ReadOnly) )
    { }
    else
    { return -1; }

    //! 2. load
    int ret;
    ret = doLoad( file );

    //! 3. close
    file.close();

    return ret;
}

int MDataSet::save( const QString &fullName )
{
    QFile file( fullName );
    if ( file.open( QIODevice::WriteOnly ) )
    { }
    else
    { return -1; }

    int ret = doSave( file );

    file.close();

    return ret;
}

MDataSection* MDataSet::tryLoad( const QString &fullName,
                     const QString &modelStr,
                     const QStringList &headers )
{
    int ret = load( fullName );
    if ( ret != 0 )
    { return NULL; }

    //! check model
    if ( modelStr.isEmpty() )
    { }
    else if ( !modelStr.isEmpty() && str_is( modelStr, model()) )
    { }
    else
    { return NULL; }

    //! check headers
    if ( verifyHeader( headers ) )
    {}
    else
    { return NULL; }

    //! empty?
    if ( isEmpty() )
    { return NULL; }

    return mSections.at( 0 );
}

void MDataSet::dbgShow()
{
    logDbg()<<mModel;
    logDbg()<<mHeaders;
    logDbg()<<"sections:"<<mSections.size();
    foreach( MDataSection *pSec, mSections )
    {
        pSec->dbgShow();
    }
}

int MDataSet::doLoad( QFile &file )
{
    //! \note utf-8
    QTextStream stream( &file );

    //! load the content
    QString rawLine;
    QString varLine;
    QStringList dataLine;
    while( !stream.atEnd() )
    {
        rawLine = stream.readLine();
        varLine = rawLine;

        //! filter
        if ( filterLine(varLine) )
        { continue; }

        //! description
        if ( varLine.startsWith( "[") )
        {
            extractDescription( varLine );

            do
            {
                //! model
                if ( mModel.isEmpty() )
                {
                    mModel = varLine;
                    break;
                }

                //! header
                if ( mHeaders.isEmpty() )
                {
                    mHeaders = extractStringList( varLine );
                    break;
                }

                //! a new section
                if ( str_is( varLine, "section" ) )
                {
                    do
                    {
                        if ( m_pNowSection != NULL )
                        { m_pNowSection = new MDataSection( m_pNowSection->section() + 1 ); }
                        else
                        { m_pNowSection = new MDataSection( ); }

                        if ( NULL == m_pNowSection )
                        { return -1; }

                        m_pNowSection->setModel( mModel );
                        m_pNowSection->setHeaders( mHeaders );
                        mSections.append( m_pNowSection );
                    }while ( 0 );

                    break;
                }

            }while( 0 );

        }
        //! data
        else
        {
            //! data set
            dataLine = extractStringList( varLine );
            if ( dataLine.size() == mHeaders.size() )
            {
                //! current section
                do
                {
                    if ( m_pNowSection != NULL )
                    { break; }

                    m_pNowSection = new MDataSection();
                    if ( NULL == m_pNowSection )
                    { return -1; }

                    m_pNowSection->setModel( mModel );
                    m_pNowSection->setHeaders( mHeaders );
                    mSections.append( m_pNowSection );
                }while ( 0 );

                if ( m_pNowSection->lineIn( varLine ) )
                {}
                else
                { return -1; }
            }
        }
    }

    return 0;
}

int MDataSet::doSave( QFile &file )
{
    QTextStream stream( &file );

    stream<<"["<<mModel<<"]"<<line_seperator;
    stream<<"["<<mHeaders.join(',')<<"]"<<line_seperator;

    for ( int i = 0; i < mSections.size(); i++ )
    {
        if ( mSections[i]->lineOut( stream ) )
        {}
        else
        { return -1; }
    }

    return 0;
}

bool MDataSet::verifyLine( QString &ary )
{
    if ( ary.isEmpty())
    { return false; }

    return true;
}

bool MDataSet::commentLine( QString &ary )
{
    if ( ary.startsWith('#') )
    { return true; }

    return false;
}

void MDataSet::normalLine( QString &ary )
{
    ary = ary.simplified();

    //! lower
    ary = ary.toLower();
}

bool MDataSet::filterLine( QString &ary )
{
    normalLine( ary );

    if ( verifyLine(ary) )
    { }
    else
    { return true; }

    if ( commentLine(ary) )
    { return true; }

    return false;
}

void MDataSet::extractDescription( QString &ary )
{
    ary = ary.replace( '[', ' ' );
    ary = ary.replace( ']', ' ' );

    ary = ary.simplified();
}

QStringList MDataSet::extractStringList( QString &ary )
{
    QStringList aryList = ary.split(',');

    QStringList strList;

    QString localAry;
//    foreach( QByteArray &ary, aryList )
    for ( int i = 0; i < aryList.size(); i++ )
    {
        localAry = aryList.at(i);
        strList << localAry.simplified();
    }

    return strList;
}

