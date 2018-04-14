#include "mcmodelobj.h"

QList<mcModelObj *> mcModelObj::_model_objs;

mcModelObj * mcModelObj::allocate( mcModelObj::obj_type type, void *pObj )
{
    mcModelObj *pModelObj;

    pModelObj = new mcModelObj();
    pModelObj->set( type, pObj );

    mcModelObj::_model_objs.append( pModelObj );

    return pModelObj;
}

void mcModelObj::decallocate( mcModelObj *pObj )
{
    Q_ASSERT( NULL != pObj );

    mcModelObj::_model_objs.removeAll( pObj );

    delete pObj;
}

void mcModelObj::gc()
{
    foreach( mcModelObj *pObj, mcModelObj::_model_objs )
    {
        Q_ASSERT( NULL != pObj );
        delete pObj;
    }

    mcModelObj::_model_objs.clear();
}

mcModelObj::mcModelObj()
{
    mType = model_none;
    m_pObj = NULL;

    mbGc = false;
    mbFilled = false;
    mbFile = false;
}

void mcModelObj::setGc( bool b )
{
    mbGc = b;
}
bool mcModelObj::getGc( )
{
    return mbGc;
}

void mcModelObj::setFilled( bool b )
{ mbFilled = b; }
bool mcModelObj::isFilled()
{ return mbFilled; }

void mcModelObj::setFile( bool b )
{
    mbFile = b;
}
bool mcModelObj::getFile()
{ return mbFile; }

mcModelObj::~mcModelObj()
{
    foreach( QVariant *pVar, mVars )
    {
        Q_ASSERT( NULL != pVar );
        delete pVar;
    }
}

void mcModelObj::setName( const QString &name )
{ mName = name; }
QString &mcModelObj::getName()
{ return mName; }

QString &mcModelObj::name()
{ return mName; }

void mcModelObj::setPath( const QString &path )
{ mPath = path; }
QString mcModelObj::getPath( )
{ return mPath; }

void mcModelObj::setType( mcModelObj::obj_type type )
{ mType = type; }
mcModelObj::obj_type mcModelObj::getType()
{ return mType; }
mcModelObj::obj_type mcModelObj::Type()
{ return mType; }

void mcModelObj::setObj( void *pObj )
{
    m_pObj = pObj;
}
void* mcModelObj::getObj()
{
    return m_pObj;
}
void* mcModelObj::Obj()
{ return m_pObj; }

void mcModelObj::set( mcModelObj::obj_type type, void *pObj )
{
    setType( type );

    setObj( pObj );
}

void mcModelObj::pushVar( const QVariant &var )
{
    QVariant *pVar;

    pVar = new QVariant( var );
    Q_ASSERT( NULL != pVar );

    mVars.append( pVar );
}
void mcModelObj::setVar( int id, const QVariant &var )
{
    Q_ASSERT( id >= 0 );

    //! add the pad
    QVariant *pVar;
    for ( int i = mVars.size(); i < id+1; i++ )
    {
        pVar = new QVariant();
        Q_ASSERT( NULL != pVar );
        mVars.append( pVar );
    }

    //! copy the data
    *mVars[id] = var;
}
QVariant* mcModelObj::getVar( int id )
{
    Q_ASSERT( id >= 0 && id < mVars.size() );

    return mVars[id];
}

QVariant *mcModelObj::operator[]( int index )
{
    return getVar( index );
}
