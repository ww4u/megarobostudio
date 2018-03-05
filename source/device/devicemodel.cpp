#include "devicemodel.h"


const char* enum_toString( int val, value_desc pDescs[], int32 cnt )
{
    for( int i = 0; i < cnt; i++ )
    {
        if ( pDescs[i].val == val )
        { return (pDescs[i].desc ); }
    }

    return ("UNK");
}

//! 0 -- no error
int enum_toValue( const char* pStr, value_desc pDescs[], int cnt, int *pVal )
{
    //! over long
    char trimStr[ 32 ];
    if ( strlen(pStr) > sizeof(trimStr) - 1 )
    { return -1; }

    strcpy( trimStr, pStr );

    //! remove the space
    char space[]={' ', '\r', '\n' };
    for ( int i = strlen(trimStr)-1; i >= 0; i-- )
    {
        for ( int j = 0; j < sizeof(space); j++ )
        {
            if ( trimStr[i] == space[j] )
            {
                trimStr[i] = '\0';
                break;
            }
        }
    }

    for( int i = 0; i < cnt; i++ )
    {
        //! case insensitive
        if ( strcasecmp( pDescs[i].desc, trimStr) == 0 )
        {
            *pVal = pDescs[i].val;
            return 0;
        }
    }

    return -1;
}

namespace MegaDevice {

DeviceModel::DeviceModel()
{

}


//void DeviceModel::setAlias( const QString &str, int id )
//{
//    //! fill the pad
//    for( int i = mAlias.length(); i < id_to_index(id) + 1; i++ )
//    { mAlias.append(""); }

//    mAlias[ id_to_index(id) ] = str;
//}
//QString DeviceModel::getAlias( int id )
//{
//    if ( id_to_index(id) >= mAlias.size() )
//    { return ""; }
//    else
//    { return mAlias[ id_to_index(id) ]; }
//}

QString DeviceModel::toString( int val, value_desc pDescs[], int32 cnt )
{
    for( int i = 0; i < cnt; i++ )
    {
        if ( pDescs[i].val == val )
        { return QString(pDescs[i].desc ); }
    }

    return QStringLiteral("UNK");
}

//! 0 -- no error
int DeviceModel::toValue( const QString &str, value_desc pDescs[], int cnt, int *pVal )
{
    for( int i = 0; i < cnt; i++ )
    {
        if ( str.compare( pDescs[i].desc, Qt::CaseInsensitive ) == 0 )
        {
            *pVal = pDescs[i].val;
            return 0;
        }
    }

    return -1;
}

QString DeviceModel::toString( byte v )
{ return QString::number( v ); }
QString DeviceModel::toString( uint32 v )
{ return QString::number( v ); }
QString DeviceModel::toString( int16 v )
{ return QString::number( v ); }
QString DeviceModel::toString( int32 v )
{ return QString::number( v ); }

QString DeviceModel::toString( float v )
{ return QString::number( v ); }

int DeviceModel::toValue( const QString &str, byte *pV )
{
    *pV = (byte)str.toInt();
    return 0;
}
int DeviceModel::toValue( const QString &str, uint32 *pV )
{
    *pV = str.toUInt();
    return 0;
}
int DeviceModel::toValue( const QString &str, int16 *pV )
{
    *pV = str.toShort();
    return 0;
}
int DeviceModel::toValue( const QString &str, uint16 *pV )
{
    *pV = str.toUShort();
    return 0;
}
int DeviceModel::toValue( const QString &str, int32 *pV )
{
    *pV = str.toInt();
    return 0;
}
int DeviceModel::toValue( const QString &str, float *pV )
{
    *pV = str.toFloat();
    return 0;
}

}
