#include "mdatarow.h"
#include "../../include/mcstd.h"
MDataRow::MDataRow() : QStringList()
{

}

MDataRow::MDataRow( const QStringList &row ) : QStringList()
{ *this = row; }

MDataRow::MDataRow( const QByteArray &row ) : QStringList()
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

MDataRow &MDataRow::operator=( const QByteArray &row )
{
    clear();

    QByteArray localAry;

    QList<QByteArray> aryList = row.split(',');

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
        MDataSection::_keyw_types.insert( "h", QVariant::Double );

        MDataSection::_keyw_types.insert( "rx", QVariant::Double );
        MDataSection::_keyw_types.insert( "ry", QVariant::Double );
        MDataSection::_keyw_types.insert( "rz", QVariant::Double );

        //! bool
        MDataSection::_keyw_types.insert( "enable", QVariant::Bool );
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

void MDataSection::setSection( int sec )
{ mSection = sec; }
int MDataSection::section()
{ return mSection; }

bool MDataSection::lineIn( QByteArray &ary )
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

QVariant MDataSection::cell( int r, int c )
{
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
        else
        { }

        return var;
    }
    else
    {
        QVariant var( QVariant::String );
        var.setValue( rawCell( r, c) );

        return var;
    }
}

QString MDataSection::rawCell( int r, int c )
{
    return mRows.at(r)->at( c );
}

void MDataSection::dbgShow()
{
    logDbg()<<"section"<<mSection;
    foreach( MDataRow *pRow, mRows )
    {
        pRow->dbgShow();

    }
}
