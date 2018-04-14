#ifndef MRQAXES2_H
#define MRQAXES2_H

#include <QWidget>
#include "mrqview.h"

namespace Ui {
class mrqaxes2;
}

class mrqAxes2 : public mrqView
{
    Q_OBJECT

public:
    explicit mrqAxes2(QWidget *parent = 0);
    ~mrqAxes2();

protected Q_SLOTS:
    void on_btnInitZero_clicked();

public:
    virtual int setApply();
public:
    virtual void modelChanged();

protected:
    void spyEdited();
    void setupUi();
    void desetupUi();

    int apply();
    int updateUi();

private:
    Ui::mrqaxes2 *ui;
};

#endif // MRQAXES2_H
