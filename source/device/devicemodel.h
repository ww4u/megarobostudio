#ifndef DEVICEMODEL_H
#define DEVICEMODEL_H

#include <QtCore>
#include "../../include/mcstd.h"

//! id = -1, device -- name
//! id = 0,1,2,3... axis -- name
#define id_to_index( id )   ((id)+1)

struct value_desc
{
    int val;
    const char *desc;
};

//! converter
const char* enum_toString( int val, value_desc pDesc[], int32 cnt );
int enum_toValue( const char *pStr, value_desc pDesc[], int cnt, int *pVal );

namespace MegaDevice
{

class DeviceModel
{
public:
    DeviceModel();

public:
//    void setAlias( const QString &str, int id = -1 );
//    QString getAlias( int id = -1 );

    //! converter
    QString toString( int val, value_desc pDesc[], int32 cnt );
    int toValue( const QString &str, value_desc pDesc[], int cnt, int *pVal );

    QString toString( byte v );
    QString toString( uint32 v );

    QString toString( int16 v );
    QString toString( int32 v );

    QString toString( float v );

    int toValue( const QString &str, byte *pV );
    int toValue( const QString &str, uint32 *pV );
    int toValue( const QString &str, int16 *pV );
    int toValue( const QString &str, uint16 *pV );
    int toValue( const QString &str, int32 *pV );
    int toValue( const QString &str, float *pV );

public:
//    QString mAlias;
//    QStringList mAlias;

};

#define desc_table( table )  table, (int)( sizeof_array( table ) )

}

#endif // DEVICEMODEL_H
