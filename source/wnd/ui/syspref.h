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
    e_rs232,
    e_usb,
};

#define port_can_mrh_e  "MRH-E"
#define port_usb_ii     "USB-CAN II"
#define port_mrh_t      "MRH-T"
#define port_mrh_u      "MRH-U"

#define port_mcp        "MCP"
#define port_rs232      "RS232"
#define port_usb        "USB"

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
//    bool validateVisaRsrc( QString &strIdn );

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

    void on_cmbPort_currentIndexChanged(const QString &arg1);

    void on_btnDetail_clicked();

    void on_btnVerify_clicked();

    void slot_updateValidateEn();

    void slot_styleLang_changed( int index );

//    void on_btnVerify_2_clicked();

//    void on_edtVisa_textChanged(const QString &arg1);

//    void on_btnAddT_clicked();

//    void on_btnRemove_clicked();

//    void on_btnClear_clicked();

//    void on_edtVisa_textEdited(const QString &arg1);

//    void slot_validate_listmrt();

//    void on_listMRTs_currentRowChanged(int currentRow);

    //! slot
    void slot_toolbar_add();
    void slot_toolbar_del();
    void slot_toolbar_clr();

    void on_gpAlias_toggled(bool arg1);

    void on_btnFont_clicked();

    void on_cmbStyle_currentIndexChanged(int index);

private:
    Ui::sysPref *ui;
    QWidget *m_pCANSetting, *m_pMRHTSetting;
    QWidget *m_pRs232Setting, *m_pUsbSetting, *m_pESetting;

public:
    modelSysPref *m_pPref;

};

#endif // SYSPREF_H
