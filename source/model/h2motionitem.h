#ifndef H2MOTIONITEM_H
#define H2MOTIONITEM_H

#include <QtCore>
#include "../../include/mcstd.h"
#include "../intf/iserial.h"
#include "../com/basetype.h"

class H2MotionItem : public H2Row,
        public SheetRow,
        public ISerial
{
public:
    static int columns();
    static QString header( int col );
public:
    H2MotionItem();

    H2MotionItem &operator=( const H2MotionItem &item );
public:
    virtual int serialOut( ImcStream &stream );
    virtual int serialIn( ImcStream &stream );

};

#endif // H2MOTIONITEM_H
