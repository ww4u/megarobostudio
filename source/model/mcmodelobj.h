#ifndef MCMODELOBJ_H
#define MCMODELOBJ_H

#include <QtCore>


class mcModelObj
{
public:
    enum obj_type
    {
        model_none,
        model_bus,
        model_device,
        model_composite_device,
        model_tpv,

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
    ~mcModelObj();

public:
    void setGc( bool b );
    bool getGc( );

    void setFilled( bool b );
    bool isFilled();

    void setFile( bool b );
    bool getFile();

    void setName( const QString &name );
    QString &getName();
    QString &name();

    void setPath( const QString &path );
    QString getPath();

    void setType( obj_type type );
    obj_type getType();
    obj_type Type();

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
    bool mbFile;

    QString mName;
    QString mPath;

    obj_type mType;
    void *m_pObj;

    QList< QVariant *> mVars;
};

Q_DECLARE_METATYPE( mcModelObj )
Q_DECLARE_METATYPE( mcModelObj * )

#endif // MCMODELOBJ_H
