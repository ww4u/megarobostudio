#ifndef SINANJUPREF_H
#define SINANJUPREF_H

#include <QtWidgets>

#include "modelview.h"

namespace Ui {
class SinanjuPref;
}

class SinanjuPref : public modelView
{
    Q_OBJECT

public:
    explicit SinanjuPref(QWidget *parent = 0);
    ~SinanjuPref();

protected:
    virtual void changeEvent( QEvent *event );

public:
    virtual void setModelObj( mcModelObj *pObj );
    virtual int setApply();

    virtual void updateScreen();
protected:
    void initModel();
    void spyEdited();
    void updateData();
    void updateUi();

    int applyGroupId();

private slots:
    void on_btnZeroArm_clicked();

    void on_btnZeroHand_clicked();

    void on_btnUploadZero_clicked();

    void on_btnZero_clicked();

    void on_btnFactory_clicked();

private:
    Ui::SinanjuPref *ui;
    QList<QTreeWidgetItem*> mItems;
    QList<QDoubleSpinBox *> mInitAngles;
    QList<QCheckBox *> mInvertAngles;

    QLabel *mLabels[8];
    QLineEdit *mEdits[8];
};

#endif // ROBOPREF_H
