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
protected:
    virtual void modelChanged();

protected:
    void setupUi();
    void desetupUi();

    int apply();
    int updateData();

private:
    Ui::mrqAxes *ui;

    QDoubleValidator dValidator;

};

#endif // MRQAXES_H
