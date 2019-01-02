#ifndef ENTITY_H
#define ENTITY_H

#include <QtCore>
#include <QDebug>

#include "entityarg.h"

class Entity
{
public:
    enum EntityType
    {
        entity_unk,

        entity_main,
        entity_sequence,
        entity_loop,

        entity_program,
        entity_call,

        entity_obj,

        entity_operation,
        entity_operator,
        entity_operand,

//        entity_com_operator,

        entity_mrq,
        entity_mrx_t4,
    };
protected:
    static QList<Entity*> _mObjs;
public:

public:
    Entity( const QString &objName="" );
    virtual ~Entity();

    virtual void postCtor();

    virtual QString snapInfo();

public:
    void setClassName( const QString &clsName );
    QString className();

    void setObjName( const QString &name );
    QString objName();

public:

public:
    int serialOut( QXmlStreamWriter &stream );
    int serialIn( QXmlStreamReader &stream );

protected:
    EntityType mType;
    QString mClass;

    QString mObjName;

protected:

};

#endif // ENTITY_H
