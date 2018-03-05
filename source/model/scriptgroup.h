#ifndef SCRIPTGROUP_H
#define SCRIPTGROUP_H

#include <QtCore>
#include "scriptfile.h"
#include "../intf/iserial.h"

class scriptGroup :  public scriptNode
{
public:
    scriptGroup( const QString &name="" );
    virtual ~scriptGroup();

public:
    virtual int save( const QString &str );
    virtual int load( const QString &str );

    virtual int serialOut( QXmlStreamWriter &writer );
    virtual int serialIn( QXmlStreamReader &reader );
public:
    int saveAll( const QString &dir );

public:
    void dbgPrint();
    void iterGroup( scriptGroup *pGp );

protected:
    QString mPath;

};

Q_DECLARE_METATYPE(scriptGroup)
Q_DECLARE_METATYPE(scriptGroup*)

#endif // SCRIPTGROUP_H
