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
    mSubViews.append( ui->pageTune );

    spyEdited();
}

MrqAxesPage::~MrqAxesPage()
{
    delete ui;
}

void MrqAxesPage::changeEvent(QEvent * event)
{
    mrqView::changeEvent( event );

    if (event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi( this );

        for ( int i = 0; i < ui->tabWidget->count(); i++ )
        {
            qApp->postEvent( ui->tabWidget->widget( i ),
                             new QEvent( event->type() ) );
        }
    }
}

#define FOREACH_SUBVIEW( api, para )    foreach( mrqView *pSubView, mSubViews )\
                                        { pSubView->api( para ); }
#define FOREACH_SUBVIEW_CALL( api )     foreach( mrqView *pSubView, mSubViews )\
                                        { pSubView->api(); }

void MrqAxesPage::setMcModel( mcModel *pMcModel )
{
    FOREACH_SUBVIEW( setMcModel, pMcModel );

    mrqView::setMcModel( pMcModel );
}

void MrqAxesPage::setModelObj( mcModelObj *pObj )
{
    FOREACH_SUBVIEW( setModelObj, pObj );

    mrqView::setModelObj( pObj );
}

void MrqAxesPage::setAxesId( int id )
{
    FOREACH_SUBVIEW( setAxesId, id );

    mrqView::setAxesId(id);
}
void MrqAxesPage::setPage( MRQ_AX_PAGE pg )
{
    FOREACH_SUBVIEW( setPage, pg );

    mrqView::setPage( pg );
}

int MrqAxesPage::setApply()
{
    foreach( mrqView *pSubView, mSubViews )
    {
        pSubView->setApply();
    }

    mrqView::setApply();

    return 0;
}

void MrqAxesPage::modelChanged()
{
    FOREACH_SUBVIEW_CALL( modelChanged );

    mrqView::modelChanged();
}

void MrqAxesPage::adaptToModel()
{
    //! adapt to model
    if ( m_pMrqModel->tunningAble() )
    {
        if ( m_pMrqModel->axesPOSAble(mAxesId) )
        {}
        else
        { ui->tabWidget->removeTab( 2 ); }
    }
    else
    {
        ui->tabWidget->removeTab( 3 );

        if ( m_pMrqModel->axesPOSAble(mAxesId) )
        {}
        else
        { ui->tabWidget->removeTab( 2 ); }
    }
}

void MrqAxesPage::spyEdited()
{
    LINK_MODIFIED( ui->pageMotor );
    LINK_MODIFIED( ui->pageMotion );
    LINK_MODIFIED( ui->pageTrigger );
    LINK_MODIFIED( ui->pageTune );
}
