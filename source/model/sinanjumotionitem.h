#ifndef SINANJU_MOTIONITEM_H
#define SINANJU_MOTIONITEM_H

#include <QtCore>
#include "../../include/mcstd.h"
#include "../intf/iserial.h"
#include "../com/basetype.h"

class SinanjuMotionItem : public SinanjuRow,
                          public SheetRow,
                          public ISerial
{

public:
    static int columns();
    static QString header( int col );



public:
    SinanjuMotionItem();
    SinanjuMotionItem( const SinanjuMotionItem &item );

    virtual ~SinanjuMotionItem()
    {}

    SinanjuMotionItem &operator=( const SinanjuMotionItem &item );

public:
    virtual int serialOut( ImcStream &stream );
    virtual int serialIn( ImcStream &stream );

public:

};

#endif // MOTIONITEM_H

