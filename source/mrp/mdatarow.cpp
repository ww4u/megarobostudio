#include "mdatarow.h"
#include "../../include/mcstd.h"
MDataRow::MDataRow() : QStringList()
{

}

MDataRow::MDataRow( const QStringList &row ) : QStringList()
{ *this = row; }

MDataRow::MDataRow( const QString &row ) : QStringList()
{ *this = row; }

//! normalize to lower
MDataRow &MDataRow::operator=( const QStringList &row )
{
    clear();

    QString localStr;
    for ( int i = 0; i < row.size(); i++ )
    {
        localStr = row.at(i);
        append( localStr.toLower() );
    }

    return *this;
}

MDataRow &MDataRow::operator=( const QString &row )
{
    clear();

    QString localAry;

    QStringList aryList = row.split(',');

    for ( int i = 0;i < aryList.size(); i++ )
    {
        localAry = aryList.at( i );

        append( localAry.toLower() );
    }

    return *this;
}

void MDataRow::dbgShow()
{
    logDbg()<<*this;
}

//! var type
QMap<QString, QVariant::Type > MDataSection::_keyw_types;

MDataSection::MDataSection( int sec ) : mSection( sec )
{
    if ( MDataSection::_keyw_types.size() < 1 )
    {
        //! value
        MDataSection::_keyw_types.insert( "t", QVariant::Double );
        MDataSection::_keyw_types.insert( "p", QVariant::Double );
        MDataSection::_keyw_types.insert( "v", QVariant::Double );

        MDataSection::_keyw_types.insert( "x", QVariant::Double );
        MDataSection::_keyw_types.insert( "y", QVariant::Double );
        MDataSection::_keyw_types.insert( "z", QVariant::Double );
        MDataSection::_keyw_types.insert( "terminal", QVariant::Double );

        MDataSection::_keyw_types.insert( "rx", QVariant::Double );
        MDataSection::_keyw_types.insert( "ry", QVariant::Double );
        MDataSection::_keyw_types.insert( "rz", QVariant::Double );

        //! bool
        MDataSection::_keyw_types.insert( "enable", QVariant::Bool );

        MDataSection::_keyw_types.insert( "comment", QVariant::String );
    }
}

MDataSection::~MDataSection()
{
    delete_all( mRows );
}

void MDataSection::setModel( const QString &model )
{ mModel = model; }
QString MDataSection::model()
{ return mModel; }

void MDataSection::setHeaders( const QStringList &header )
{ mHeaders = header; }
QStringList MDataSection::headers()
{ return mHeaders; }

QString MDataSection::columnName( int c )
{
    if ( c < 0 || c >= mHeaders.size() )
    { return ""; }

    return mHeaders.at( c );
}

void MDataSection::setAttribute( const QString &str )
{
    QString cleanStr = str;

    cleanStr.replace("#!","");
    cleanStr = cleanStr.simplified();

    mAttributes.append( cleanStr );
}
//! name=value
void MDataSection::addAttribute( const QString &str, const QString &name )
{
    QString strAttr = QString("%1=%2").arg( str ).arg( name );
    mAttributes.append( strAttr );
}
QStringList MDataSection::attributes()
{
    return mAttributes;
}

QString MDataSection::getAttribute( const QString &strName )
{
    QString str;
    QStringList pairs;
    for( int i = 0; i < mAttributes.size(); i++ )
    {
        str = mAttributes.at( i );

        pairs = str.split( '=', QString::SkipEmptyParts );
        //! name=value
        if ( pairs.size() == 2 )
        {
            if ( str_is( pairs[0], strName ) )
            { return pairs[1]; }
        }
        else
        { continue; }
    }

    return QStringLiteral("");
}

void MDataSection::setSection( int sec )
{ mSection = sec; }
int MDataSection::section()
{ return mSection; }

bool MDataSection::lineIn( QString &ary )
{
    MDataRow *pRow = new MDataRow( ary );
    if ( NULL == pRow )
    { return false; }

    mRows.append( pRow );
    return true;
}

bool MDataSection::lineOut( QTextStream &stream )
{
    //! section name
    stream<<"[section]"<<line_seperator;

    //! attributes
    if ( mAttributes.size() > 0 )
    {
        for ( int i = 0; i < mAttributes.size(); i++ )
        {
            stream<<attr_header<<mAttributes.at(i)<<line_seperator;
        }
    }

    //! foreach row
    for ( int i = 0; i < mRows.size(); i++ )
    {
        stream<<mRows[i]->join(',')<<line_seperator;
    }

    return true;
}

bool MDataSection::addRow( const QString &str1, QChar sep )
{
    QStringList strList = str1.split( sep );

    MDataRow *pRow = new MDataRow( strList );
    if ( NULL == pRow )
    { return false; }

    mRows.append( pRow );

    return true;
}

int MDataSection::rows()
{ return mRows.size(); }

QVariant MDataSection::cell( int r, int c )
{
    //! invalid cell
    if ( r < 0 || r >= mRows.size()
         || c < 0 || c >= mHeaders.size() )
    { return QVariant(); }

    QString cName = columnName( c );

    if ( MDataSection::_keyw_types.contains( cName ) )
    {
        QVariant var;

        if ( MDataSection::_keyw_types[ cName ] == QVariant::Double )
        { var.setValue( rawCell( r, c ).toDouble() ); }
        else if(  MDataSection::_keyw_types[ cName ] == QVariant::Int )
        { var.setValue( rawCell( r, c ).toInt() ); }
        else if(  MDataSection::_keyw_types[ cName ] == QVariant::Bool )
        { var.setValue( rawCell( r, c ).toInt() > 0 ); }
        else if(  MDataSection::_keyw_types[ cName ] == QVariant::String )
        { var.setValue( rawCell( r, c )  ); }
        else
        { }

        return var;
    }
    else
    {
        QVariant var;
        var.setValue( rawCell( r, c) );

        return var;
    }
}

QString MDataSection::rawCell( int r, int c )
{
    return mRows.at(r)->at( c );
}

bool MDataSection::cellValue( int r, int c,
                bool &v, bool def,
                bool bOv )
{
    QVariant var = cell( r, c );

    bool ok;
    ok = var.isValid() && !var.toString().isEmpty();

    if ( ok  )
    { v = var.toBool(); }
    else if ( bOv )
    { v = def; }
    else
    {}

    return ok;
}

bool MDataSection::cellValue( int r, int c,
                float &v, float def,
                bool bOv )
{
    QVariant var = cell( r, c );
    float localV;

    bool ok;
    localV = var.toFloat( &ok );
    if ( ok )
    { v = localV; }
    else if ( bOv )
    { v = def; }
    else
    {}

    return ok;
}
bool MDataSection::cellValue( int r, int c,
                double &v, double def,
                bool bOv )
{
    QVariant var = cell( r, c );
    double localV;

    bool ok;
    localV = var.toDouble( &ok );
    if ( ok )
    { v = localV; }
    else if ( bOv )
    { v = def; }
    else
    {}

    return ok;
}
bool MDataSection::cellValue( int r, int c,
                int &v, int def,
                bool bOv )
{
    QVariant var = cell( r, c );
    int localV;

    bool ok;
    localV = var.toInt( &ok );
    if ( ok )
    { v = localV; }
    else if ( bOv )
    { v = def; }
    else
    {}

    return ok;
}
bool MDataSection::cellValue( int r, int c,
                QString &v, const QString &def,
                bool bOv )
{
    QVariant var = cell( r, c );
    QString localV;

    bool ok;

    localV   = var.toString();
    ok = !localV.isEmpty();
    if ( ok )
    { v = localV; }
    else if ( bOv )
    { v = def; }
    else
    {}

    return ok;
}

void MDataSection::dbgShow()
{
    logDbg()<<"section"<<mSection;
    foreach( MDataRow *pRow, mRows )
    {
        pRow->dbgShow();

    }
}
