#ifndef ISERIAL_H
#define ISERIAL_H

#include <QtCore>

#include "imcstream.h"

class ISerial
{
public:
    ISerial();

public:
    virtual int save( const QString &str );
    virtual int load( const QString &str );

    //! item serial out/in
    virtual int serialOut( ImcStream &stream );
    virtual int serialIn( ImcStream &stream );

    virtual int serialOut( QXmlStreamWriter &writer );
    virtual int serialIn( QXmlStreamReader &reader );

protected:
    int mVersion;

};

#define COL_SEP     ","
#define ROW_SEP     endl

#endif // ISERIAL_H
