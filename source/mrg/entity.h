#ifndef ENTITY_H
#define ENTITY_H

#include <QtCore>
#include <QDebug>

class Entity
{
public:
    enum EntityType
    {
        entity_unk,

        entity_main,
        entity_sequence,
        entity_loop,

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

    virtual bool isContainer();

public:
    QString className();

    void setObjName( const QString &name );
    QString objName();

    void setLabel( const QString &label );
    QString label();

//    void setGeo( const QRect &geo );
//    QRect geo();

//    void setLT( const QPoint &pt );
//    QPoint LT();

//    void setSize( const QSize &size );
//    QSize size();

public:
//    void attachPrecessor( Entity *p );
//    void detachPrecessor( Entity *p );

//    void attachSuccessor( Entity *p );
//    void detachSuccessor( Entity *p );

public:
    void traverse();

public:
    int serialOut( QXmlStreamWriter &stream );
    int serialIn( QXmlStreamReader &stream );

protected:
    EntityType mType;
    QString mClass;

    QString mObjName;
    QString mLabel;

    QRect mGeo;

protected:
//    Entity *m_pPrecessor;
//    QList<Entity*> mSuccessors;
};

#endif // ENTITY_H
