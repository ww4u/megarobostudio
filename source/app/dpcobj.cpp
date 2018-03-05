#include "dpcobj.h"
#include "../../include/mcstd.h"


dpcObj *dpcObj::_pSingleObj = NULL;
dpcObj* dpcObj::instance()
{
    return dpcObj::_pSingleObj;
}

dpcObj::dpcObj()
{
    Q_ASSERT( dpcObj::_pSingleObj == NULL );

    dpcObj::_pSingleObj = this;
}

void dpcObj::tlsProgress( int now, int from, int to )
{
    emit sig_prog_do( mTlsContext, now, from, to );
    logDbg();
}

void dpcObj::slot_post_do( appWidget::servContext context
                             )
{
    Q_ASSERT( NULL != context.m_pBase );
    Q_ASSERT( NULL != context.mGroup.mPostDo );

    emit sig_begin_do( context );

    //! call the proc
    int ret;

    mTlsContext = context;
    ret = (context.m_pBase->*(context.mGroup.mPostDo))( context.mMsg, context.m_pPara );

    emit sig_end_do( context, ret );
}
