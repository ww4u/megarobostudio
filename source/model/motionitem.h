#ifndef MOTIONITEM_H
#define MOTIONITEM_H

#include <QtCore>
#include "../../include/mcstd.h"
#include "../intf/iserial.h"
#include "../com/basetype.h"

class motionItem : public mcRow, public ISerial
{

public:
    static int columns();
    static QString header( int col );

public:
    motionItem();
    motionItem( const motionItem &item );

    virtual ~motionItem()
    {}

    motionItem &operator=( const motionItem &item );

public:
    virtual int serialOut( ImcStream &stream );
    virtual int serialIn( ImcStream &stream );

public:
    void setBreak( bool b );
    bool getBreak();

    void setItem( const QString &cmd, tpvType x = 0, tpvType y = 0, tpvType z = 0, tpvType t = 0);
    void setCmd( const QString &cmd );
    void setX( tpvType x );
    void setY( tpvType y );
    void setZ( tpvType z );
    void setT( tpvType t );

    void setPara( tpvType x, tpvType y, tpvType z, tpvType t );
    void setComment( const QString &comment );

    QString getCmd();
    tpvType getX();
    tpvType getY();
    tpvType getZ();
    tpvType getT();
    QString getComment();

public:
    bool mBreak;
    QString mCmd;       //! cmd

    QString mComment;
};

#endif // MOTIONITEM_H
