#ifndef SYSPREF_H
#define SYSPREF_H

#include <QtWidgets>

#include "../../model/modelsyspref.h"

namespace Ui {
class sysPref;
}

enum EnumCanPort
{
    e_can_mrh_e,
    e_can_usb_ii,
    e_can_mrh_t,
    e_can_mrh_u,

    e_can_mcp,
};

class sysPref : public QDialog
{
    Q_OBJECT

public:
    explicit sysPref(QWidget *parent = 0);
    ~sysPref();

    void setPref( modelSysPref *pPref );

public:
    void updateUi();
    void updateData();

protected:
    bool validateDb();
    bool updateValidateEn();
    bool validateVisaRsrc( QString &strIdn );

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

    void on_cmbPort_currentIndexChanged(int index);

    void on_btnDetail_clicked();

    void on_btnVerify_clicked();

    void slot_updateValidateEn();

    void slot_styleLang_changed( int index );

    void on_btnVerify_2_clicked();

    void on_edtVisa_textChanged(const QString &arg1);

    void on_btnAddT_clicked();

    void on_btnRemove_clicked();

    void on_btnClear_clicked();

    void on_edtVisa_textEdited(const QString &arg1);

    void slot_validate_listmrt();

    void on_listMRTs_currentRowChanged(int currentRow);

    //! slot
    void slot_toolbar_add();
    void slot_toolbar_del();
    void slot_toolbar_clr();

private:
    Ui::sysPref *ui;
    QWidget *m_pCANSetting, *m_pMRHTSetting;
public:
    modelSysPref *m_pPref;

};

#endif // SYSPREF_H
