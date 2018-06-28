#ifndef TPEDIT_H
#define TPEDIT_H

#include <QWidget>

#include "tableedit.h"
#include "../model/tpgroup.h"

namespace Ui {
class TpEdit;
}

class TpEdit : public tableEdit
{
    Q_OBJECT

public:
    explicit TpEdit(QWidget *parent = 0);
    ~TpEdit();

public:
    virtual void setModelObj( mcModelObj *pObj );
    virtual int save( QString &outFileName );
    virtual int saveAs( QString &outFileName );

protected:
    virtual void context_remove();
    virtual void context_add_before();
    virtual void context_add_below();
    virtual void context_clear();

protected Q_SLOTS:
    void slot_data_changed();

    void on_btnBuild_clicked();     //! build line

    void on_btnDown_clicked();
    void on_btnStart_clicked();
    void on_btnStop_clicked();

    void on_btnAdd_clicked();
    void on_btnDel_clicked();
    void on_btnClr_clicked();
    void on_btnGraph_clicked();

    void slot_download_cancel();

    void on_spinLoop_valueChanged(int arg1);

private:
    Ui::TpEdit *ui;

    TpGroup *m_pTpGroup;
};

#endif // TPEDIT_H
