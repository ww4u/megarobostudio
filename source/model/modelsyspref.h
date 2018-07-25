#ifndef MODELSYSPREF_H
#define MODELSYSPREF_H

#include "../intf/iserial.h"
#include "dbmeta.h"
#include "../para/syspara.h"

class modelSysPref : public SysPara, public ISerial
{
public:
    enum enumPort
    {
        port_mega_can,
        port_usb_can,
        port_rs232,
    };

public:
    modelSysPref();
    void rst();
public:
    virtual int save( const QString &str );
    virtual int load( const QString &str );

protected:
    int loadFromTo( QXmlStreamReader &reader, int &from, int &to );
    int loadDatabase( QXmlStreamReader &reader );

public:
    void setLatestPrj( const QString &path,
                       const QString &name);
    QString& latestPrjPath();
    QString& latestPrjName();

    int findAlias( const QString &sn, QString &alias );

public:
    dbMeta mDbMeta;

};

#endif // MODELSYSPREF_H
