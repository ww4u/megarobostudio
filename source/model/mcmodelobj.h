#ifndef MCMODELOBJ_H
#define MCMODELOBJ_H

#include <QtCore>


enum mcModelObj_Op
{
    model_obj_op_none = 0,
    model_obj_op_activate = 0,
    model_obj_op_new_mrp,
};

class mcModelObj
{
public:
    enum obj_type
    {
        model_none,
        model_bus,
        model_device,
        model_hub,
        model_composite_device,

        model_tpv,
        model_tp,

        model_robot,
        model_motion_file,
        model_motion_group,

        model_scene_variable,

        model_py_file,
        model_scene_file,

        model_setup_file,       //! many setup files
    };

protected:
    static QList<mcModelObj *> _model_objs;

public:
    static mcModelObj* allocate( obj_type type, void *pObj );
    static void decallocate( mcModelObj *pObj );

    static void gc();

public:
    mcModelObj();
    virtual ~mcModelObj();

public:
    void setGc( bool b );
    bool getGc( );

    void setFilled( bool b );
    bool isFilled();

    void setShadow( bool b );
    bool hasShadow();

    virtual int uploadSetting();

    virtual int tryLoad();

    virtual void setName( const QString &name );
    QString &getName();
    QString &name();

    virtual QString getFullDesc( int axesId = -1 );

    void setPath( const QString &path );
    QString getPath();

    void setType( obj_type type );
    obj_type getType();
    obj_type Type();
    QString typeString();

    void setObj( void *pObj );
    void* getObj();
    void* Obj();

    void set( obj_type type, void *pObj );

    void pushVar( const QVariant &var );
    void setVar( int id, const QVariant &var );
    QVariant* getVar( int id );

    QVariant *operator[]( int index );

protected:
    bool mbGc;
    bool mbFilled;      //! has filled
    bool mbShadow;

    QString mName;
    QString mPath;

    obj_type mType;
    void *m_pObj;

    QList< QVariant *> mVars;
};

Q_DECLARE_METATYPE( mcModelObj )
Q_DECLARE_METATYPE( mcModelObj * )

#endif // MCMODELOBJ_H
