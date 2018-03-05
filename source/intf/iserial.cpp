#include "iserial.h"

ISerial::ISerial()
{
    mVersion = 0;
}

int ISerial::save( const QString &str )
{ return 0; }
int ISerial::load( const QString &str )
{ return 0; }

int ISerial::serialOut( ImcStream &stream )
{ return 0; }
int ISerial::serialIn( ImcStream &stream )
{ return 0; }

int ISerial::serialOut( QXmlStreamWriter &writer )
{ return 0; }
int ISerial::serialIn( QXmlStreamReader &reader )
{ return 0; }
