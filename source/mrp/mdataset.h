#ifndef MDATASET_H
#define MDATASET_H

#include <QtCore>

#include "mdatarow.h"

class MDataSet
{
public:
    MDataSet();
    ~MDataSet();

public:
    QString model();
    QStringList headers();
    QString header( int c );

    int sections();
    int columns();

    int columnIndex( const QString &name, int from=0 );
    QString columnName( int index );

    MDataSection * section( int id );

public:
    int load( const QString &fullname );
    int save( const QString &fullName );

    void dbgShow();

protected:
    int doLoad( QFile &file );
    int doSave( QFile &file );

    bool verifyLine( QByteArray &ary );
    bool commentLine( QByteArray &ary );
    void normalLine( QByteArray &ary );

    bool filterLine( QByteArray &ary );

    void extractDescription( QByteArray &ary );
    QStringList extractStringList( QByteArray &ary );

public:
    QString mModel;

    QStringList mHeaders;

protected:
    MDataSection *m_pNowSection;
    QList< MDataSection * > mSections;

};

#endif // MDATASET_H
