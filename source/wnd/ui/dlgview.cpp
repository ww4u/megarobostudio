#include "dlgview.h"

DlgView::DlgView( mcModel *pModel, QWidget *parent ) : QDialog( parent )
{
    Q_ASSERT( NULL != pModel );
    m_pMcModel = pModel;
}
