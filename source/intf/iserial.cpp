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

bool ISerial::toBool( QXmlStreamReader &reader )
{  return reader.readElementText().toInt() > 0; }
int ISerial::toInt( QXmlStreamReader &reader )
{ return reader.readElementText().toInt(); }
float ISerial::toFloat( QXmlStreamReader &reader )
{ return reader.readElementText().toFloat(); }
double ISerial::toDouble( QXmlStreamReader &reader )
{ return reader.readElementText().toDouble(); }
