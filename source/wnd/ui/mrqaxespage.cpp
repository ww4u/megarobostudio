#include "mrqaxespage.h"
#include "ui_mrqaxespage.h"

MrqAxesPage::MrqAxesPage(QWidget *parent) :
    mrqView(parent),
    ui(new Ui::MrqAxesPage)
{
    ui->setupUi(this);

    mSubViews.append( ui->pageMotor );
    mSubViews.append( ui->pageMotion );
    mSubViews.append( ui->pageTrigger );
    mSubViews.append( ui->pagePlan );
}

MrqAxesPage::~MrqAxesPage()
{
    delete ui;
}

#define FOREACH_SUBVIEW( api, para )    foreach( mrqView *pSubView, mSubViews )\
                                        { pSubView->api( para ); }
#define FOREACH_SUBVIEW_CALL( api )     foreach( mrqView *pSubView, mSubViews )\
                                        { pSubView->api(); }

void MrqAxesPage::setMcModel( mcModel *pMcModel )
{
    mrqView::setMcModel( pMcModel );

    FOREACH_SUBVIEW( setMcModel, pMcModel );
}

void MrqAxesPage::setModelObj( mcModelObj *pObj )
{
    mrqView::setModelObj( pObj );

    FOREACH_SUBVIEW( setModelObj, pObj );
}

void MrqAxesPage::setAxesId( int id )
{
    mrqView::setAxesId(id);

    FOREACH_SUBVIEW( setAxesId, id );
}
void MrqAxesPage::setPage( MRQ_AX_PAGE pg )
{
    mrqView::setPage( pg );

    FOREACH_SUBVIEW( setPage, pg );
}

int MrqAxesPage::setApply()
{
    mrqView::setApply();

    FOREACH_SUBVIEW_CALL( setApply );

    return 0;
}


