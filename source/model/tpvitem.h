#ifndef TPVITEM_H
#define TPVITEM_H

#include <QtCore>
#include "../../include/mcstd.h"
#include "../intf/iserial.h"

#include "../com/basetype.h"

class tpvItem : public tpvRow, public ISerial
{
public:
    static int columns();
    static QString header( int col );

public:
    tpvItem( tpvType t=0, tpvType p=0, tpvType v = 0 );
    tpvItem( const tpvItem &item );

public:
    int serialOut(ImcStream &stream);
    int serialIn_tpv( ImcStream &stream );
    int serialIn_pvt( ImcStream &stream );
public:
    void setValid( bool b );
    bool getValid();

    void setComment( const QString & );
    QString getComment();

    void set( tpvType t, tpvType p, tpvType v );

    void setT( tpvType t );
    void setP( tpvType p );
    void setV( tpvType v );

    tpvType getT();
    tpvType getP();
    tpvType getV();

public:
    int mValid;         //! valid

    QString mComment;   //! comment
};

#endif // TPVITEM_H
