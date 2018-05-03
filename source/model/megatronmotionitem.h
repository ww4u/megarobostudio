#ifndef MEGATRONMOTIONITEM_H
#define MEGATRONMOTIONITEM_H

#include <QtCore>
#include "../../include/mcstd.h"
#include "../intf/iserial.h"
#include "../com/basetype.h"

class MegatronMotionItem : public MegatronRow,
                           public SheetRow,
                           public ISerial
{
public:
    static int columns();
    static QString header( int col );

public:
    MegatronMotionItem();
    MegatronMotionItem( const MegatronMotionItem &item );

    virtual ~MegatronMotionItem()
    {}

    MegatronMotionItem &operator=( const MegatronMotionItem &item );

public:
    virtual int serialOut( ImcStream &stream );
    virtual int serialIn( ImcStream &stream );

};

#endif // MEGATRONMOTIONITEM_H
