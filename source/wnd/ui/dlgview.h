#ifndef DLGVIEW_H
#define DLGVIEW_H

#include <QDialog>
#include "../../model/mcmodel.h"

class DlgView : public QDialog
{
    Q_OBJECT
public:
    DlgView( mcModel *pModel, QWidget *parent = 0 );

protected:
    mcModel *m_pMcModel;
};

#endif // DLGVIEW_H
