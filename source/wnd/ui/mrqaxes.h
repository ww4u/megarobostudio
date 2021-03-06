#ifndef MRQAXES_H
#define MRQAXES_H

#include <QWidget>
#include <QValidator>

#include "mrqview.h"

namespace Ui {
class mrqAxes;
}

class mrqAxes : public mrqView
{
    Q_OBJECT

public:
    explicit mrqAxes(QWidget *parent = 0);
    ~mrqAxes();

public:
    virtual int setApply();
public:
    virtual void modelChanged();

protected:
    void spyEdited();
    void setupUi();
    void desetupUi();

    void adaptUi();

    int apply();
    int updateUi();

private slots:
    void on_cmbMotionType_currentIndexChanged(int index);

private:
    Ui::mrqAxes *ui;

    QDoubleValidator dValidator;

    QList <QWidget*> mRotateDependList;
    QList <QWidget*> mLineDependList;

};

#endif // MRQAXES_H
