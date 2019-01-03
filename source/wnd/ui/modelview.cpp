#include "modelview.h"

dpcObj *modelView::m_pDpcObj = NULL;

void modelView::attachDpcObj( dpcObj *pObj )
{
    Q_ASSERT( NULL != pObj );
    modelView::m_pDpcObj = pObj;
}

modelView::modelView( QWidget *parent ) : appWidget( parent )
{
    m_pModelObj = NULL;
    mbGc = false;

    m_pmcModel = NULL;
    mbModified = false;

    mbReqSpy = false;

    //! default
    mAttributes = 0;
    set_bit( mAttributes, OK_ABLE_BIT );
    set_bit( mAttributes, CANCEL_ABLE_BIT );
    set_bit( mAttributes, APPLY_ABLE_BIT );
    set_bit( mAttributes, RESET_ABLE_BIT );

    setAttribute( Qt::WA_DeleteOnClose );

    Q_ASSERT( NULL != modelView::m_pDpcObj );
    buildConnection();

}
modelView::~modelView()
{
    //! shadow gc in local
    if ( mbGc && NULL != m_pModelObj && m_pModelObj->getGc() )
    {
        m_pModelObj->setGc( false );
        delete m_pModelObj;
        m_pModelObj = NULL;
    }
}

void modelView::retranslate()
{}

void modelView::buildConnection()
{
    connect( this,
             SIGNAL(signal_post_do(appWidget::servContext)),
             m_pDpcObj,
             SLOT(slot_post_do(appWidget::servContext)) );
    connect( m_pDpcObj,
             SIGNAL(sig_begin_do(appWidget::servContext)),
             this,
             SLOT(slot_begin_do(appWidget::servContext)));
    connect( m_pDpcObj,
             SIGNAL(sig_end_do(appWidget::servContext,int)),
             this,
             SLOT(slot_end_do(appWidget::servContext,int)));
    connect( m_pDpcObj,
             SIGNAL(sig_prog_do(appWidget::servContext,int,int,int)),
             this,
             SLOT(slot_prog_do(appWidget::servContext,int,int,int)));
}

void modelView::adaptToUserRole()
{
    if ( sysMode() == sys_admin )
    {
    }
    else
    {
        unset_bit( mAttributes, OK_ABLE_BIT );
        unset_bit( mAttributes, APPLY_ABLE_BIT );
        unset_bit( mAttributes, RESET_ABLE_BIT );
    }
}

void modelView::adaptToModel()
{}

void modelView::slot_modified()
{
    slotModified( true );
}

void modelView::slotModified( bool b )
{
    mbModified = b;
    emit sigModified( b );
    emit sigModified( this, b );
}

void modelView::slot_request( const RpcRequest &req )
{
    if ( !isVisible() )
    { return; }

    if ( !mbReqSpy )
    { return; }

    RpcRequest lreq = req;
    onRequest( lreq );
}

void modelView::slot_device_busy( bool b)
{
    setEnabled( !b );
}

bool modelView::isCanceAble()
{ return is_bit1(mAttributes,CANCEL_ABLE_BIT); }
bool modelView::isOkAble()
{ return is_bit1(mAttributes,OK_ABLE_BIT); }
bool modelView::isApplyAble()
{ return is_bit1(mAttributes,APPLY_ABLE_BIT); }
bool modelView::isResetAble()
{ return is_bit1(mAttributes,RESET_ABLE_BIT); }

QString modelView::name()
{ return mName; }

void modelView::setReqSpy( bool b )
{ mbReqSpy = b; }
bool modelView::reqSpy()
{ return mbReqSpy; }

void modelView::setModelObj( mcModelObj *pObj )
{
    Q_ASSERT( NULL != pObj );

    m_pModelObj = pObj;
    mbGc = m_pModelObj->getGc();        //! shadow gc
}
mcModelObj *modelView::getModelObj()
{ return m_pModelObj; }

bool modelView::matchModelObj( mcModelObj *pObj )
{
    Q_ASSERT( NULL != pObj );
    Q_ASSERT( NULL != m_pModelObj );

    //! check shadow
    if ( pObj->hasShadow() && m_pModelObj->hasShadow() )
    {
//        if ( pObj->getType() != m_pModelObj->getType() )
//        { return false; }
//logDbg()<<m_pModelObj->getName()<<m_pModelObj->getPath();
//logDbg()<<pObj->getName()<<pObj->getPath();
        //! name match
        if ( str_is( m_pModelObj->getName(), pObj->getName() ) )
        {}
        else
        { return false; }

        //! path match
        QString natPath1, natPath2;
        natPath1 = QDir::fromNativeSeparators( m_pModelObj->getPath() );
        natPath2 = QDir::fromNativeSeparators( pObj->getPath() );

        //! remove the last seperator
        if ( natPath1.size() > 0 && natPath2.size() > 0 )
        {}
        else
        { return false; }

        if ( natPath1.back() == '/' )
        { natPath1.remove( natPath1.size()-1, 1 ); }

        if ( natPath2.back() == '/' )
        { natPath2.remove( natPath2.size()-1, 1 ); }

        if ( str_is( natPath1, natPath2) )
        {}
        else
        { return false; }

        return true;
    }
    else
    {
        return false;
    }
}

int modelView::save( QString &/*outFileName*/ )
{
    return 0;
}

int modelView::saveAs( QString &/*outFileName*/ )
{ return 0; }

int modelView::setApply()
{
    return 0;
}

int modelView::setReset()
{
    return 0;
}

QStringList & modelView::filePattern()
{
    return mFilePattern;
}

void modelView::setMcModel( mcModel *pMcModel )
{
    Q_ASSERT( NULL != pMcModel );

    m_pmcModel = pMcModel;
}

bool modelView::modified()
{ return mbModified; }

void modelView::syncData()
{
}
void modelView::updateScreen()
{}
void modelView::updateModel()
{}

void modelView::setActive()
{}

void modelView::onRequest( RpcRequest &req )
{

}
