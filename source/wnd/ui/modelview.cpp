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
    m_pmcModel = NULL;
    mbModified = false;

    mAttributes = 0;
    set_bit( mAttributes, OK_ABLE_BIT );
    set_bit( mAttributes, CANCEL_ABLE_BIT );
    set_bit( mAttributes, APPLY_ABLE_BIT );

    setAttribute( Qt::WA_DeleteOnClose );

    Q_ASSERT( NULL != modelView::m_pDpcObj );
    buildConnection();

}
modelView::~modelView()
{
    if ( NULL != m_pModelObj && m_pModelObj->getGc() )
    {
        delete m_pModelObj;
        m_pModelObj = NULL;
    }
}

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

//    connect( this, SIGNAL(sigModified(bool )),
//             this, SLOT(slot_modified()));
}

void modelView::slot_modified()
{
    logDbg();
    slotModified( true );
}

void modelView::slotModified( bool b )
{
    mbModified = b;
    emit sigModified( b );
    emit sigModified( this, b );
}

bool modelView::isCanceAble()
{ return is_bit1(mAttributes,CANCEL_ABLE_BIT); }
bool modelView::isOkAble()
{ return is_bit1(mAttributes,OK_ABLE_BIT); }
bool modelView::isApplyAble()
{ return is_bit1(mAttributes,APPLY_ABLE_BIT); }

void modelView::setModelObj( mcModelObj *pObj )
{
    Q_ASSERT( NULL != pObj );

    m_pModelObj = pObj;
}
mcModelObj *modelView::getModelObj()
{ return m_pModelObj; }

bool modelView::matchModelObj( mcModelObj *pObj )
{
    Q_ASSERT( NULL != pObj );
    Q_ASSERT( NULL != m_pModelObj );

    //! check file
    if ( pObj->getFile() )
    {
        if ( pObj->getType() != m_pModelObj->getType() )
        { return false; }

        //! name match
        if ( QString::compare( m_pModelObj->getName(), pObj->getName(),Qt::CaseInsensitive ) != 0 )
        { return false; }

        //! path match
        if ( QString::compare( m_pModelObj->getPath(), pObj->getPath(),Qt::CaseInsensitive ) != 0 )
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
