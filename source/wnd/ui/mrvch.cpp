#include "mrvch.h"
#include "ui_mrvch.h"

MrvCh::MrvCh(QWidget *parent) :
    mrvView(parent),
    ui(new Ui::MrvCh)
{
    ui->setupUi(this);

    mSubViews.append( ui->pageValve );
    mSubViews.append( ui->pageThreshold );
    mSubViews.append( ui->pageIo );

    spyEdited();
}

MrvCh::~MrvCh()
{
    delete ui;
}

#define FOREACH_SUBVIEW( api, para )    foreach( mrvView *pSubView, mSubViews )\
                                        { pSubView->api( para ); }
#define FOREACH_SUBVIEW_CALL( api )     foreach( mrvView *pSubView, mSubViews )\
                                        { pSubView->api(); }

void MrvCh::setMcModel( mcModel *pMcModel )
{
    FOREACH_SUBVIEW( setMcModel, pMcModel );

    mrvView::setMcModel( pMcModel );
}

void MrvCh::setModelObj( mcModelObj *pObj )
{
    FOREACH_SUBVIEW( setModelObj, pObj );

    mrvView::setModelObj( pObj );
}

void MrvCh::setAxesId(int id)
{
    mrvView::setAxesId( id );

    FOREACH_SUBVIEW( setAxesId, id );
}

int MrvCh::setApply()
{
    foreach( mrvView *pSubView, mSubViews )
    {
        pSubView->setApply();
//        QThread::msleep( 100 );     //! \todo need by mrq overflow
    }

    mrvView::setApply();

    return 0;
}

void MrvCh::modelChanged()
{
    FOREACH_SUBVIEW_CALL( modelChanged );

    mrvView::modelChanged();
}

void MrvCh::spyEdited()
{
    LINK_MODIFIED( ui->pageValve );
    LINK_MODIFIED( ui->pageThreshold );
    LINK_MODIFIED( ui->pageIo );
}

