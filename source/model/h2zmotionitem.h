#ifndef H2ZMOTIONITEM_H
#define H2ZMOTIONITEM_H

#include <QtCore>
#include "../../include/mcstd.h"
#include "../intf/iserial.h"
#include "../com/basetype.h"

class H2ZMotionItem : public H2ZRow,
                    public SheetRow,
                    public ISerial
{
public:
    static int columns();
    static QString header( int col );
public:
    H2ZMotionItem();

public:
    virtual int serialOut( ImcStream &stream );
    virtual int serialIn( ImcStream &stream );
};



#endif // H2ZMOTIONITEM_H
