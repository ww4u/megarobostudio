#ifndef TPITEM_H
#define TPITEM_H

#include <QtCore>
#include "../../include/mcstd.h"
#include "../intf/iserial.h"

#include "../com/basetype.h"

class TpItem : public TpRow, public ISerial
{
public:
    static int columns();
    static QString header( int col );

public:
    TpItem( tpvType t= 0, tpvType p = 0 );
    TpItem( const TpItem &item );

    virtual ~TpItem()
    {}

public:
    int serialOut(ImcStream &stream);
    int serialIn( ImcStream &stream );

public:
    void set( tpvType t, tpvType p );

    void setT( tpvType t );
    void setP( tpvType p );

    tpvType getT();
    tpvType getP();
};

#endif // TPITEM_H
