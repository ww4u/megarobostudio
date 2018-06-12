#include "basetype.h"

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
