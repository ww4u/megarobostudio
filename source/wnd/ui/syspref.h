#ifndef SYSPREF_H
#define SYSPREF_H

#include <QtWidgets>

#include "../../model/modelsyspref.h"


namespace Ui {
class sysPref;
}

class sysPref : public QDialog
{
    Q_OBJECT

public:
    explicit sysPref(QWidget *parent = 0);
    ~sysPref();

    void setPref( const modelSysPref &pref );
    modelSysPref getPref();

public:
    void updateUi();
    void updateData();

protected:
    bool validateDb();
    bool updateValidateEn();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

    void on_cmbPort_currentIndexChanged(int index);

    void on_btnDetail_clicked();

    void on_btnVerify_clicked();

    void slot_updateValidateEn();

private:
    Ui::sysPref *ui;

    modelSysPref mPref;
};

#endif // SYSPREF_H
