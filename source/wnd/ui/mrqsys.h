#ifndef MRQSYS_H
#define MRQSYS_H

#include <QWidget>
#include "mrqview.h"

namespace Ui {
class mrqSys;
}

class mrqSys : public mrqView
{
    Q_OBJECT

public:
    explicit mrqSys(QWidget *parent = 0);
    ~mrqSys();

public:
    virtual int setApply();
protected:
    virtual void modelChanged();

protected:
    void setupUi();
    void desetupUi();

    int apply();
    int updateData();

protected:
    void on_chkLed_toggled( bool b );

private:
    Ui::mrqSys *ui;
};

#endif // MRQSYS_H
