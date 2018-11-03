#ifndef MDATASET_H
#define MDATASET_H

#include <QtCore>

#include "mdatarow.h"

#define deparse_column_index( name, n )    int c_##name = dataSet.columnIndex( n );
#define deparse_column_index_( name, sep )    int c_##name = dataSet.columnIndex( "name" );
#define deparse_column_index__( name )    deparse_column_index_( name, n )
#define headerlist( headers )              QString( headers ).split('/')

class MDataSet
{
public:
    MDataSet();
    ~MDataSet();

public:
    bool isEmpty();

    void setModel( const QString &m );
    QString model();

    void setHeaders( const QStringList &strs );
    QStringList headers();
    QString header( int c );

    int sections();
    int columns();

    bool verifyHeader( const QStringList &header );

    bool verifyHeader( const QString &str1 );
    bool verifyHeader( const QString &str1,
                       const QString &str2 );
    bool verifyHeader( const QString &str1,
                       const QString &str2,
                       const QString &str3
                       );
    bool verifyHeader( const QString &str1,
                       const QString &str2,
                       const QString &str3,
                       const QString &str4
                       );

    int columnIndex( const QString &name, int from=0 );
    QString columnName( int index );

    MDataSection * addSection( );
    MDataSection * section( int id );

public:
    int load( const QString &fullname );
    int save( const QString &fullName );

    MDataSection * tryLoad( const QString &fullName,
                 const QString &model,
                 const QStringList &headers );

    void dbgShow();

protected:
    int doLoad( QFile &file );
    int doSave( QFile &file );

    bool verifyLine( QString &ary );
    bool commentLine( QString &ary );
    void normalLine( QString &ary );

    bool filterLine( QString &ary );

    void extractDescription( QString &ary );
    QStringList extractStringList( QString &ary );

public:
    QString mModel;

    QStringList mHeaders;

protected:
    MDataSection *m_pNowSection;
    QList< MDataSection * > mSections;

};

#endif // MDATASET_H
