#include "basetype.h"
#include "../../include/mydebug.h"
RpcRequest::RpcRequest( EnumRequest eReq,
                        EnumParaType pType)
{
    mReq = eReq;
    mParaType = pType;
}

void RpcRequest::setRequest( RpcRequest::EnumRequest eReq )
{ mReq = eReq; }
RpcRequest::EnumRequest RpcRequest::request()
{ return mReq; }

void RpcRequest::setParaType( EnumParaType pType )
{ mParaType = pType; }
RpcRequest::EnumParaType RpcRequest::paraType()
{ return mParaType; }

bool RpcRequest::checkRequest( EnumRequest eReq,
                   EnumParaType eType ) const
{
    do
    {
        if ( mReq != eReq )
        { return false; }

        if ( mParaType != eType )
        { return false; }

    }while(0);

    return true;
}

int RpcRequest::push( float v )
{
    append( (char*)&v, sizeof(v) );
    return 0;
}
int RpcRequest::push( int v )
{
    append( (char*)&v, sizeof(v) );
    return 0;
}

float RpcRequest::popFloat()
{
    Q_ASSERT( size() >= (int)sizeof(float) );

    //! get value
    float v;
    memcpy( &v, data(), sizeof(float) );

    remove( 0, sizeof(float) );

    return v;
}
int RpcRequest::popInt()
{
    Q_ASSERT( size() >= (int)sizeof(int) );

    //! get value
    int v;
    memcpy( &v, data(), sizeof(int) );

    remove( 0, sizeof(int) );

    return v;
}

QString MotionRow::encodeAttr( quint32 attr )
{
    if ( attr != 0 )
    { return QString("line=( enable = 1;)"); }
    else
    { return QString(); }
}

quint32 MotionRow::decodeAttr( const QString &str )
{
    if ( str.isEmpty() )
    { return 0; }

    bool bOk;
    int iVal;
    iVal = str.toInt( &bOk );
    if ( bOk )
    {
        if ( iVal > 0 )
        { return 1; }
        else
        { return 0; }
    }

    if ( str.contains( QRegExp("enable\\s?=\\s?1\\s?") ) )
    { return 1; }
    else
    { return 0; }
}

